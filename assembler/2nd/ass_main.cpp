#include<string>
#include<map>

#include<cstdio>
#include<cstdlib>
#include<stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define MV2TKN(p) while(*p == ' ' || *p == '\t') p++
#define MV2SP(p) while(*p != ' ' && *p != '\t' && *p != '\n') p++
#define MV2NXTKN(p) MV2SP(p); MV2TKN(p)
#define GO2EL(p) while(*p != '\n') p++
#define IA "invalid argument"
#define NLCHK if (*p != '\n') throw "too many tokens"
using namespace std;
/*
   1. delete comments
   2. determin the data and text area
   3. find the entry point in text and store the lable referred to as it as 0 in mapping obj and get the last address, which is used to determine the allocation size for data on memory written afterward to file.
   4. store lable-address pairs on the data section to the map obj, which is used like above
   5. allocate memory
   6. write data there
   7. save those to file
   8. all done
*/


int TA, DA; // current address
char *TEX, *DAT; // tha data to be saved
typedef map<string, void (*) (char *) >::iterator mi;

extern map<string, void (*) (char *) > SZM;
extern map<string, void (*) (char *) > OPM;
map<string, int> LBM;


int streq(char *p, const char *str) {
	while(1) {
		if (*str == '\0') {
			if (*p == ' ' || *p == '\n' || *p == '\t')
				return 1;
			else
				return 0;
		} else if (*p != *str)
			return 0;
		p++; str++;
	}
}

int slblchk(char *p, char *slbl) {
	while(1) {
		if (*p != *slbl) return 0;
		p++; slbl++;
		if (*slbl == ' ' || *slbl == '\t' || *slbl == '\n')
			if (*p == ':') {
				p++; MV2TKN(p);
				if (*p == '\n') return 1;
				else return 0;
			}
	}
}

void print_line(FILE *file, char *p) {
	putc(*p, file);
	while(*p != '\n') {
		p++;
		putc(*p, file);
	}
}
void print_numbered_line(FILE *file, char *p, int n) {
	fprintf(file, "%d", n);
	print_line(file, p);
}

int main(int argc, char *argv[]) {
	int ln, tsln, teln, dsln; // num of currently parsing line
	int fd, len, r, rv, tfs, dfs;
	struct stat statbuf;
	char *fc, *flast, *p, *dstart, *dlast, *tstart, *tentry;
	FILE *numbered_file;
	if (argc <= 2) {
		puts("too few arguments");
		return 1;
	}
	fd = open(argv[1], O_RDONLY);
	if (fd <= 0) {
		perror("main");
		return 1;
	}
	numbered_file = fopen(argv[2], "w");
	if (numbered_file == NULL) {
		perror("main");
		return 1;
	}
	if (fstat(fd, &statbuf) != 0) {
		perror("main");
		return 1;
	}
	len = statbuf.st_size;
	fc = (char*) malloc(len+1);
	if (fc == NULL) {
		puts("memory allocation error");
		return 1;
	}
	p = fc;
	for(r = len; r > 0;) {
		rv = read(fd, p, r);
		if (rv < 0) {
			perror("main");
			return 1;
		}
		r -= rv;
		p += rv;
	}
	close(fd);
	flast = p;
	*flast = '\n';
	/* delete all comments */
	for(p = fc; p <= fc + len; p++) {
		if (p[0] == '#' || p[0] == '!') {
			do {*p = ' '; p++;} while (*p != '\n');
		}
	}
	
	/* determine the data and text region and get the entry point */
	ln = 1;
	try {
		/* determine the data range */
		for(p = fc; p <= flast; p++) {
			if (*p == '\n') ln++;
			else if (*p != ' ' && *p != '\t') break;
		}
		if (p > flast) throw "empty file";
		if (!streq(p, ".section\t\".rodata\""))
			throw "start file with .section\t\".rodata\"";
		MV2NXTKN(p);MV2NXTKN(p);
		NLCHK;
		p++;
		ln++;
		dsln = ln;
		dstart = p;
		if (p > flast) throw "cannnot find text section";
		while(1) {
			MV2TKN(p);
			if (streq(p, ".section\t\".text\""))
				break;
			GO2EL(p);
			p++;
			ln++;
			if (p > flast) throw "cannnot find text section";
		}
		for(; *p != '\n'; p--);
		dlast = p;
		p++;
		MV2TKN(p);MV2NXTKN(p);MV2NXTKN(p);
		NLCHK;
		p++;
		ln++;
		/* determine the text range */
		tsln = ln;
		tstart = p;
		while(1) {
			MV2TKN(p);
			if (streq(p, ".global"))
				break;
			GO2EL(p);
			p++;
			ln++;
			if (p > flast) throw "cannnot find .global directive";
		}
		MV2NXTKN(p);
		if (*p == '\n') throw IA;
		char *q = p;
		MV2NXTKN(p);
		if (*p != '\n') throw IA;
		/* q points the starting lable name */
		p = tstart; ln = tsln;
		while(1) {
			MV2TKN(p);
			if (slblchk(p, q)) break;
			GO2EL(p);
			p++;
			ln++;
			if (p > flast) throw "cannnot find entry lable";
		}
		teln = ln;
		for(tentry = p; *tentry != '\n'; tentry--);
		tentry++;
	}
	catch (char const *s) {
		printf("%d: %s\n", ln, s);
		return 1;
	}
	
	try {
		/* data section analysis */
		DA = 0; ln = dsln; p = dstart;
		while(p <= dlast) {
			char *q, c;
			//int32_t ui;
			mi i;
		
			MV2TKN(p);
			if (*p == '\n') {
				ln++; p++;
				continue;
			}
			
			
			q = p;
			MV2SP(q);
			c = *q;
			*q = '\0';
			i = SZM.find(p);
			*q = c;
			if (i != SZM.end()) {
				MV2TKN(q);
				(i->second)(q);
				p = q;
				GO2EL(p);
			} else {
				q--;
				if (*q == ':') {
					if (p >= q) throw "invalid lable";
					*q = '\0';
					LBM[p] = DA;
					*q = ':';
				} else {
					DA++;
				}
				p = q + 1;
				MV2TKN(p);
				NLCHK;
			}
			p++; ln++;
		}
		
		/* text section analysis */
		TA = 0; ln = teln; p = tentry;
		for(char *p0 = p; p <= flast; p0 = p) {
			char c, *q;
			mi i;
			
			
			MV2TKN(p);
			if (*p == '\n') {
				print_line(numbered_file, p0);
				ln++; p++;
				continue;
			}
			q = p;
			MV2SP(q);
			c = *q;
			*q = '\0';
			i = SZM.find(p);
			*q = c;
			if (i != SZM.end()) {
				print_numbered_line(numbered_file, p0, TA);
				MV2TKN(q);
				(i->second)(q);
				p = q;
				GO2EL(p);
			} else {
				print_line(numbered_file, p0);
				q--;
				if (*q == ':' && p < q) {
					*q = '\0';
					LBM[p] = TA;
					*q = ':';
					p = q + 1;
					MV2TKN(p);
					NLCHK;
				} else {
					throw "unknown operator";
				}
			}
			p++; ln++;
		}
		TA++; // add buffer
		ln = tsln; p = tstart;
		while(p < tentry) {
			char c, *q;
			mi i;
			
			
			MV2TKN(p);
			if (*p == '\n') {
				ln++; p++;
				continue;
			}
			q = p;
			MV2SP(q);
			c = *q;
			*q = '\0';
			i = SZM.find(p);
			*q = c;
			if (i != SZM.end()) {
				MV2TKN(q);
				(i->second)(q);
				p = q;
				GO2EL(p);
			} else {
				q--;
				if (*q == ':' && p < q) {
					*q = '\0';
					LBM[p] = TA;
					*q = ':';
					p = q + 1;
					MV2TKN(p);
					NLCHK;
				} else {
					throw "unknown operator";
				}
			}
			p++; ln++;
		}
	}
	catch (char const *s) {
		printf("%d: %s\n", ln, s);
		return 1;
	}
	catch (...) {
		puts("some error has occurred");
		return 1;
	}
	/* allocate memory for enbedding data to be written to file beforehand */
	tfs = TA*4; dfs = DA*4;
	TEX = (char*)calloc(tfs+1, 1);
	DAT = (char*)calloc(dfs+1, 1);
	if (TEX == NULL || DAT == NULL) {
		puts("memory allocation error");
		return 1;
	}
	
	try {
		/* data section coding */
		DA = 0; ln = dsln; p = dstart;
		while(p <= dlast) {
			char *q, c;
			int32_t ui;
			mi i;
		
			MV2TKN(p);
			if (*p == '\n') {
				ln++; p++;
				continue;
			}
			q = p;
			MV2SP(q);
			c = *q;
			*q = '\0';
			i = SZM.find(p);
			*q = c;
			if (i != SZM.end()) {
				MV2TKN(q);
				(i->second)(q);
				p = q;
				GO2EL(p);
			} else {
				if (*(q-1) != ':') {
					if ('0' <= *p && *p <= '9') { // num literal case
						ui = (int32_t)strtol(p, &p, 0);
						if (p != q) throw "invalid number literal";
					} else {
						c = *q;
						*q = '\0';
						map<string, int>::iterator itr = LBM.find(p);
						*q = c;
						if (itr == LBM.end()) throw "unknown lable referenced";
						ui = itr->second;
						p = q;
					}
					*((int32_t *)(&DAT[DA*4])) = ui;
					DA++;
				}
				MV2TKN(p);
			}
			p++; ln++;
		}
		/* text section coding */
		TA = 0; ln = teln; p = tentry;
		while(p <= flast) {
			char c, *q;
			mi i;
			
			
			MV2TKN(p);
			if (*p == '\n') {
				ln++; p++;
				continue;
			}
			q = p;
			MV2SP(q);
			c = *q;
			*q = '\0';
			i = OPM.find(p);
			*q = c;
			p = q;
			if (i != OPM.end()) {
				MV2TKN(p);
				(i->second)(p);
			}
			GO2EL(p);
			p++; ln++;
		}
		*((uint32_t *)(&TEX[TA*4])) = 0xFFFFFFFF;
		TA++;
		ln = tsln; p = tstart;
		while(p < tentry) {
			char c, *q;
			mi i;
			
			
			MV2TKN(p);
			if (*p == '\n') {
				ln++; p++;
				continue;
			}
			q = p;
			MV2SP(q);
			c = *q;
			*q = '\0';
			i = OPM.find(p);
			*q = c;
			p = q;
			if (i != OPM.end()) {
				MV2TKN(p);
				(i->second)(p);
			}
			GO2EL(p);
			p++; ln++;
		}
	}
	catch (char const *s) {
		printf("%d: %s\n", ln, s);
		return 1;
	}
	catch (...) {
		puts("some error has occurred");
		return 1;
	}
	
	fd = open("text", O_WRONLY|O_CREAT|O_TRUNC, 0777);
	if (fd < 0) {
		perror("main");
		return 1;
	}
	p = TEX;
	for(r = tfs; r > 0;) {
		rv = write(fd, p, r);
		if (rv < 0) {
			perror("main");
			return 1;
		}
		r -= rv;
		p += rv;
	}
	close(fd);
	fd = open("data", O_WRONLY|O_CREAT|O_TRUNC, 0777);
	if (fd < 0) {
		perror("main");
		return 1;
	}
	p = DAT;
	for(r = dfs; r > 0;) {
		rv = write(fd, p, r);
		if (rv < 0) {
			perror("main");
			return 1;
		}
		r -= rv;
		p += rv;
	}
	close(fd);
	fclose(numbered_file);
	free(fc); free(TEX); free(DAT);
	return 0;
}

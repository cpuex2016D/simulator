#include<cstdio>
#include<cstdlib>
#include<stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include<map>

#define MV2TKN(p) while(*p == ' ' || *p == '\t') p++
#define MV2SP(p) while(*p != ' ' && *p != '\t' && *p != '\n') p++
#define MV2NXTKN(p) MV2SP(p); MV2TKN(p)
#define GO2EL(p) while(*p != '\n') p++
#define IA "invalid argument"
#define NLCHK if (*p != '\n') throw "too many tokens"

using namespace std;

int TA, DA; // current address
char *TEX, *DAT; // tha data to be saved

extern map<char*, void (*) (char*), bool (*) (char*, char*)> SZM;
extern map<char*, void (*) (char*), bool (*) (char*, char*)> OPM;

typedef map<char*, void (*) (char*), bool (*) (char*, char*)>::iterator mi;
typedef map<char*, int, bool (*) (char*, char*)>::iterator lmi;

bool lablecmp(char* p1, char* p2) {
    while(1) {
        if (*p1 == ' ' || *p1 == '\n' || *p1 == '\t') {
            if (*p2 == ' ' || *p2 == '\n' || *p2 == '\t') {
                return 0;
            } else if (*p2 == ':') {
                p2++;
                if (*p2 == ' ' || *p2 == '\n' || *p2 == '\t') {
                    return 0;
                }
            }
            return 1;
        }
        if (*p2 == ' ' || *p2 == '\n' || *p2 == '\t') {
            return 0;
        }
        if (*p1 < *p2) {
            return 1;
        } else if (*p1 > *p2) {
            return 0;
        }
        p1++; p2++;
    }
}

map<char*, int, bool (*) (char*, char*)> LBM(lablecmp);

void* fopen_on_memory(char *fname, int *fsize) {
	struct stat statbuf;
	int fd = open(fname, O_RDONLY);
	
	if (fd <= 0) {
		perror("fopen_on_memory");
		return NULL;
	}
	if (fstat(fd, &statbuf) != 0) {
		perror("fopen_on_memory");
		close(fd);
		return NULL;
	}
	*fsize = statbuf.st_size;
	
	void *rp = malloc(*fsize+1);
	char *tp = (char *)rp;
	if (rp == NULL) {
		puts("malloc failed");
		close(fd);
		return NULL;
	}
	for(int r = *fsize; r > 0;) {
		int rv = read(fd, tp, r);
		if (rv < 0) {
			perror("fopen_on_memory");
			close(fd);
			free(rp);
			return NULL;
		}
		r -= rv;
		tp += rv;
	}
	close(fd);
	return rp;
}

void delete_comments(char *start, char *end) {
	for(; start <= end; start++) {
		if (start[0] == '#' || start[0] == '!') {
			do {*start = ' '; start++;} while (*start != '\n');
		}
	}
}

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

bool is_lable(char *p) {
	p++;
	if (*p == ' ' || *p == '\t' || *p == '\n')
		return false;
	MV2SP(p);
	if (*(p-1) == ':')
		return true;
	else
		return false;
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

int output_file(const char *fname, char *data, int size) {
	int fd;
	fd = open(fname, O_WRONLY|O_CREAT|O_TRUNC, 0777);
	if (fd < 0) {
		perror("output_file");
		return 0;
	}
	while(size > 0) {
		int rv = write(fd, data, size);
		if (rv < 0) {
			perror("output_file");
			close(fd);
			return 0;
		}
		size -= rv;
		data += rv;
	}
	close(fd);
	return 1;
}
	
int main(int argc, char *argv[]) {
	int len, ln, tsln, dsln; // num of currently parsing line
	char *fc, *flast, *p, *dstart, *dlast, *tstart;
	FILE *numbered_file;
	
	
	if (argc <= 2) {
		puts("too few arguments");
		return 1;
	}
	
	fc = (char*)fopen_on_memory(argv[1], &len);
	if (fc == NULL) {
		return 1;
	}
	flast = fc + len;
	*flast = '\n';
	delete_comments(fc, flast);
	
	numbered_file = fopen(argv[2], "w");
	if (numbered_file == NULL) {
		perror("main");
		return 1;
	}
	
	
	ln = 1;
	try {
		ln = 1; p = fc;
		/* determine the data range */
		while(p <= flast) {
			MV2TKN(p);
			if (*p == '\n') {
				p++;
				ln++;
			} else break;
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
		while(p <= flast) {
			MV2TKN(p);
			if (streq(p, ".section\t\".text\""))
				break;
			GO2EL(p);
			p++;
			ln++;
		}
		if (p > flast) throw "cannnot find text section";
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
	}
	catch (char const *s) {
		printf("%d: %s\n", ln, s);
		return 1;
	}
	
	
	try {
		/* data section analysis */
		DA = 0; ln = dsln; p = dstart;
		while(p <= dlast) {
			MV2TKN(p);
			if (streq(p, ".align")) {
				GO2EL(p);
			} else if (*p != '\n') {
				if (is_lable(p)) {
					LBM[p] = DA;
				} else {
					DA++;
				}
				MV2NXTKN(p);
				NLCHK;
			}
			ln++; p++;
		}
		
		/* text section analysis */
		TA = 0; ln = tsln; p = tstart;
		while(p <= flast) {
			char *p0 = p;
			MV2TKN(p);
			if (*p == '\n') {
				print_line(numbered_file, p0);
				ln++; p++;
				continue;
			}
			mi i = SZM.find(p);
			if (i != SZM.end()) {
				print_numbered_line(numbered_file, p0, TA);
				MV2NXTKN(p);
				(i->second)(p);
				GO2EL(p);
			} else {
				print_line(numbered_file, p0);
				if (is_lable(p)) {
					LBM[p] = TA;
					MV2NXTKN(p);
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
	
	int tfs = TA*4, dfs = DA*4;
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
			MV2TKN(p);
			if (!streq(p, ".align") && *p != '\n' && !is_lable(p)) {
				int32_t ui;
				if ('0' <= *p && *p <= '9') {
					ui = (int32_t)strtol(p, NULL, 0);
				} else {
					lmi itr = LBM.find(p);
					if (itr == LBM.end()) throw "unknown lable referenced";
					ui = itr->second;
				}
				*((int32_t *)(&DAT[DA*4])) = ui;
				DA++;
			}
			GO2EL(p);
			ln++; p++;
		}
		/* text section coding */
		TA = 0; ln = tsln; p = tstart;
		while(p <= flast) {
			MV2TKN(p);
			if (*p == '\n') {
				ln++; p++;
				continue;
			}
			mi i = OPM.find(p);
			if (i != OPM.end()) {
				MV2NXTKN(p);
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
	
	if (!output_file("text", TEX, tfs)) {
		return 1;
	}
	if (!output_file("data", DAT, dfs)) {
		return 1;
	}
	
	fclose(numbered_file);
	free(fc); free(TEX); free(DAT);
	return 0;
}
	
	
	
	

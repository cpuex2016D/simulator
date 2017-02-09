#include"sim.h"

#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>

void* open_text(char *fname, int *fsize);
void* open_data(char *fname, int *fsize);
int simprepare(int argc, char *argv[]) {
	if (argc != 5) {
		fprintf(stderr, "specify 3 files: parent-text child-text data-text input");
		return 0;
	}
	int fsize;
	PTEX = (uint32_t*)open_text(argv[1], &fsize);
	PTEX_LAST = fsize / 4;
	CTEX = (uint32_t*)open_text(argv[2], &fsize);
	CTEX_LAST = fsize / 4;
	if (PTEX == NULL || CTEX == NULL) {
		return 0;
	}


	IFILE = fopen(argv[4], "r");
	if (IFILE == NULL) {
		perror("main");
		return 0;
	}

	for(int c = 0; c < 4; c++) {
		/*for(int i = 0; i < 32; i++) {
			CE[c].GPR[i] = 0;
			CE[c].FPR[i] = 0.0;
		}*/
		for(int i = 0; i < 8; i++) {
			CE[c].FPCC[i] = '0';
		}
		CE[c].FPCC[8] = '\0';
		CE[c].DAT = (uint32_t*)open_data(argv[3], &fsize);
		CE[c].GPR[0] = 0;
		CE[c].GPR[29] = fsize/4;
		CE[c].GPR[30] = 0x40000;
		if (CE[c].DAT == NULL) {
			return 0;
		}
		CE[c].PJ = 0;
	}
	return 1;
}


void* open_text(char *fname, int *fsize) {
	struct stat statbuf;
	int fd = open(fname, O_RDONLY);
	if (fd <= 0) {
		perror("main");
		return NULL;
	}
	if (fstat(fd, &statbuf) != 0) {
		perror("main");
		return NULL;
	}
	*fsize = statbuf.st_size;
	if (statbuf.st_size%4 != 0) {
		fprintf(stderr, "text file size is not multiples of 4\n");
		return NULL;
	}
	char *rp = (char *)calloc(statbuf.st_size+4, 1);
	if (rp == NULL) {
		fprintf(stderr, "malloc error\n");
		return NULL;
	}
	char *p = rp;
	for(int r = statbuf.st_size; r > 0;) {
		int rv = read(fd, p, r);
		if (rv < 0) {
			perror("main");
			return NULL;
		}
		r -= rv;
		p += rv;
	}
	close(fd);
	return (void*)rp;
}

void* open_data(char *fname, int *fsize) {
	struct stat statbuf;
	int fd = open(fname, O_RDONLY);
	if (fd <= 0) {
		perror("main");
		return NULL;
	}
	if (fstat(fd, &statbuf) != 0) {
		perror("main");
		return NULL;
	}
	*fsize = statbuf.st_size;
	if (statbuf.st_size%4 != 0 || statbuf.st_size > 1024*1024*2) {
		fprintf(stderr, "data file size error\n");
		return NULL;
	}
	char *rp = (char *)calloc(1024*1024*2, 1);
	if (rp == NULL) {
		fprintf(stderr, "malloc error\n");
		return NULL;
	}
	char *p = rp;
	for(int r = statbuf.st_size; r > 0;) {
		int rv = read(fd, p, r);
		if (rv < 0) {
			perror("main");
			return NULL;
		}
		r -= rv;
		p += rv;
	}
	close(fd);
	return (void*)rp;
}


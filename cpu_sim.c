#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include <fcntl.h>

#define GPR_SIZE 32
#define FPR_SIZE 32
#include"op_def.h"
#define MAX_(X, Y) (((X)>(Y)) ? (X) : (Y))

uint32_t PC = 0;
uint32_t GPR[GPR_SIZE] = {0};
float FPR[FPR_SIZE] = {0.0};
uint32_t OP = 0;
uint32_t *TEX = NULL;
uint32_t *DAT = NULL;
unsigned char FPCC = 0;

/*
void print_reg(void) {
	int i;
	fprintf(stderr, "PC: %d\tOP:%08X\tFPCC: %02X\n", PC, OP, FPCC);
	puts("GPR");
	for(i = 0; i < GPR_SIZE; i += 4) {
		printf("%d-%d\t%d\t%d\t%d\t%d\n", i, i+3, GPR[i], GPR[i+1], GPR[i+2], GPR[i+3]);
	}
	puts("FPR");
	for(i = 0; i < FPR_SIZE; i += 4) {
		printf("%d-%d\t%f\t%f\t%f\t%f\n", i, i+3, FPR[i], FPR[i+1], FPR[i+2], FPR[i+3]);
	}
	puts("\n\n");
	return;
}
*/
void print_reg(void) {
	fprintf(stderr, "PC: %d\tOP:%08X\tFPCC: %02X\nGPR\n0-3\t%d\t%d\t%d\t%d\n4-7\t%d\t%d\t%d\t%d\n8-11\t%d\t%d\t%d\t%d\n12-15\t%d\t%d\t%d\t%d\n16-19\t%d\t%d\t%d\t%d\n20-23\t%d\t%d\t%d\t%d\n24-27\t%d\t%d\t%d\t%d\n28-31\t%d\t%d\t%d\t%d\nFPR\n0-3\t%f\t%f\t%f\t%f\n4-7\t%f\t%f\t%f\t%f\n8-11\t%f\t%f\t%f\t%f\n12-15\t%f\t%f\t%f\t%f\n16-19\t%f\t%f\t%f\t%f\n20-23\t%f\t%f\t%f\t%f\n24-27\t%f\t%f\t%f\t%f\n28-31\t%f\t%f\t%f\t%f\n\n\n", PC, OP, FPCC, GPR[0], GPR[1], GPR[2], GPR[3], GPR[4], GPR[5], GPR[6], GPR[7], GPR[8], GPR[9], GPR[10], GPR[11], GPR[12], GPR[13], GPR[14], GPR[15], GPR[16], GPR[17], GPR[18], GPR[19], GPR[20], GPR[21], GPR[22], GPR[23], GPR[24], GPR[25], GPR[26], GPR[27], GPR[28], GPR[29], GPR[30], GPR[31], FPR[0], FPR[1], FPR[2], FPR[3], FPR[4], FPR[5], FPR[6], FPR[7], FPR[8], FPR[9], FPR[10], FPR[11], FPR[12], FPR[13], FPR[14], FPR[15], FPR[16], FPR[17], FPR[18], FPR[19], FPR[20], FPR[21], FPR[22], FPR[23], FPR[24], FPR[25], FPR[26], FPR[27], FPR[28], FPR[29], FPR[30], FPR[31]);
	return;
}

int main(int argc, char *argv[]) {
	int fsize, r, rv, fd, lastpc;
	void *p;
	struct stat statbuf;
	op_set *opp;
	if (argc < 3) {
		fprintf(stderr, "too few arguments\n");
		return 0;
	}
	
	fd = open(argv[1], O_RDONLY);
	if (fd <= 0) {
		perror("main");
		return 1;
	}
	if (fstat(fd, &statbuf) != 0) {
		perror("main");
		return 1;
	}
	fsize = statbuf.st_size;
	lastpc = (fsize + 3)/4 - 1;
	p = malloc(fsize+4);
	if (p == NULL) {
		fprintf(stderr, "malloc error\n");
		return 0;
	}
	TEX = (uint32_t *) p;
	for(r = fsize; r > 0;) {
		rv = read(fd, p, r);
		if (rv < 0) {
			perror("main");
			return 1;
		}
		r -= rv;
		p += rv;
	}
	close(fd);
	
	fd = open(argv[2], O_RDONLY);
	if (fd <= 0) {
		perror("main");
		return 1;
	}
	if (fstat(fd, &statbuf) != 0) {
		perror("main");
		return 1;
	}
	fsize = statbuf.st_size;
	if (fsize >= 1024*1024*1) {
		fprintf(stderr, "data file size is too large\nshould be less than 1 MB\n");
		return 1;
	}
	p = malloc(1024*1024*2);
	if (p == NULL) {
		fprintf(stderr, "malloc error\n");
		return 0;
	}
	DAT = (uint32_t *) p;
	for(r = fsize; r > 0;) {
		rv = read(fd, p, r);
		if (rv < 0) {
			perror("main");
			return 1;
		}
		r -= rv;
		p += rv;
	}
	close(fd);

	GPR[0] = 0;
	GPR[29] = fsize/4;
	GPR[30] = 0x40000;
	for(PC = 0; PC <= lastpc;) {
		OP = TEX[PC];
		//print_reg();
		PC += 1;
		for(opp = op_array;;opp++) {
			if (opp->is_op == NULL) {
				fprintf(stderr, "undefined: %08X\n", OP);
				return 0;
			}
			if ((*opp->is_op)()) {
				(*opp->op)();
				break;
			}
		}
	}
	print_reg();
	free(DAT);
	free(TEX);
	return 0;
}

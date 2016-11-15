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

void print_reg(void) {
	int i;
	printf("PC: %d\tOP:%08X\tFPCC: %02X\n", PC, OP, FPCC);
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
	

int main(int argc, char *argv[]) {
	int fsize, r, rv, fd, lastpc;
	void *p;
	struct stat statbuf;
	op_set *opp;
	if (argc < 3) {
		puts("too few arguments");
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
		puts("malloc error");
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
	p = malloc(MAX_(fsize+4, 1024*1024*4));
	if (p == NULL) {
		puts("malloc error");
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

	for(PC = 0; PC <= lastpc;) {
		OP = TEX[PC];
		print_reg();
		while(getchar() != '\n');
		PC += 1;
		for(opp = op_array;;opp++) {
			if (opp->is_op == NULL) {
				printf("undefined: %08X\n", OP);
				return 0;
			}
			if ((*opp->is_op)()) {
				(*opp->op)();
				break;
			}
		}
	}

	free(DAT);
	free(TEX);
	return 0;
}

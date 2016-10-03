#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#define GPR_SIZE 32
#define FPR_SIZE 32
#include"op_def.h"

uint32_t PC = 0;
uint32_t GPR[GPR_SIZE];
float FPR[FPR_SIZE];
uint32_t OP = 0;
void *MEM = NULL;
unsigned char FPCC = 0;

int main(int argc, char *argv[]) {
	FILE *fp;
	int fsize, r, s;
	void *p;
	int (**opp) (void);
	if (argc < 2) return 0;
	fp = fopen(argv[1], "r");
	if (fp == NULL) return 0;
	fseek(fp, 0, SEEK_END);
	fsize = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	MEM = malloc(fsize);
	if (MEM == NULL) return 0;
	p = MEM;
	for(r = fsize; r > 0;) {
		s = fread(p, 1, r, fp);
		if (s <= 0) return 0;
		p += s;
		r -= s;
	}
	fclose(fp);

	for(PC = 0;;) {
		OP = *((uint32_t*)(MEM+PC));
		PC += 4;
		for(opp = op_array;;opp++) {
			if (*opp == NULL) {
				printf("undefined: %08X\n", OP);
				return 0;
			}
			if ((*opp)()) {
				break;
			}
		}
	}

	free(MEM);
	return 0;
}

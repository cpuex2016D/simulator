#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<set>
#include"examine_op.h"
#define CPSWAP(X, Y) {char *emvc2m8 = X; X = Y; Y = emvc2m8;}
#define WNSZ 0x100
using namespace std;

int PC;
uint32_t GPR[32];
char FPCC[9] = "00000000";
float FPR[32];
uint32_t OP;
uint32_t *TEX, *DAT;
FILE *IFILE;

void (*OP_EX) (void);
void (*OP_PRNT) (void);
int STOP;

int PJ = 0;
enum {ADD_L, ADDI_L, SUB_L, AND_L, ANDI_L, OR_L, ORI_L, NOR_L, SLL_L, SRL_L, SLT_L, SLTI_L, BEQ_L, BNE_L, J_L, JAL_L, JR_L, JALR_L, LW_L, LUI_L, SW_L, IN_L, OUT_L, BT_S_L, BF_S_L, ADD_S_L, SUB_S_L, MUL_S_L, DIV_S_L, MOV_S_L, NEG_S_L, ABS_S_L, SQRT_S_L, C_EQ_S_L, C_LT_S_L, C_LE_S_L, LW_S_L, SW_S_L, FTOI_L, ITOF_L, OP_TOTAL};
unsigned long long OP_COUNT[OP_TOTAL] = {0};
int OP_TYPE;
unsigned long long COUNTS;

set<int> BREAKPOINTS;



int simprepare(int, char*[], int*);

void print_state(void) {
	OP_PRNT();
	fprintf(stderr, "GPR\n0-3\t%d\t%d\t%d\t%d\n4-7\t%d\t%d\t%d\t%d\n8-11\t%d\t%d\t%d\t%d\n12-15\t%d\t%d\t%d\t%d\n16-19\t%d\t%d\t%d\t%d\n20-23\t%d\t%d\t%d\t%d\n24-27\t%d\t%d\t%d\t%d\n28-31\t%d\t%X\t%X\t%d\nFPR FPCC: %s\n0-3\t%f\t%f\t%f\t%f\n4-7\t%f\t%f\t%f\t%f\n8-11\t%f\t%f\t%f\t%f\n12-15\t%f\t%f\t%f\t%f\n16-19\t%f\t%f\t%f\t%f\n20-23\t%f\t%f\t%f\t%f\n24-27\t%f\t%f\t%f\t%f\n28-31\t%f\t%f\t%f\t%f\n\n", GPR[0], GPR[1], GPR[2], GPR[3], GPR[4], GPR[5], GPR[6], GPR[7], GPR[8], GPR[9], GPR[10], GPR[11], GPR[12], GPR[13], GPR[14], GPR[15], GPR[16], GPR[17], GPR[18], GPR[19], GPR[20], GPR[21], GPR[22], GPR[23], GPR[24], GPR[25], GPR[26], GPR[27], GPR[28], GPR[29], GPR[30], GPR[31], FPCC, FPR[0], FPR[1], FPR[2], FPR[3], FPR[4], FPR[5], FPR[6], FPR[7], FPR[8], FPR[9], FPR[10], FPR[11], FPR[12], FPR[13], FPR[14], FPR[15], FPR[16], FPR[17], FPR[18], FPR[19], FPR[20], FPR[21], FPR[22], FPR[23], FPR[24], FPR[25], FPR[26], FPR[27], FPR[28], FPR[29], FPR[30], FPR[31]);
	return;
}

void print_stats(void) {
	fprintf(stderr, "\nadd:\t%llu\naddi:\t%llu\nsub:\t%llu\nand:\t%llu\nandi:\t%llu\nor:\t%llu\nori:\t%llu\nnor:\t%llu\nsll:\t%llu\nsrl:\t%llu\nslt:\t%llu\nslti:\t%llu\nbeq:\t%llu\nbne:\t%llu\nj:\t%llu\njal:\t%llu\njr:\t%llu\njalr:\t%llu\nlw:\t%llu\nlui:\t%llu\nsw:\t%llu\nin:\t%llu\nout:\t%llu\nbt.s:\t%llu\nbf.s:\t%llu\nadd.s:\t%llu\nsub.s:\t%llu\nmul.s:\t%llu\ndiv.s:\t%llu\nmov.s:\t%llu\nneg.s:\t%llu\nabs.s:\t%llu\nsqrt.s:\t%llu\nc.eq.s:\t%llu\nc.lt.s:\t%llu\nc.le.s:\t%llu\nlw.s:\t%llu\nsw.s:\t%llu\nftoi:\t%llu\nitof:\t%llu\n", OP_COUNT[ADD_L], OP_COUNT[ADDI_L], OP_COUNT[SUB_L], OP_COUNT[AND_L], OP_COUNT[ANDI_L], OP_COUNT[OR_L], OP_COUNT[ORI_L], OP_COUNT[NOR_L], OP_COUNT[SLL_L], OP_COUNT[SRL_L], OP_COUNT[SLT_L], OP_COUNT[SLTI_L], OP_COUNT[BEQ_L], OP_COUNT[BNE_L], OP_COUNT[J_L], OP_COUNT[JAL_L], OP_COUNT[JR_L], OP_COUNT[JALR_L], OP_COUNT[LW_L], OP_COUNT[LUI_L], OP_COUNT[SW_L], OP_COUNT[IN_L], OP_COUNT[OUT_L], OP_COUNT[BT_S_L], OP_COUNT[BF_S_L], OP_COUNT[ADD_S_L], OP_COUNT[SUB_S_L], OP_COUNT[MUL_S_L], OP_COUNT[DIV_S_L], OP_COUNT[MOV_S_L], OP_COUNT[NEG_S_L], OP_COUNT[ABS_S_L], OP_COUNT[SQRT_S_L], OP_COUNT[C_EQ_S_L], OP_COUNT[C_LT_S_L], OP_COUNT[C_LE_S_L], OP_COUNT[LW_S_L], OP_COUNT[SW_S_L], OP_COUNT[FTOI_L], OP_COUNT[ITOF_L]);
	return;
}

void print_memory(int addr, int dhf) {
	int line;
	if (dhf == 0) {
		for(line = addr; line < addr + WNSZ; line += 8) {
			fprintf(stderr, "%5X %8d %8d %8d %8d %8d %8d %8d %8d\n", line, 
				DAT[line], DAT[line+1], DAT[line+2], DAT[line+3], 
				DAT[line+4], DAT[line+5], DAT[line+6], DAT[line+7]);
		}
	}
	if (dhf == 1) {
		for(line = addr; line < addr + WNSZ; line += 8) {
			fprintf(stderr, "%5X %8X %8X %8X %8X %8X %8X %8X %8X\n", line, 
				DAT[line], DAT[line+1], DAT[line+2], DAT[line+3], 
				DAT[line+4], DAT[line+5], DAT[line+6], DAT[line+7]);
		}
	}
	if (dhf == 2) {
		for(line = addr; line < addr + WNSZ; line += 8) {
			fprintf(stderr, "%5X %8f %8f %8f %8f %8f %8f %8f %8f\n", line, 
				*((float*)(DAT+line)), *((float*)(DAT+line+1)), *((float*)(DAT+line+2)), *((float*)(DAT+line+3)), 
				*((float*)(DAT+line+4)), *((float*)(DAT+line+5)), *((float*)(DAT+line+6)), *((float*)(DAT+line+7)));
		}
	}
}

int main(int argc, char *argv[]) {
	int lastpc;
	long long repeat;
	int continue_printing = 0;
	//int svstep1, svstep2, stpstep1, stpstep2;
	char buf1[100] = "s\n", buf2[100] = "s\n", *p1, *p2;
	int addr = 0, dhf = 0; // vars for memory print
	int max_func_depth, current_func_depth;
	if (simprepare(argc, argv, &lastpc)) {
		return 1;
	}
	p1 = buf1;
	p2 = buf2;
	repeat = 0;
	COUNTS = 0;
	STOP = 0;
	max_func_depth = 0; current_func_depth = 0;
	for(PC = 0; PC <= lastpc;) {
	//execution_top:
		OP = TEX[PC];
		
		examine_op();

		if (repeat <= 0 || STOP == 1 || BREAKPOINTS.find(PC) != BREAKPOINTS.end()) {
			fflush(stdout);
		print_again:
			fprintf(stderr, "\n%lld\t", COUNTS);
			print_state();
			if (STOP == 1) {
				print_stats();
				fprintf(stderr, "max function depth:\t%d\ncurrent function depth:\t%d\n\n", max_func_depth, current_func_depth);
			}
			while(1) {
				CPSWAP(p1, p2)
				fprintf(stderr, ">");
				fgets(p1, 100, stdin);
				if (p1[0] == '\n')
					CPSWAP(p1, p2)
				if (p1[0] == 's') {
					if (p1[1] == '\n') {
						repeat = 1;
						break;
					} else if (p1[1] == ' ') {
						repeat = (int)strtol(p1+2, NULL, 0);
						break;
					} else fprintf(stderr, "invalid\n");
				} else if (p1[0] == 'p' && p1[1] == '\n') {
					goto print_again;
				} else if (p1[0] == 'p' && p1[1] == 'j') {
					PJ = !PJ;
					fprintf(stderr, "print jump: %s\n", PJ ? "ON" : "OFF");
				} else if (p1[0] == 'p' && p1[1] == 's') {
					print_stats();
					fprintf(stderr, "max function depth:\t%d\ncurrent function depth:\t%d\n\n", max_func_depth, current_func_depth);
				} else if (p1[0] == 'j' && p1[1] == ' ') {
					repeat = (int)strtol(p1+2, NULL, 0) - COUNTS;
					break;
				} else if (p1[0] == 'P') {
					continue_printing = !continue_printing;
					fprintf(stderr, "continue_printing: %s\n", continue_printing ? "ON" : "OFF");
				} else if (p1[0] == 'm') {
					if (p1[1] == ' ') {
						addr = (int)strtol(p1+2, NULL, 0);
						addr = (addr/WNSZ)*WNSZ;
						if (0 <= addr && addr < 0x80000) {
							print_memory(addr, dhf);
						} else {
							fprintf(stderr, "invalid address\n");
						}
					} else if (p1[1] == '\n') {
						print_memory(addr, dhf);
					} else if (p1[1] == 'd') {
						dhf = 0;
					} else if (p1[1] == 'h') {
						dhf = 1;
					} else if (p1[1] == 'f') {
						dhf = 2;
					} else {
						fprintf(stderr, "invalid\n");
					}
				} else if (p1[0] == 'd') {
					if (addr + WNSZ < 0x80000) {
						addr += WNSZ;
						print_memory(addr, dhf);
					} else fprintf(stderr, "cannot go down any more\n");
				} else if (p1[0] == 'u') {
					if (addr - WNSZ >= 0) {
						addr -= WNSZ;
						print_memory(addr, dhf);
					} else fprintf(stderr, "cannot go up any more\n");
				} else if (p1[0] == 'c') {
					repeat = 0x7FFFFFFFFFFFFFFF;
					break;
				} else if (p1[0] == 'b') {
					if (p1[1] == '\n') {
							//print_breakpoints();
						for(set<int>::iterator si = BREAKPOINTS.begin();
											si != BREAKPOINTS.end(); si++) {
							fprintf(stderr, "%d\n", *si);
						}
					} else if (p1[1] == ' ') {
						int i = (int)strtol(p1+2, NULL, 0);
						if (0 <= i && i <= lastpc) {
							BREAKPOINTS.insert(i);
						} else {
							fprintf(stderr, "out of valid range");
						}
					} else if (p1[1] == 'c') {
						BREAKPOINTS.clear();
					} else {
						fprintf(stderr, "invalid\n");
					}
				} else {
					fprintf(stderr, "invalid\n");
				}
			}
		} else if (continue_printing) {
			fprintf(stderr, "\n%lld\t", COUNTS);
			print_state();
		}
		if (STOP == 1) break;
		/*
			print_state();
			command_prompt();
			*/;
		
		
		if (STOP) { // invalid op or exit
			goto endexec;
		}
		
		/* save the current state if needed */
		
		
		
		/* execution */
		PC += 1;
		OP_EX();
		repeat--;
		OP_COUNT[OP_TYPE]++;
		COUNTS++; //increment COUNTS
		switch(OP_TYPE) {
		case JAL_L:
			++current_func_depth;
			if (current_func_depth > max_func_depth) {
				max_func_depth = current_func_depth;
			}
			break;
		case JR_L:
			--current_func_depth;
			break;
		default:
			break;
		}
	}
endexec:
	//print_reg();
	free(DAT);
	free(TEX);
	return 0;
}



int simprepare(int argc, char *argv[], int *lastpc) {
	int fsize, r, rv, fd;
	char *p;
	struct stat statbuf;
	if (argc < 3) {
		fprintf(stderr, "too few arguments\n");
		return 1;
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
	if (fsize%4 != 0) {
		fprintf(stderr, "text file size is not multiples of 4\n");
		return 1;
	}
	*lastpc = fsize/4;
	p = (char *)malloc(fsize+4);
	if (p == NULL) {
		fprintf(stderr, "malloc error\n");
		return 1;
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
	p = (char *)malloc(1024*1024*2);
	if (p == NULL) {
		fprintf(stderr, "malloc error\n");
		return 1;
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

	IFILE = fopen(argv[3], "r");
	if (IFILE == NULL) {
		perror("main");
		return 1;
	}

	GPR[0] = 0;
	GPR[29] = fsize/4;
	GPR[30] = 0x40000;
	return 0;
}

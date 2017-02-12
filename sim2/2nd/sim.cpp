#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<set>
#include"sim.h"
#include"examine_op.h"
#define CPSWAP(X, Y) {char *emvc2m8 = X; X = Y; Y = emvc2m8;}
#define WNSZ 0x100
using namespace std;

Core CORE[N_CORE];

uint32_t *TEX;
FILE *IFILE;

int STOP;

int PJ = 0;
unsigned long long OP_COUNT[OP_TOTAL] = {0};
unsigned long long COUNTS;
unsigned long long COUNTS_BY_MODE[2] = {0};

set<int> BREAKPOINTS;

int PARALLEL = 0;
int PARALLEL_END_PC;
int GC;
int GD;



int simprepare(int, char*[], int*);

void print_state(void) {
	fprintf(stderr, "\n動的命令数:\t%lld\n", COUNTS);
	fprintf(stderr, "mode: %s execution\n", PARALLEL ? "parallel" : "single");
	for(int i=0; i<(PARALLEL ? N_CORE : 1); i++) {
		CORE[i].print_state();
	}
	fprintf(stderr, "GC = %d\tGD = %d\n\n", GC, GD);
}
void Core::print_state(void) {
	fprintf(stderr, "CORE[%d]\t", my_num);
	(this->*OP_PRNT)();
	fprintf(stderr,
		"GPR\n"
		"0-3\t%d\t%d\t%d\t%d\n"
		"4-7\t%d\t%d\t%d\t%d\n"
		"8-11\t%d\t%d\t%d\t%d\n"
		"12-15\t%d\t%d\t%d\t%d\n"
		"16-19\t%d\t%d\t%d\t%d\n"
		"20-23\t%d\t%d\t%d\t%d\n"
		"24-27\t%d\t%d\t%d\t%d\n"
		"28-31\t%d\t%X\t%X\t%d\n"
		"FPR\n"
		"0-3\t%f\t%f\t%f\t%f\n"
		"4-7\t%f\t%f\t%f\t%f\n"
		"8-11\t%f\t%f\t%f\t%f\n"
		"12-15\t%f\t%f\t%f\t%f\n"
		"16-19\t%f\t%f\t%f\t%f\n"
		"20-23\t%f\t%f\t%f\t%f\n"
		"24-27\t%f\t%f\t%f\t%f\n"
		"28-31\t%f\t%f\t%f\t%f\n",
		GPR[0], GPR[1], GPR[2], GPR[3], GPR[4], GPR[5], GPR[6], GPR[7], GPR[8], GPR[9], GPR[10], GPR[11], GPR[12], GPR[13], GPR[14], GPR[15], GPR[16], GPR[17], GPR[18], GPR[19], GPR[20], GPR[21], GPR[22], GPR[23], GPR[24], GPR[25], GPR[26], GPR[27], GPR[28], GPR[29], GPR[30], GPR[31], FPR[0], FPR[1], FPR[2], FPR[3], FPR[4], FPR[5], FPR[6], FPR[7], FPR[8], FPR[9], FPR[10], FPR[11], FPR[12], FPR[13], FPR[14], FPR[15], FPR[16], FPR[17], FPR[18], FPR[19], FPR[20], FPR[21], FPR[22], FPR[23], FPR[24], FPR[25], FPR[26], FPR[27], FPR[28], FPR[29], FPR[30], FPR[31]
	);
}

void print_stats(void) {
	fprintf(stderr, "\n");
	fprintf(stderr, "single mode:\t%llu\n", COUNTS_BY_MODE[0]);
	fprintf(stderr, "parallel mode:\t%llu\n", COUNTS_BY_MODE[1]);
	fprintf(stderr, "add:\t%llu\n", OP_COUNT[ADD_L]);
	fprintf(stderr, "addi:\t%llu\n", OP_COUNT[ADDI_L]);
	fprintf(stderr, "sub:\t%llu\n", OP_COUNT[SUB_L]);
	fprintf(stderr, "next:\t%llu\n", OP_COUNT[NEXT_L]);
	fprintf(stderr, "sl2add:\t%llu\n", OP_COUNT[SL2ADD_L]);
	fprintf(stderr, "sl2addi:\t%llu\n", OP_COUNT[SL2ADDI_L]);
	fprintf(stderr, "mov:\t%llu\n", OP_COUNT[MOV_L]);
	fprintf(stderr, "movi:\t%llu\n", OP_COUNT[MOVI_L]);
	fprintf(stderr, "fadd:\t%llu\n", OP_COUNT[FADD_L]);
	fprintf(stderr, "fsub:\t%llu\n", OP_COUNT[FSUB_L]);
	fprintf(stderr, "fmul:\t%llu\n", OP_COUNT[FMUL_L]);
	fprintf(stderr, "fdiv:\t%llu\n", OP_COUNT[FDIV_L]);
	fprintf(stderr, "fmov:\t%llu\n", OP_COUNT[FMOV_L]);
	fprintf(stderr, "fneg:\t%llu\n", OP_COUNT[FNEG_L]);
	fprintf(stderr, "fabs:\t%llu\n", OP_COUNT[FABS_L]);
	fprintf(stderr, "fsqrt:\t%llu\n", OP_COUNT[FSQRT_L]);
	fprintf(stderr, "lw:\t%llu\n", OP_COUNT[LW_L]);
	fprintf(stderr, "lwi:\t%llu\n", OP_COUNT[LWI_L]);
	fprintf(stderr, "flw:\t%llu\n", OP_COUNT[FLW_L]);
	fprintf(stderr, "flwi:\t%llu\n", OP_COUNT[FLWI_L]);
	fprintf(stderr, "sw:\t%llu\n", OP_COUNT[SW_L]);
	fprintf(stderr, "swi:\t%llu\n", OP_COUNT[SWI_L]);
	fprintf(stderr, "fsw:\t%llu\n", OP_COUNT[FSW_L]);
	fprintf(stderr, "fswi:\t%llu\n", OP_COUNT[FSWI_L]);
	fprintf(stderr, "ftoi:\t%llu\n", OP_COUNT[FTOI_L]);
	fprintf(stderr, "itof:\t%llu\n", OP_COUNT[ITOF_L]);
	fprintf(stderr, "in:\t%llu\n", OP_COUNT[IN_L]);
	fprintf(stderr, "fin:\t%llu\n", OP_COUNT[FIN_L]);
	fprintf(stderr, "out:\t%llu\n", OP_COUNT[OUT_L]);
	fprintf(stderr, "jr:\t%llu\n", OP_COUNT[JR_L]);
	fprintf(stderr, "acc:\t%llu\n", OP_COUNT[ACC_L]);
	fprintf(stderr, "fork:\t%llu\n", OP_COUNT[FORK_L]);
	fprintf(stderr, "end:\t%llu\n", OP_COUNT[END_L]);
	fprintf(stderr, "(ended):\t%llu\n", OP_COUNT[ENDED_L]);
	fprintf(stderr, "j:\t%llu\n", OP_COUNT[J_L]);
	fprintf(stderr, "jal:\t%llu\n", OP_COUNT[JAL_L]);
	fprintf(stderr, "fbz:\t%llu\n", OP_COUNT[FBZ_L]);
	fprintf(stderr, "fble:\t%llu\n", OP_COUNT[FBLE_L]);
	fprintf(stderr, "be:\t%llu\n", OP_COUNT[BE_L]);
	fprintf(stderr, "bei:\t%llu\n", OP_COUNT[BEI_L]);
	fprintf(stderr, "ble:\t%llu\n", OP_COUNT[BLE_L]);
	fprintf(stderr, "blei:\t%llu\n", OP_COUNT[BLEI_L]);
	for(int i=0; i<N_CORE; i++) {
		CORE[i].print_stats();
	}
	fprintf(stderr, "\n");
}
void Core::print_stats(void) {
	fprintf(stderr, "CORE[%d]\nmax function depth:\t%d\ncurrent function depth:\t%d\n", my_num, max_func_depth, current_func_depth);
}

void Core::print_memory(int addr, int dhf) {
	int line;
	fprintf(stderr, "CORE[%d]\n", my_num);
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
	int addr = 0, dhf = 0, m_core = 0; // vars for memory print
	for(int i=0; i<N_CORE; i++) {
		CORE[i].my_num = i;
		CORE[i].ended = 0;
		CORE[i].max_func_depth = 0;
		CORE[i].current_func_depth = 0;
	}
	if (simprepare(argc, argv, &lastpc)) {
		return 1;
	}
	p1 = buf1;
	p2 = buf2;
	repeat = 0;
	COUNTS = 0;
	STOP = 0;
	while(1) {
	//execution_top:
		int breakpoint_hit = 0;
		for(int i=0; i<(PARALLEL ? N_CORE : 1); i++) {
			CORE[i].OP = TEX[CORE[i].PC];
			CORE[i].examine_op();
			if (BREAKPOINTS.find(CORE[i].PC) != BREAKPOINTS.end()) breakpoint_hit = 1;
		}
		if (repeat <= 0 || STOP == 1 || breakpoint_hit) {
			fflush(stdout);
		print_again:
			print_state();
			if (STOP == 1) {
				print_stats();
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
				} else if (p1[0] == 'p' && p1[1] == 'c') {
					if (PARALLEL) {
						fprintf(stderr, "this command cannot be used in the parallel mode\n");
					} else {
						CORE[0].PC = (int)strtol(p1+3, NULL, 0);
						CORE[0].OP = TEX[CORE[0].PC];
						CORE[0].examine_op();
					}
				} else if (p1[0] == 'p' && p1[1] == 'j') {
					PJ = !PJ;
					fprintf(stderr, "print jump: %s\n", PJ ? "ON" : "OFF");
				} else if (p1[0] == 'p' && p1[1] == 's') {
					print_stats();
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
							CORE[m_core].print_memory(addr, dhf);
						} else {
							fprintf(stderr, "invalid address\n");
						}
					} else if (p1[1] == '\n') {
						CORE[m_core].print_memory(addr, dhf);
					} else if (p1[1] == 'd') {
						dhf = 0;
					} else if (p1[1] == 'h') {
						dhf = 1;
					} else if (p1[1] == 'f') {
						dhf = 2;
					} else if (p1[1] == 'c') {
						int i = (int)strtol(p1+3, NULL, 0);
						if (0 <= i && i < N_CORE) {
							m_core = i;
						} else {
							fprintf(stderr, "out of valid range");
						}
					} else {
						fprintf(stderr, "invalid\n");
					}
				} else if (p1[0] == 'd') {
					if (addr + WNSZ < 0x80000) {
						addr += WNSZ;
						CORE[m_core].print_memory(addr, dhf);
					} else fprintf(stderr, "cannot go down any more\n");
				} else if (p1[0] == 'u') {
					if (addr - WNSZ >= 0) {
						addr -= WNSZ;
						CORE[m_core].print_memory(addr, dhf);
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
		COUNTS_BY_MODE[PARALLEL]++;
		int all_ended = 1;
		for(int i=0, i0 = PARALLEL ? N_CORE : 1; i<i0; i++) {
			CORE[i].PC += 1;
			(CORE[i].*CORE[i].OP_EX)();
			OP_COUNT[CORE[i].OP_TYPE]++;
			if (!CORE[i].ended) all_ended = 0;
		}
		if (all_ended) {
			if (PJ) fprintf(stderr, "%8llu all end, return to %5d\n", COUNTS, PARALLEL_END_PC);
			for(int i=0; i<N_CORE; i++) CORE[i].ended = 0;
			PARALLEL = 0;
			CORE[0].PC = PARALLEL_END_PC;
		}
		repeat--;
		COUNTS++; //increment COUNTS
	}
endexec:
	//print_reg();
	free(TEX);
	return 0;
}



int simprepare(int argc, char *argv[], int *lastpc) {
	int fsize, r, rv, fd;
	char *p;
	struct stat statbuf;
	if (argc != 5) {
		fprintf(stderr, "usage: %s text data pc input > output\n", argv[0]);
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
	if (fsize >= DAT_BYTE_SIZE) {
		fprintf(stderr, "data file size is too large\n");
		return 1;
	}
	p = (char *)CORE[0].DAT;
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
	for(int i=1; i<N_CORE; i++) {
		memcpy(CORE[i].DAT, CORE[0].DAT, fsize);
	}

	{
		FILE *pc_file;
		pc_file = fopen(argv[3], "r");
		if (pc_file == NULL) {
			perror(argv[3]);
			return 1;
		}
		if (fscanf(pc_file, "%d", &CORE[0].PC) != 1) {
			fprintf(stderr, "reading pc failed\n");
			perror("fscanf");
			return 1;
		}
		fclose(pc_file);
	}

	IFILE = fopen(argv[4], "r");
	if (IFILE == NULL) {
		perror("main");
		return 1;
	}

	CORE[0].GPR[31] = fsize/4;
	CORE[0].GPR[30] = DAT_WORD_SIZE-1;
	return 0;
}

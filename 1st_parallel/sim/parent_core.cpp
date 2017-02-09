#include"sim.h"
#include"examine_op.h"
#include"prnt_state_stats_memory.h"

#include<stdlib.h>

#define CPSWAP(X, Y) {char *emvc2m8 = X; X = Y; Y = emvc2m8;}
#define WNSZ 0x100


using namespace std;

void* invoke_parent_core(void *vparg) {
	arg ag;
	std::set<int> bp;
	ag.REPEAT = 0;
	ag.COUNTS = 0;
	char s1[100], s2[100] = "s\n";
	char *p1 = s2, *p2 = s1;
	int addr = 0, dhf = 0;
	for(CE[0].PC = 0; CE[0].PC <= PTEX_LAST;) {
		uint32_t op = PTEX[CE[0].PC];
		examine_op_parent(op, ag);
		if (ag.REPEAT <= 0 || ag.STOP == 1 || bp.find(CE[0].PC) != bp.end()) {
			fflush(stdout);
		print_again:
			fprintf(stderr, "\n%lld\t", ag.COUNTS);
			print_state(CE[0], ag);
			/*if (ag.STOP == 1) {
				print_stats();
				fprintf(stderr, "max function depth:\t%d\ncurrent function depth:\t%d\n\n", max_func_depth, current_func_depth);
			}*/
			while(1) {
			CPSWAP(p1, p2)
				fprintf(stderr, ">");
				fgets(p1, 100, stdin);
				if (p1[0] == '\n')
					CPSWAP(p1, p2)
				if (p1[0] == 's') {
					if (p1[1] == '\n') {
						ag.REPEAT = 1;
						break;
					} else if (p1[1] == ' ') {
						ag.REPEAT = (int)strtol(p1+2, NULL, 0);
						break;
					} else fprintf(stderr, "invalid\n");
				} else if (p1[0] == 'p' && p1[1] == '\n') {
					goto print_again;
				} else if (p1[0] == 'p' && p1[1] == 'j') {
					CE[0].PJ = !CE[0].PJ;
					fprintf(stderr, "print jump: %s\n", CE[0].PJ ? "ON" : "OFF");
				} /*else if (p1[0] == 'p' && p1[1] == 's') {
					print_stats();
					fprintf(stderr, "max function depth:\t%d\ncurrent function depth:\t%d\n\n", max_func_depth, current_func_depth);
				}*/ else if (p1[0] == 'j' && p1[1] == ' ') {
					ag.REPEAT = (int)strtol(p1+2, NULL, 0) - ag.COUNTS;
					break;
				}/*else if (p1[0] == 'P') {
					continue_printing = !continue_printing;
					fprintf(stderr, "continue_printing: %s\n", continue_printing ? "ON" : "OFF");
				}*/ else if (p1[0] == 'm') {
					if (p1[1] == ' ') {
						addr = (int)strtol(p1+2, NULL, 0);
						addr = (addr/WNSZ)*WNSZ;
						if (0 <= addr && addr < 0x80000) {
							print_memory(CE[0].DAT, addr, dhf);
						} else {
							fprintf(stderr, "invalid address\n");
						}
					} else if (p1[1] == '\n') {
						print_memory(CE[0].DAT, addr, dhf);
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
						print_memory(CE[0].DAT, addr, dhf);
					} else fprintf(stderr, "cannot go down any more\n");
				} else if (p1[0] == 'u') {
					if (addr - WNSZ >= 0) {
						addr -= WNSZ;
						print_memory(CE[0].DAT, addr, dhf);
					} else fprintf(stderr, "cannot go up any more\n");
				} else if (p1[0] == 'c') {
					ag.REPEAT = 0x7FFFFFFFFFFFFFFF;
					break;
				} else if (p1[0] == 'b') {
					if (p1[1] == '\n') {
							//print_bp();
						for(set<int>::iterator si = bp.begin();
											si != bp.end(); si++) {
							fprintf(stderr, "%d\n", *si);
						}
					} else if (p1[1] == ' ') {
						int i = (int)strtol(p1+2, NULL, 0);
						if (0 <= i && i <= PTEX_LAST) {
							bp.insert(i);
						} else {
							fprintf(stderr, "out of valid range");
						}
					} else if (p1[1] == 'c') {
						bp.clear();
					} else {
						fprintf(stderr, "invalid\n");
					}
				} else {
					fprintf(stderr, "invalid\n");
				}
			}
		}/* else if (continue_printing) {
			fprintf(stderr, "\n%lld\t", counts);
			print_state();
		}*/
		if (ag.STOP == 1) break;
		
		/* execution */
		CE[0].PC += 1;
		ag.OP_EX(CE[0], ag);
		ag.REPEAT--; ag.COUNTS++;
		/*
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
		*/
	}
	return 0;
}

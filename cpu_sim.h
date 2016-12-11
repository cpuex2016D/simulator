#ifndef CPU_SIM_H
#define CPU_SIM_H

#include <set>

enum exec_mode {
	MODE_CONTINUE,
	MODE_STEP,
};

extern uint32_t PC;
extern uint32_t GPR[];
extern float FPR[];
extern uint32_t OP;
extern uint32_t *TEX;
extern uint32_t *DAT;
extern unsigned char FPCC;
extern FILE *IFILE;
extern exec_mode mode;
extern std::set<int> breakpoints;
extern int lastpc;

void print_reg();

#endif

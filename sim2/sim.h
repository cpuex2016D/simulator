#ifndef _SIM_H
#define _SIM_H

#include<stdio.h>
extern int PC;
extern uint32_t GPR[32];
extern char FPCC[9];
extern float FPR[32];
extern uint32_t OP;
extern uint32_t *TEX, *DAT;
extern FILE *IFILE;

extern void (*OP_EX) (void);
extern void (*OP_PRNT) (void);
extern int STOP;

extern int PJ;
enum {ADD_L, ADDI_L, SUB_L, AND_L, ANDI_L, OR_L, ORI_L, NOR_L, SLL_L, SRL_L, SLT_L, SLTI_L, BEQ_L, BNE_L, J_L, JAL_L, JR_L, JALR_L, LW_L, LUI_L, SW_L, IN_L, OUT_L, BT_S_L, BF_S_L, ADD_S_L, SUB_S_L, MUL_S_L, DIV_S_L, MOV_S_L, NEG_S_L, ABS_S_L, SQRT_S_L, C_EQ_S_L, C_LT_S_L, C_LE_S_L, LW_S_L, SW_S_L, FTOI_L, ITOF_L, OP_TOTAL};
extern unsigned long long OP_COUNT[];
extern int OP_TYPE;
extern unsigned long long COUNTS;

#define N_MSBs 10
#define N_LSBs 4
#define MASK_MSBs 0x00003ff0
#define MASK_LSBs 0x0000000f
extern uint32_t GH;
extern char PHT[1<<(N_MSBs+N_LSBs)];
extern unsigned long long BP_COUNT[2];

#endif

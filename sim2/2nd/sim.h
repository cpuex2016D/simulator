#ifndef _SIM_H
#define _SIM_H

#include<stdio.h>
#define N_CORE 4
#define DAT_WORD_SIZE (1 << 17)
#define DAT_BYTE_SIZE (DAT_WORD_SIZE * 4)
#define ADDR_STACK_SIZE 16
class Core {
public:
	int my_num;
	int ended;
	int PC;
	uint32_t GPR[32];
	float FPR[32];
	uint32_t DAT[DAT_WORD_SIZE];

	uint32_t OP;
	int32_t R0, R1, R2, C, C2;
	int OP_TYPE;
	void (Core::*OP_EX) (void);
	void (Core::*OP_PRNT) (void);

	int max_func_depth;
	int current_func_depth;
	int addr_stack[ADDR_STACK_SIZE];

	void print_state(void);
	void print_stats(void);
	void print_memory(int, int);

	void examine_op(void);

	void print_jump(uint32_t, uint32_t);
	void print_end(uint32_t);
	void op_add(void);
	void op_addi(void);
	void op_sub(void);
	void op_next(void);
	void op_sl2add(void);
	void op_sl2addi(void);
	void op_mov(void);
	void op_movi(void);
	void op_fadd(void);
	void op_fsub(void);
	void op_fmul(void);
	void op_fdiv(void);
	void op_fmov(void);
	void op_fneg(void);
	void op_fabs(void);
	void op_fsqrt(void);
	void op_lw(void);
	void op_lwi(void);
	void op_flw(void);
	void op_flwi(void);
	void op_sw(void);
	void op_swi(void);
	void op_fsw(void);
	void op_fswi(void);
	void op_ftoi(void);
	void op_itof(void);
	void op_in(void);
	void op_fin(void);
	void op_out(void);
	void op_jr(void);
	void op_acc(void);
	void op_fork(void);
	void op_end(void);
	void op_j(void);
	void op_jal(void);
	void op_fbz(void);
	void op_fble(void);
	void op_be(void);
	void op_bei(void);
	void op_ble(void);
	void op_blei(void);

	void prnt_add(void);
	void prnt_addi(void);
	void prnt_sub(void);
	void prnt_next(void);
	void prnt_sl2add(void);
	void prnt_sl2addi(void);
	void prnt_mov(void);
	void prnt_movi(void);
	void prnt_fadd(void);
	void prnt_fsub(void);
	void prnt_fmul(void);
	void prnt_fdiv(void);
	void prnt_fmov(void);
	void prnt_fneg(void);
	void prnt_fabs(void);
	void prnt_fsqrt(void);
	void prnt_lw(void);
	void prnt_lwi(void);
	void prnt_flw(void);
	void prnt_flwi(void);
	void prnt_sw(void);
	void prnt_swi(void);
	void prnt_fsw(void);
	void prnt_fswi(void);
	void prnt_ftoi(void);
	void prnt_itof(void);
	void prnt_in(void);
	void prnt_fin(void);
	void prnt_out(void);
	void prnt_jr(void);
	void prnt_acc(void);
	void prnt_fork(void);
	void prnt_end(void);
	void prnt_j(void);
	void prnt_jal(void);
	void prnt_fbz(void);
	void prnt_fble(void);
	void prnt_be(void);
	void prnt_bei(void);
	void prnt_ble(void);
	void prnt_blei(void);
};

extern Core CORE[];

extern uint32_t *TEX;
extern FILE *IFILE;

extern int STOP;

extern int PJ;
enum {ADD_L, ADDI_L, SUB_L, NEXT_L, SL2ADD_L, SL2ADDI_L, MOV_L, MOVI_L, FADD_L, FSUB_L, FMUL_L, FDIV_L, FMOV_L, FNEG_L, FABS_L, FSQRT_L, LW_L, LWI_L, FLW_L, FLWI_L, SW_L, SWI_L, FSW_L, FSWI_L, FTOI_L, ITOF_L, IN_L, FIN_L, OUT_L, JR_L, ACC_L, FORK_L, END_L, J_L, JAL_L, FBZ_L, FBLE_L, BE_L, BEI_L, BLE_L, BLEI_L, OP_TOTAL};
extern unsigned long long OP_COUNT[];
extern int OP_TYPE;
extern unsigned long long COUNTS;

extern int PARALLEL;
extern int PARALLEL_END_PC;
extern int GC;
extern int GD;

#endif

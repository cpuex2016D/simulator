#ifndef _SIM_H
#define _SIM_H

#include<stdio.h>
#define N_CORE 4
#define DAT_BYTE_SIZE 1024*1024*2
#define DAT_WORD_SIZE DAT_BYTE_SIZE/4
class Core {
public:
	int my_num;
	int ended;
	int PC;
	uint32_t GPR[32];
	char FPCC[9];
	float FPR[32];
	uint32_t DAT[DAT_WORD_SIZE];

	uint32_t OP;
	int32_t RD, RS, RT, C, FD, FS, FT, CC;
	int OP_TYPE;
	void (Core::*OP_EX) (void);
	void (Core::*OP_PRNT) (void);

	int max_func_depth;
	int current_func_depth;

	void print_state(void);
	void print_stats(void);
	void print_memory(int, int);
	void update_func_depth(void);

	void examine_op(void);

	void print_jump(uint32_t, uint32_t);
	void print_end(uint32_t);
	void op_add(void);
	void op_addi(void);
	void op_sub(void);
	void op_and(void);
	void op_andi(void);
	void op_or(void);
	void op_ori(void);
	void op_nor(void);
	void op_sll(void);
	void op_srl(void);
	void op_slt(void);
	void op_slti(void);
	void op_beq(void);
	void op_bne(void);
	void op_j(void);
	void op_jal(void);
	void op_jr(void);
	void op_jalr(void);
	void op_lw(void);
	void op_lui(void);
	void op_sw(void);
	void op_in(void);
	void op_out(void);
	void op_bt_s(void);
	void op_bf_s(void);
	void op_add_s(void);
	void op_sub_s(void);
	void op_mul_s(void);
	void op_div_s(void);
	void op_mov_s(void);
	void op_neg_s(void);
	void op_abs_s(void);
	void op_sqrt_s(void);
	void op_c_eq_s(void);
	void op_c_lt_s(void);
	void op_c_le_s(void);
	void op_lw_s(void);
	void op_sw_s(void);
	void op_ftoi(void);
	void op_itof(void);
	void op_fork(void);
	void op_end(void);
	void op_next(void);
	void op_acc(void);

	void prnt_add(void);
	void prnt_addi(void);
	void prnt_sub(void);
	void prnt_and(void);
	void prnt_andi(void);
	void prnt_or(void);
	void prnt_ori(void);
	void prnt_nor(void);
	void prnt_sll(void);
	void prnt_srl(void);
	void prnt_slt(void);
	void prnt_slti(void);
	void prnt_beq(void);
	void prnt_bne(void);
	void prnt_j(void);
	void prnt_jal(void);
	void prnt_jr(void);
	void prnt_jalr(void);
	void prnt_lw(void);
	void prnt_lui(void);
	void prnt_sw(void);
	void prnt_in(void);
	void prnt_out(void);
	void prnt_bt_s(void);
	void prnt_bf_s(void);
	void prnt_add_s(void);
	void prnt_sub_s(void);
	void prnt_mul_s(void);
	void prnt_div_s(void);
	void prnt_mov_s(void);
	void prnt_neg_s(void);
	void prnt_abs_s(void);
	void prnt_sqrt_s(void);
	void prnt_c_eq_s(void);
	void prnt_c_lt_s(void);
	void prnt_c_le_s(void);
	void prnt_lw_s(void);
	void prnt_sw_s(void);
	void prnt_ftoi(void);
	void prnt_itof(void);
	void prnt_fork(void);
	void prnt_end(void);
	void prnt_next(void);
	void prnt_acc(void);
};

extern Core CORE[];

extern uint32_t *TEX;
extern FILE *IFILE;

extern int STOP;

extern int PJ;
enum {ADD_L, ADDI_L, SUB_L, AND_L, ANDI_L, OR_L, ORI_L, NOR_L, SLL_L, SRL_L, SLT_L, SLTI_L, BEQ_L, BNE_L, J_L, JAL_L, JR_L, JALR_L, LW_L, LUI_L, SW_L, IN_L, OUT_L, BT_S_L, BF_S_L, ADD_S_L, SUB_S_L, MUL_S_L, DIV_S_L, MOV_S_L, NEG_S_L, ABS_S_L, SQRT_S_L, C_EQ_S_L, C_LT_S_L, C_LE_S_L, LW_S_L, SW_S_L, FTOI_L, ITOF_L, FORK_L, END_L, NEXT_L, ACC_L, OP_TOTAL};
extern unsigned long long OP_COUNT[];
extern int OP_TYPE;
extern unsigned long long COUNTS;

extern int PARALLEL;
extern int PARALLEL_END_PC;
extern int GC;
extern int GD;

#endif

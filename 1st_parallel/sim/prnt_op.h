#ifndef _PRNT_OP_H
#define _PRNT_OP_H

#include"sim.h"

void prnt_add(arg&);
void prnt_addi(arg&);
void prnt_sub(arg&);
void prnt_and(arg&);
void prnt_andi(arg&);
void prnt_or(arg&);
void prnt_ori(arg&);
void prnt_nor(arg&);
void prnt_sll(arg&);
void prnt_srl(arg&);
void prnt_slt(arg&);
void prnt_slti(arg&);
void prnt_beq(arg&);
void prnt_bne(arg&);
void prnt_j(arg&);
void prnt_jal(arg&);
void prnt_jr(arg&);
void prnt_jalr(arg&);
void prnt_lw(arg&);
void prnt_lui(arg&);
void prnt_sw(arg&);
void prnt_in(arg&);
void prnt_out(arg&);
void prnt_bt_s(arg&);
void prnt_bf_s(arg&);
void prnt_add_s(arg&);
void prnt_sub_s(arg&);
void prnt_mul_s(arg&);
void prnt_div_s(arg&);
void prnt_mov_s(arg&);
void prnt_neg_s(arg&);
void prnt_abs_s(arg&);
void prnt_sqrt_s(arg&);
void prnt_c_eq_s(arg&);
void prnt_c_lt_s(arg&);
void prnt_c_le_s(arg&);
void prnt_lw_s(arg&);
void prnt_sw_s(arg&);
void prnt_ftoi(arg&);
void prnt_itof(arg&);
void prnt_next(arg&);
void prnt_acc(arg&);
void prnt_fork(arg&);
void prnt_end(arg&);

#endif

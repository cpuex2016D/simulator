#ifndef _EXEC_OP_H
#define _EXEC_OP_H

#include"sim.h"

void op_add(coreenv& ce, arg& ag);
void op_addi(coreenv& ce, arg& ag);
void op_sub(coreenv& ce, arg& ag);
void op_and(coreenv& ce, arg& ag);
void op_andi(coreenv& ce, arg& ag);
void op_or(coreenv& ce, arg& ag);
void op_ori(coreenv& ce, arg& ag);
void op_nor(coreenv& ce, arg& ag);
void op_sll(coreenv& ce, arg& ag);
void op_srl(coreenv& ce, arg& ag);
void op_slt(coreenv& ce, arg& ag);
void op_slti(coreenv& ce, arg& ag);
void op_beq(coreenv& ce, arg& ag);
void op_bne(coreenv& ce, arg& ag);
void op_j(coreenv& ce, arg& ag);
void op_jal(coreenv& ce, arg& ag);
void op_jr(coreenv& ce, arg& ag);
void op_jalr(coreenv& ce, arg& ag);
void op_lw(coreenv& ce, arg& ag);
void op_lui(coreenv& ce, arg& ag);
void op_sw_parent(coreenv& ce, arg& ag);
void op_sw_child(coreenv& ce, arg& ag);
void op_in(coreenv& ce, arg& ag);
void op_out(coreenv& ce, arg& ag);
void op_bt_s(coreenv& ce, arg& ag);
void op_bf_s(coreenv& ce, arg& ag);
void op_add_s(coreenv& ce, arg& ag);
void op_sub_s(coreenv& ce, arg& ag);
void op_mul_s(coreenv& ce, arg& ag);
void op_div_s(coreenv& ce, arg& ag);
void op_mov_s(coreenv& ce, arg& ag);
void op_neg_s(coreenv& ce, arg& ag);
void op_abs_s(coreenv& ce, arg& ag);
void op_sqrt_s(coreenv& ce, arg& ag);
void op_c_eq_s(coreenv& ce, arg& ag);
void op_c_lt_s(coreenv& ce, arg& ag);
void op_c_le_s(coreenv& ce, arg& ag);
void op_lw_s(coreenv& ce, arg& ag);
void op_sw_s_parent(coreenv& ce, arg& ag);
void op_sw_s_child(coreenv& ce, arg& ag);
void op_ftoi(coreenv& ce, arg& ag);
void op_itof(coreenv& ce, arg& ag);
void op_next(coreenv& ce, arg& ag);
void op_acc(coreenv& ce, arg& ag);
void op_fork(coreenv& ce, arg& ag);
void op_end(coreenv& ce, arg& ag);

#endif

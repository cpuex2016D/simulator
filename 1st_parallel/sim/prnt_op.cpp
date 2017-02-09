#include<stdio.h>
#include"sim.h"

void prnt_add(arg& ag) {
	fprintf(stderr, "add\t%%r%d, %%r%d, %%r%d\n", ag.RD, ag.RS, ag.RT);
	return;
}
void prnt_addi(arg& ag) {
	fprintf(stderr, "addi\t%%r%d, %%r%d, %d\n", ag.RT, ag.RS, ag.C);
	return;
}
void prnt_sub(arg& ag) {
	fprintf(stderr, "sub\t%%r%d, %%r%d, %%r%d\n", ag.RD, ag.RS, ag.RT);
	return;
}
void prnt_and(arg& ag) {
	fprintf(stderr, "and\t%%r%d, %%r%d, %%r%d\n", ag.RD, ag.RS, ag.RT);
	return;
}
void prnt_andi(arg& ag) {
	fprintf(stderr, "andi\t%%r%d, %%r%d, %d\n", ag.RT, ag.RS, ag.C);
	return;
}
void prnt_or(arg& ag) {
	fprintf(stderr, "or\t%%r%d, %%r%d, %%r%d\n", ag.RD, ag.RS, ag.RT);
	return;
}
void prnt_ori(arg& ag) {
	fprintf(stderr, "ori\t%%r%d, %%r%d, %d\n", ag.RT, ag.RS, ag.C);
	return;
}
void prnt_nor(arg& ag) {
	fprintf(stderr, "nor\t%%r%d, %%r%d, %%r%d\n", ag.RD, ag.RS, ag.RT);
	return;
}
void prnt_sll(arg& ag) {
	fprintf(stderr, "sll\t%%r%d, %%r%d, %d\n", ag.RD, ag.RT, ag.C);
	return;
}
void prnt_srl(arg& ag) {
	fprintf(stderr, "srl\t%%r%d, %%r%d, %d\n", ag.RD, ag.RT, ag.C);
	return;
}
void prnt_slt(arg& ag) {
	fprintf(stderr, "slt\t%%r%d, %%r%d, %%r%d\n", ag.RD, ag.RS, ag.RT);
	return;
}
void prnt_slti(arg& ag) {
	fprintf(stderr, "slti\t%%r%d, %%r%d, %d\n", ag.RT, ag.RS, ag.C);
	return;
}
void prnt_beq(arg& ag) {
	fprintf(stderr, "beq\t%%r%d, %%r%d, %d\n", ag.RS, ag.RT, ag.C);
	return;
}
void prnt_bne(arg& ag) {
	fprintf(stderr, "bne\t%%r%d, %%r%d, %d\n", ag.RS, ag.RT, ag.C);
	return;
}
void prnt_j(arg& ag) {
	fprintf(stderr, "j\t%d\n", ag.C);
	return;
}
void prnt_jal(arg& ag) {
	fprintf(stderr, "jal\t%d\n", ag.C);
	return;
}
void prnt_jr(arg& ag) {
	fprintf(stderr, "jr\t%%r%d\n", ag.RS);
	return;
}
void prnt_jalr(arg& ag) {
	fprintf(stderr, "jalr\t%%r%d\n", ag.RS);
	return;
}
void prnt_lw(arg& ag) {
	fprintf(stderr, "lw\t%%r%d, %d(%%r%d)\n", ag.RT, ag.C, ag.RS);
	return;
}
void prnt_lui(arg& ag) {
	fprintf(stderr, "lui\t%%r%d, %d\n", ag.RT, ag.C);
	return;
}
void prnt_sw(arg& ag) {
	fprintf(stderr, "sw\t%%r%d, %d(%%r%d)\n", ag.RT, ag.C, ag.RS);
	return;
}
void prnt_in(arg& ag) {
	fprintf(stderr, "in\t%%r%d\n", ag.RT);
	return;
}
void prnt_out(arg& ag) {
	fprintf(stderr, "out\t%%r%d\n", ag.RT);
	return;
}
void prnt_bt_s(arg& ag) {
	fprintf(stderr, "bt.s\t%d, %d\n", ag.CC, ag.C);
	return;
}
void prnt_bf_s(arg& ag) {
	fprintf(stderr, "bf.s\t%d, %d\n", ag.CC, ag.C);
	return;
}
void prnt_add_s(arg& ag) {
	fprintf(stderr, "add.s\t%%f%d, %%f%d, %%f%d\n", ag.FD, ag.FS, ag.FT);
	return;
}
void prnt_sub_s(arg& ag) {
	fprintf(stderr, "sub.s\t%%f%d, %%f%d, %%f%d\n", ag.FD, ag.FS, ag.FT);
	return;
}
void prnt_mul_s(arg& ag) {
	fprintf(stderr, "mul.s\t%%f%d, %%f%d, %%f%d\n", ag.FD, ag.FS, ag.FT);
	return;
}
void prnt_div_s(arg& ag) {
	fprintf(stderr, "div.s\t%%f%d, %%f%d, %%f%d\n", ag.FD, ag.FS, ag.FT);
	return;
}
void prnt_mov_s(arg& ag) {
	fprintf(stderr, "mov.s\t%%f%d, %%f%d\n", ag.FD, ag.FT);
	return;
}
void prnt_neg_s(arg& ag) {
	fprintf(stderr, "neg.s\t%%f%d, %%f%d\n", ag.FD, ag.FT);
	return;
}
void prnt_abs_s(arg& ag) {
	fprintf(stderr, "abs.s\t%%f%d, %%f%d\n", ag.FD, ag.FT);
	return;
}
void prnt_sqrt_s(arg& ag) {
	fprintf(stderr, "sqrt.s\t%%f%d, %%f%d\n", ag.FD, ag.FT);
	return;
}
void prnt_c_eq_s(arg& ag) {
	fprintf(stderr, "c.eq.s\t%d, %%f%d, %%f%d\n", ag.CC, ag.FS, ag.FT);
	return;
}
void prnt_c_lt_s(arg& ag) {
	fprintf(stderr, "c.lt.s\t%d, %%f%d, %%f%d\n", ag.CC, ag.FS, ag.FT);
	return;
}
void prnt_c_le_s(arg& ag) {
	fprintf(stderr, "c.le.s\t%d, %%f%d, %%f%d\n", ag.CC, ag.FS, ag.FT);
	return;
}
void prnt_lw_s(arg& ag) {
	fprintf(stderr, "lw.s\t%%f%d, %d(%%r%d)\n", ag.FT, ag.C, ag.RS);
	return;
}
void prnt_sw_s(arg& ag) {
	fprintf(stderr, "sw.s\t%%f%d, %d(%%r%d)\n", ag.FT, ag.C, ag.RS);
	return;
}
void prnt_ftoi(arg& ag) {
	fprintf(stderr, "ftoi\t%%r%d, %%f%d\n", ag.RT, ag.FS);
	return;
}
void prnt_itof(arg& ag) {
	fprintf(stderr, "itof\t%%f%d, %%r%d\n", ag.FT, ag.RS);
	return;
}
void prnt_next(arg& ag) {
	fprintf(stderr, "next\t%%r%d\n", ag.RS);
	return;
}
void prnt_acc(arg& ag) {
	fprintf(stderr, "acc\t%%a%d, %%f%d\n", ag.RS, ag.FT);
	return;
}
void prnt_fork(arg& ag) {
	fprintf(stderr, "fork\t%%r%d, %%r%d\n", ag.RT, ag.RD);
	return;
}
void prnt_end(arg& ag) {
	fprintf(stderr, "end\n");
	return;
}




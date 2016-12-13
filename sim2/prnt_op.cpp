#include<stdio.h>
#include<stdint.h>
extern uint32_t PC;
extern int32_t RD, RS, RT, C, FD, FS, FT, CC;

void prnt_add(void) {
	fprintf(stderr, "%d\tadd\t%%r%d, %%r%d, %%r%d\n", PC, RD, RS, RT);
	return;
}
void prnt_addi(void) {
	fprintf(stderr, "%d\taddi\t%%r%d, %%r%d, %d\n", PC, RT, RS, C);
	return;
}
void prnt_sub(void) {
	fprintf(stderr, "%d\tsub\t%%r%d, %%r%d, %%r%d\n", PC, RD, RS, RT);
	return;
}
void prnt_and(void) {
	fprintf(stderr, "%d\tand\t%%r%d, %%r%d, %%r%d\n", PC, RD, RS, RT);
	return;
}
void prnt_andi(void) {
	fprintf(stderr, "%d\tandi\t%%r%d, %%r%d, %d\n", PC, RT, RS, C);
	return;
}
void prnt_or(void) {
	fprintf(stderr, "%d\tor\t%%r%d, %%r%d, %%r%d\n", PC, RD, RS, RT);
	return;
}
void prnt_ori(void) {
	fprintf(stderr, "%d\tori\t%%r%d, %%r%d, %d\n", PC, RT, RS, C);
	return;
}
void prnt_nor(void) {
	fprintf(stderr, "%d\tnor\t%%r%d, %%r%d, %%r%d\n", PC, RD, RS, RT);
	return;
}
void prnt_sll(void) {
	fprintf(stderr, "%d\tsll\t%%r%d, %%r%d, %d\n", PC, RD, RT, C);
	return;
}
void prnt_srl(void) {
	fprintf(stderr, "%d\tsrl\t%%r%d, %%r%d, %d\n", PC, RD, RT, C);
	return;
}
void prnt_slt(void) {
	fprintf(stderr, "%d\tslt\t%%r%d, %%r%d, %%r%d\n", PC, RD, RS, RT);
	return;
}
void prnt_slti(void) {
	fprintf(stderr, "%d\tslti\t%%r%d, %%r%d, %d\n", PC, RT, RS, C);
	return;
}
void prnt_beq(void) {
	fprintf(stderr, "%d\tbeq\t%%r%d, %%r%d, %d\n", PC, RS, RT, C);
	return;
}
void prnt_bne(void) {
	fprintf(stderr, "%d\tbne\t%%r%d, %%r%d, %d\n", PC, RS, RT, C);
	return;
}
void prnt_j(void) {
	fprintf(stderr, "%d\tj\t%d\n", PC, C);
	return;
}
void prnt_jal(void) {
	fprintf(stderr, "%d\tjal\t%d\n", PC, C);
	return;
}
void prnt_jr(void) {
	fprintf(stderr, "%d\tjr\t%%r%d\n", PC, RS);
	return;
}
void prnt_jalr(void) {
	fprintf(stderr, "%d\tjalr\t%%r%d\n", PC, RS);
	return;
}
void prnt_lw(void) {
	fprintf(stderr, "%d\tlw\t%%r%d, %d(%%r%d)\n", PC, RT, C, RS);
	return;
}
void prnt_lui(void) {
	fprintf(stderr, "%d\tlui\t%%r%d, %d\n", PC, RT, C);
	return;
}
void prnt_sw(void) {
	fprintf(stderr, "%d\tsw\t%%r%d, %d(%%r%d)\n", PC, RT, C, RS);
	return;
}
void prnt_in(void) {
	fprintf(stderr, "%d\tin\t%%r%d\n", PC, RT);
	return;
}
void prnt_out(void) {
	fprintf(stderr, "%d\tout\t%%r%d\n", PC, RT);
	return;
}
void prnt_bt_s(void) {
	fprintf(stderr, "%d\tbt.s\t%d, %d\n", PC, CC, C);
	return;
}
void prnt_bf_s(void) {
	fprintf(stderr, "%d\tbf.s\t%d, %d\n", PC, CC, C);
	return;
}
void prnt_add_s(void) {
	fprintf(stderr, "%d\tadd.s\t%%f%d, %%f%d, %%f%d\n", PC, FD, FS, FT);
	return;
}
void prnt_sub_s(void) {
	fprintf(stderr, "%d\tsub.s\t%%f%d, %%f%d, %%f%d\n", PC, FD, FS, FT);
	return;
}
void prnt_mul_s(void) {
	fprintf(stderr, "%d\tmul.s\t%%f%d, %%f%d, %%f%d\n", PC, FD, FS, FT);
	return;
}
void prnt_div_s(void) {
	fprintf(stderr, "%d\tdiv.s\t%%f%d, %%f%d, %%f%d\n", PC, FD, FS, FT);
	return;
}
void prnt_mov_s(void) {
	fprintf(stderr, "%d\tmov.s\t%%f%d, %%f%d\n", PC, FD, FT);
	return;
}
void prnt_neg_s(void) {
	fprintf(stderr, "%d\tneg.s\t%%f%d, %%f%d\n", PC, FD, FT);
	return;
}
void prnt_abs_s(void) {
	fprintf(stderr, "%d\tabs.s\t%%f%d, %%f%d\n", PC, FD, FT);
	return;
}
void prnt_sqrt_s(void) {
	fprintf(stderr, "%d\tsqrt.s\t%%f%d, %%f%d\n", PC, FD, FT);
	return;
}
void prnt_c_eq_s(void) {
	fprintf(stderr, "%d\tc.eq.s\t%d, %%f%d, %%f%d\n", PC, CC, FS, FT);
	return;
}
void prnt_c_lt_s(void) {
	fprintf(stderr, "%d\tc.lt.s\t%d, %%f%d, %%f%d\n", PC, CC, FS, FT);
	return;
}
void prnt_c_le_s(void) {
	fprintf(stderr, "%d\tc.le.s\t%d, %%f%d, %%f%d\n", PC, CC, FS, FT);
	return;
}
void prnt_lw_s(void) {
	fprintf(stderr, "%d\tlw.s\t%%f%d, %d(%%r%d)\n", PC, FT, C, RS);
	return;
}
void prnt_sw_s(void) {
	fprintf(stderr, "%d\tsw.s\t%%f%d, %d(%%r%d)\n", PC, FT, C, RS);
	return;
}
void prnt_ftoi(void) {
	fprintf(stderr, "%d\tftoi\t%%r%d, %%f%d\n", PC, RT, FS);
	return;
}
void prnt_itof(void) {
	fprintf(stderr, "%d\titof\t%%f%d, %%r%d\n", PC, FT, RS);
	return;
}




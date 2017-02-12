#include<stdio.h>
#include<stdint.h>
#include"sim.h"
#include"examine_op.h"

void Core::prnt_add(void) {
	fprintf(stderr, "%d\tadd\t%%r%d, %%r%d, %%r%d\n", PC, R0, R1, R2);
	return;
}
void Core::prnt_addi(void) {
	fprintf(stderr, "%d\taddi\t%%r%d, %%r%d, %d\n", PC, R0, R1, C);
	return;
}
void Core::prnt_sub(void) {
	fprintf(stderr, "%d\tsub\t%%r%d, %%r%d, %%r%d\n", PC, R0, R1, R2);
	return;
}
void Core::prnt_next(void) {
	fprintf(stderr, "%d\tnext\t%%r%d\n", PC, R0);
	return;
}
void Core::prnt_sl2add(void) {
	fprintf(stderr, "%d\tsl2add\t%%r%d, %%r%d, %%r%d\n", PC, R0, R1, R2);
	return;
}
void Core::prnt_sl2addi(void) {
	fprintf(stderr, "%d\tsl2addi\t%%r%d, %%r%d, %d\n", PC, R0, R1, C);
	return;
}
void Core::prnt_mov(void) {
	fprintf(stderr, "%d\tmov\t%%r%d, %%r%d\n", PC, R0, R1);
	return;
}
void Core::prnt_movi(void) {
	fprintf(stderr, "%d\tmovi\t%%r%d, %d\n", PC, R0, C);
	return;
}
void Core::prnt_fadd(void) {
	fprintf(stderr, "%d\tfadd\t%%f%d, %%f%d, %%f%d\n", PC, R0, R1, R2);
	return;
}
void Core::prnt_fsub(void) {
	fprintf(stderr, "%d\tfsub\t%%f%d, %%f%d, %%f%d\n", PC, R0, R1, R2);
	return;
}
void Core::prnt_fmul(void) {
	fprintf(stderr, "%d\tfmul\t%%f%d, %%f%d, %%f%d\n", PC, R0, R1, R2);
	return;
}
void Core::prnt_fdiv(void) {
	fprintf(stderr, "%d\tfdiv\t%%f%d, %%f%d, %%f%d\n", PC, R0, R1, R2);
	return;
}
void Core::prnt_fmov(void) {
	fprintf(stderr, "%d\tfmov\t%%f%d, %%f%d\n", PC, R0, R1);
	return;
}
void Core::prnt_fneg(void) {
	fprintf(stderr, "%d\tfneg\t%%f%d, %%f%d\n", PC, R0, R1);
	return;
}
void Core::prnt_fabs(void) {
	fprintf(stderr, "%d\tfabs\t%%f%d, %%f%d\n", PC, R0, R1);
	return;
}
void Core::prnt_fsqrt(void) {
	fprintf(stderr, "%d\tfsqrt\t%%f%d, %%f%d\n", PC, R0, R1);
	return;
}
void Core::prnt_lw(void) {
	fprintf(stderr, "%d\tlw\t%%r%d, %d(%%r%d)\n", PC, R0, C, R1);
	return;
}
void Core::prnt_lwi(void) {
	fprintf(stderr, "%d\tlwi\t%%r%d, %d\n", PC, R0, C);
	return;
}
void Core::prnt_flw(void) {
	fprintf(stderr, "%d\tflw\t%%f%d, %d(%%r%d)\n", PC, R0, C, R1);
	return;
}
void Core::prnt_flwi(void) {
	fprintf(stderr, "%d\tflwi\t%%f%d, %d\n", PC, R0, C);
	return;
}
void Core::prnt_sw(void) {
	fprintf(stderr, "%d\tsw\t%%r%d, %d(%%r%d)\n", PC, R2, C, R1);
	return;
}
void Core::prnt_swi(void) {
	fprintf(stderr, "%d\tswi\t%%r%d, %d\n", PC, R2, C);
	return;
}
void Core::prnt_fsw(void) {
	fprintf(stderr, "%d\tfsw\t%%f%d, %d(%%r%d)\n", PC, R2, C, R1);
	return;
}
void Core::prnt_fswi(void) {
	fprintf(stderr, "%d\tfswi\t%%f%d, %d\n", PC, R2, C);
	return;
}
void Core::prnt_ftoi(void) {
	fprintf(stderr, "%d\tftoi\t%%r%d, %%f%d\n", PC, R0, R1);
	return;
}
void Core::prnt_itof(void) {
	fprintf(stderr, "%d\titof\t%%f%d, %%r%d\n", PC, R0, R1);
	return;
}
void Core::prnt_in(void) {
	fprintf(stderr, "%d\tin\t%%r%d\n", PC, R0);
	return;
}
void Core::prnt_fin(void) {
	fprintf(stderr, "%d\tfin\t%%f%d\n", PC, R0);
	return;
}
void Core::prnt_out(void) {
	fprintf(stderr, "%d\tout\t%%r%d\n", PC, R1);
	return;
}
void Core::prnt_jr(void) {
	fprintf(stderr, "%d\tjr\n", PC);
	return;
}
void Core::prnt_acc(void) {
	fprintf(stderr, "%d\tacc\t%%f%d, %%f%d\n", PC, R0, R1);
	return;
}
void Core::prnt_fork(void) {
	fprintf(stderr, "%d\tfork\t%%r%d, %%r%d\n", PC, R1, R2);
	return;
}
void Core::prnt_end(void) {
	fprintf(stderr, "%d\tend\n", PC);
	return;
}
void Core::prnt_ended(void) {
	fprintf(stderr, "%d\t(ended)\n", PC);
	return;
}
void Core::prnt_j(void) {
	fprintf(stderr, "%d\tj\t%d\n", PC, C);
	return;
}
void Core::prnt_jal(void) {
	fprintf(stderr, "%d\tjal\t%d\n", PC, C);
	return;
}
void Core::prnt_fbz(void) {
	fprintf(stderr, "%d\tfbz\t%%f%d, %d\n", PC, R1, C);
	return;
}
void Core::prnt_fble(void) {
	fprintf(stderr, "%d\tfble\t%%f%d, %%f%d, %d\n", PC, R1, R2, C);
	return;
}
void Core::prnt_be(void) {
	fprintf(stderr, "%d\tbe\t%%r%d, %%r%d, %d\n", PC, R1, R2, C);
	return;
}
void Core::prnt_bei(void) {
	fprintf(stderr, "%d\tbei\t%%r%d, %d, %d\n", PC, R1, C2, C);
	return;
}
void Core::prnt_ble(void) {
	fprintf(stderr, "%d\tble\t%%r%d, %%r%d, %d\n", PC, R1, R2, C);
	return;
}
void Core::prnt_blei(void) {
	fprintf(stderr, "%d\tblei\t%%r%d, %d, %d\n", PC, R1, C2, C);
	return;
}




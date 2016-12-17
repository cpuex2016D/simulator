#include<stdio.h>
#include<stdint.h>
#include<math.h>

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
extern int32_t RD, RS, RT, C, FD, FS, FT, CC;

extern int PJ;
extern unsigned long long OPCOUNT;

void print_jump(uint32_t PC_from, uint32_t PC_to) {
	fprintf(stderr, "%8llu jump from %5u to %5u\n", OPCOUNT, PC_from, PC_to);
}

void op_add(void) {
	GPR[RD] = GPR[RS] + GPR[RT];
	return;
}

void op_addi(void) {
	GPR[RT] = GPR[RS] + C;
	return;
}

void op_sub(void) {
	GPR[RD] = GPR[RS] - GPR[RT];
	return;
}

void op_and(void) {
	GPR[RD] = GPR[RS] & GPR[RT];
	return;
}

void op_andi(void) {
	GPR[RT] = GPR[RS] & C;
	return;
}

void op_or(void) {
	GPR[RD] = GPR[RS] | GPR[RT];
	return;
}

void op_ori(void) {
	GPR[RT] = GPR[RS] | C;
	return;
}

void op_nor(void) {
	GPR[RD] = ~(GPR[RS] | GPR[RT]);
	return;
}

void op_sll(void) {
	GPR[RD] = GPR[RT] << C;
	return;
}

void op_srl(void) {
	GPR[RD] = GPR[RT] >> C;
	return;
}

void op_slt(void) {
	GPR[RD] = (int32_t)GPR[RS] <= (int32_t)GPR[RT];
	return;
}

void op_slti(void) {
	GPR[RT] = (int32_t)GPR[RS] <= (int32_t)C;
	return;
}

void op_beq(void) {
	if (GPR[RS] == GPR[RT]) {
		uint32_t PC_from = PC - 1;
		PC += C - 1;
		if (PJ) print_jump(PC_from, PC);
	}
	return;
}

void op_bne(void) {
	if (GPR[RS] != GPR[RT]) {
		uint32_t PC_from = PC - 1;
		PC += C - 1;
		if (PJ) print_jump(PC_from, PC);
	}
	return;
}

void op_j(void) {
	uint32_t PC_from = PC - 1;
	PC = C;
	if (PJ) print_jump(PC_from, PC);
	return;
}

void op_jal(void) {
	uint32_t PC_from = PC - 1;
	GPR[31] = PC;
	PC = C;
	if (PJ) print_jump(PC_from, PC);
	return;
}

void op_jr(void) {
	uint32_t PC_from = PC - 1;
	PC = GPR[RS];
	if (PJ) print_jump(PC_from, PC);
	return;
}

void op_jalr(void) {
	uint32_t PC_from = PC - 1;
	int temp;
	temp = PC;
	PC = GPR[RS];
	GPR[31] = temp;
	if (PJ) print_jump(PC_from, PC);
	return;
}

void op_lw(void) {
	int addr;
	addr = GPR[RS]+C;
	if (!(0 <= addr && addr < 0x80000)) {
		fprintf(stderr, "overflow: trying to lw from %X\n", addr);
		STOP = 1;
		return;
	}
	GPR[RT] = DAT[addr];
	return;
}

void op_lui(void) {
	GPR[RT] = ((uint32_t)C) << 16;
	return;
}

void op_sw(void) {
	int addr;
	addr = GPR[RS]+C;
	if (!(0 <= addr && addr < 0x80000)) {
		fprintf(stderr, "overflow: trying to sw to %X\n", addr);
		STOP = 1;
		return;
	}
	DAT[addr] = GPR[RT];
	return;
}

void op_in(void) {
	int inp;
	inp = getc(IFILE);
	if (inp == EOF) {
		STOP = 1;
		return;
	}
	*((unsigned char*)&GPR[RT]) = (unsigned char) inp;
	return;
}

void op_out(void) {
	fwrite((char*)&GPR[RT], 1, 1, stdout);
	return;
}

void op_bt_s(void) {
	if (FPCC[CC] == '1') {
		uint32_t PC_from = PC - 1;
		PC += C - 1;
		if (PJ) print_jump(PC_from, PC);
	}
	return;
}

void op_bf_s(void) {
	if (FPCC[CC] == '0') {
		uint32_t PC_from = PC - 1;
		PC += C - 1;
		if (PJ) print_jump(PC_from, PC);
	}
	return;
}


void op_add_s(void) {
	FPR[FD] = FPR[FS] + FPR[FT];
	return;
}

void op_sub_s(void) {
	FPR[FD] = FPR[FS] - FPR[FT];
	return;
}

void op_mul_s(void) {
	FPR[FD] = FPR[FS] * FPR[FT];
	return;
}

void op_div_s(void) {
	FPR[FD] = FPR[FS] / FPR[FT];
	return;
}

void op_mov_s(void) {
	FPR[FD] = FPR[FT];
	return;
}

void op_neg_s(void) {
	FPR[FD] = -FPR[FT];
	return;
}

void op_abs_s(void) {
	FPR[FD] = (FPR[FT] < 0) ? -FPR[FT] : FPR[FT];
	return;
}

void op_sqrt_s(void) {
	FPR[FD] = (float)sqrt((double)FPR[FT]);
	return;
}
void op_c_eq_s(void) {
	FPCC[CC] = FPR[FS] == FPR[FT] ? '1' : '0';
	return;
}

void op_c_lt_s(void) {
	FPCC[CC] = FPR[FS] < FPR[FT] ? '1' : '0';
	return;
}

void op_c_le_s(void) {
	FPCC[CC] = FPR[FS] <= FPR[FT] ? '1' : '0';
	return;
}

void op_lw_s(void) {
	int addr;
	addr = GPR[RS]+C;
	if (!(0 <= addr && addr < 0x80000)) {
		fprintf(stderr, "overflow: trying to lw_s from %X\n", addr);
		STOP = 1;
		return;
	}
	FPR[FT] = *((float*)&DAT[addr]);
	return;
}

void op_sw_s(void) {
	int addr;
	addr = GPR[RS]+C;
	if (!(0 <= addr && addr < 0x80000)) {
		fprintf(stderr, "overflow: trying to sw_s to %X\n", addr);
		STOP = 1;
		return;
	}
	*((float*)&DAT[addr]) = FPR[FT];
	return;
}

void op_ftoi(void) {
	GPR[RT] = lrintf(FPR[FS]);
	return;
}

void op_itof(void) {
	FPR[FT] = (float) ((int32_t)GPR[RS]);
	return;
}


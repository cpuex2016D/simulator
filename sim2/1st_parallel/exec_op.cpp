#include<stdio.h>
#include<stdint.h>
#include<math.h>
#include<string.h>
#include"sim.h"
#include"examine_op.h"

void Core::print_jump(uint32_t PC_from, uint32_t PC_to) {
	if (!PARALLEL) {
		fprintf(stderr, "%8llu jump from %5u to %5u\n", COUNTS, PC_from, PC_to);
	} else {
		fprintf(stderr, "%8llu CORE[%d] jump from %5u to %5u\n", COUNTS, my_num, PC_from, PC_to);
	}
}
void print_fork(uint32_t PC_at) {
	fprintf(stderr, "%8llu fork at %5u\n", COUNTS, PC_at);
}
void Core::print_end(uint32_t PC_at) {
	fprintf(stderr, "%8llu CORE[%d] end at %5u\n", COUNTS, my_num, PC_at);
}

void Core::op_add(void) {
	GPR[RD] = GPR[RS] + GPR[RT];
	return;
}

void Core::op_addi(void) {
	GPR[RT] = GPR[RS] + C;
	return;
}

void Core::op_sub(void) {
	GPR[RD] = GPR[RS] - GPR[RT];
	return;
}

void Core::op_and(void) {
	GPR[RD] = GPR[RS] & GPR[RT];
	return;
}

void Core::op_andi(void) {
	GPR[RT] = GPR[RS] & C;
	return;
}

void Core::op_or(void) {
	GPR[RD] = GPR[RS] | GPR[RT];
	return;
}

void Core::op_ori(void) {
	GPR[RT] = GPR[RS] | C;
	return;
}

void Core::op_nor(void) {
	GPR[RD] = ~(GPR[RS] | GPR[RT]);
	return;
}

void Core::op_sll(void) {
	GPR[RD] = GPR[RT] << C;
	return;
}

void Core::op_srl(void) {
	GPR[RD] = GPR[RT] >> C;
	return;
}

void Core::op_slt(void) {
	GPR[RD] = (int32_t)GPR[RS] <= (int32_t)GPR[RT];
	return;
}

void Core::op_slti(void) {
	GPR[RT] = (int32_t)GPR[RS] <= (int32_t)C;
	return;
}

void Core::op_beq(void) {
	if (GPR[RS] == GPR[RT]) {
		uint32_t PC_from = PC - 1;
		PC += C - 1;
		if (PJ) print_jump(PC_from, PC);
	}
	return;
}

void Core::op_bne(void) {
	if (GPR[RS] != GPR[RT]) {
		uint32_t PC_from = PC - 1;
		PC += C - 1;
		if (PJ) print_jump(PC_from, PC);
	}
	return;
}

void Core::op_j(void) {
	uint32_t PC_from = PC - 1;
	PC = C;
	if (PJ) print_jump(PC_from, PC);
	return;
}

void Core::op_jal(void) {
	uint32_t PC_from = PC - 1;
	GPR[31] = PC;
	PC = C;
	if (PJ) print_jump(PC_from, PC);
	return;
}

void Core::op_jr(void) {
	uint32_t PC_from = PC - 1;
	PC = GPR[RS];
	if (PJ) print_jump(PC_from, PC);
	return;
}

void Core::op_jalr(void) {
	uint32_t PC_from = PC - 1;
	int temp;
	temp = PC;
	PC = GPR[RS];
	GPR[31] = temp;
	if (PJ) print_jump(PC_from, PC);
	return;
}

void Core::op_lw(void) {
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

void Core::op_lui(void) {
	GPR[RT] = ((uint32_t)C) << 16;
	return;
}

void Core::op_sw(void) {
	int addr;
	addr = GPR[RS]+C;
	if (!(0 <= addr && addr < 0x80000)) {
		fprintf(stderr, "overflow: trying to sw to %X\n", addr);
		STOP = 1;
		return;
	}
	if (PARALLEL) {
		DAT[addr] = GPR[RT];
	} else {
		for(int i=0; i<N_CORE; i++) {
			CORE[i].DAT[addr] = GPR[RT];
		}
	}
	return;
}

void Core::op_in(void) {
	int inp;
	inp = getc(IFILE);
	if (inp == EOF) {
		STOP = 1;
		return;
	}
	*((unsigned char*)&GPR[RT]) = (unsigned char) inp;
	return;
}

void Core::op_out(void) {
	fwrite((char*)&GPR[RT], 1, 1, stdout);
	return;
}

void Core::op_bt_s(void) {
	if (FPCC[CC] == '1') {
		uint32_t PC_from = PC - 1;
		PC += C - 1;
		if (PJ) print_jump(PC_from, PC);
	}
	return;
}

void Core::op_bf_s(void) {
	if (FPCC[CC] == '0') {
		uint32_t PC_from = PC - 1;
		PC += C - 1;
		if (PJ) print_jump(PC_from, PC);
	}
	return;
}


void Core::op_add_s(void) {
	FPR[FD] = FPR[FS] + FPR[FT];
	return;
}

void Core::op_sub_s(void) {
	FPR[FD] = FPR[FS] - FPR[FT];
	return;
}

void Core::op_mul_s(void) {
	FPR[FD] = FPR[FS] * FPR[FT];
	return;
}

void Core::op_div_s(void) {
	FPR[FD] = FPR[FS] / FPR[FT];
	return;
}

void Core::op_mov_s(void) {
	FPR[FD] = FPR[FT];
	return;
}

void Core::op_neg_s(void) {
	FPR[FD] = -FPR[FT];
	return;
}

void Core::op_abs_s(void) {
	FPR[FD] = (FPR[FT] < 0) ? -FPR[FT] : FPR[FT];
	return;
}

void Core::op_sqrt_s(void) {
	FPR[FD] = (float)sqrt((double)FPR[FT]);
	return;
}
void Core::op_c_eq_s(void) {
	FPCC[CC] = FPR[FS] == FPR[FT] ? '1' : '0';
	return;
}

void Core::op_c_lt_s(void) {
	FPCC[CC] = FPR[FS] < FPR[FT] ? '1' : '0';
	return;
}

void Core::op_c_le_s(void) {
	FPCC[CC] = FPR[FS] <= FPR[FT] ? '1' : '0';
	return;
}

void Core::op_lw_s(void) {
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

void Core::op_sw_s(void) {
	int addr;
	addr = GPR[RS]+C;
	if (!(0 <= addr && addr < 0x80000)) {
		fprintf(stderr, "overflow: trying to sw_s to %X\n", addr);
		STOP = 1;
		return;
	}
	if (PARALLEL) {
		*((float*)&DAT[addr]) = FPR[FT];
	} else {
		for(int i=0; i<N_CORE; i++) {
			*((float*)&CORE[i].DAT[addr]) = FPR[FT];
		}
	}
	return;
}

void Core::op_ftoi(void) {
	GPR[RT] = lrintf(FPR[FS]);
	return;
}

void Core::op_itof(void) {
	FPR[FT] = (float) ((int32_t)GPR[RS]);
	return;
}

void Core::op_fork(void) {
	PARALLEL = 1;
	PARALLEL_END_PC = PC;
	GC = GPR[RT];
	GD = GPR[RD];
	if (PJ) print_fork(PC-1);
	for(int i=0; i<N_CORE; i++) {
		CORE[i].PC = 0;
	}
	for(int i=1; i<N_CORE; i++) {
		memcpy(CORE[i].GPR, CORE[0].GPR, 4*32);
		memcpy(CORE[i].FPR, CORE[0].FPR, 4*32);
		memcpy(CORE[i].FPCC, CORE[0].FPCC, 9);
	}
	return;
}

void Core::op_end(void) {
	PC = PC - 1;
	if (!ended) {
		ended = 1;
		if (PJ) print_end(PC-1);
	}
	return;
}

void Core::op_next(void) {
	GPR[RS] = GC;
	GC += GD;
	return;
}

void Core::op_acc(void) {
	if (!(29<=RS && RS<=31)) {
		fprintf(stderr, "acc: 1st operand must be between %%f29 to %%f31, but %%f%d was specified\n", RS);
		STOP = 1;
		return;
	}
	if (!(0<=RT && RT<=28)) {
		fprintf(stderr, "acc: 2nd operand must be between %%f0 to %%f28, but %%f%d was specified\n", RT);
		STOP = 1;
		return;
	}
	CORE[0].FPR[RS] += FPR[RT];
	return;
}


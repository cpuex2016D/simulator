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
	GPR[R0] = GPR[R1] + GPR[R2];
	return;
}

void Core::op_addi(void) {
	GPR[R0] = GPR[R1] + C;
	return;
}

void Core::op_sub(void) {
	GPR[R0] = GPR[R1] - GPR[R2];
	return;
}

void Core::op_next(void) {
	GPR[R0] = GC;
	GC += GD;
	return;
}

void Core::op_sl2add(void) {
	GPR[R0] = (GPR[R1] << 2) + GPR[R2];
	return;
}

void Core::op_sl2addi(void) {
	GPR[R0] = (GPR[R1] << 2) + C;
	return;
}

void Core::op_mov(void) {
	GPR[R0] = GPR[R1];
	return;
}

void Core::op_movi(void) {
	GPR[R0] = C;
	return;
}

void Core::op_fadd(void) {
	FPR[R0] = FPR[R1] + FPR[R2];
	return;
}

void Core::op_fsub(void) {
	FPR[R0] = FPR[R1] - FPR[R2];
	return;
}

void Core::op_fmul(void) {
	FPR[R0] = FPR[R1] * FPR[R2];
	return;
}

void Core::op_fdiv(void) {
	FPR[R0] = FPR[R1] / FPR[R2];
	return;
}

void Core::op_fmov(void) {
	FPR[R0] = FPR[R1];
	return;
}

void Core::op_fneg(void) {
	FPR[R0] = -FPR[R1];
	return;
}

void Core::op_fabs(void) {
	FPR[R0] = (FPR[R1] < 0) ? -FPR[R1] : FPR[R1];
	return;
}

void Core::op_fsqrt(void) {
	FPR[R0] = (float)sqrt((double)FPR[R1]);
	return;
}



void Core::op_lw(void) {
	int addr;
	addr = GPR[R1]+C;
	if (!(0 <= addr && addr < DAT_WORD_SIZE)) {
		fprintf(stderr, "overflow: trying to lw from %X\n", addr);
		STOP = 1;
		return;
	}
	GPR[R0] = DAT[addr];
	return;
}

void Core::op_lwi(void) {
	GPR[R0] = DAT[C];
	return;
}

void Core::op_flw(void) {
	int addr;
	addr = GPR[R1]+C;
	if (!(0 <= addr && addr < DAT_WORD_SIZE)) {
		fprintf(stderr, "overflow: trying to flw from %X\n", addr);
		STOP = 1;
		return;
	}
	FPR[R0] = *((float*)&DAT[addr]);
	return;
}

void Core::op_flwi(void) {
	FPR[R0] = *((float*)&DAT[C]);
	return;
}

void Core::op_sw(void) {
	int addr;
	addr = GPR[R1]+C;
	if (!(0 <= addr && addr < DAT_WORD_SIZE)) {
		fprintf(stderr, "overflow: trying to sw to %X\n", addr);
		STOP = 1;
		return;
	}
	if (PARALLEL) {
		DAT[addr] = GPR[R2];
	} else {
		for(int i=0; i<N_CORE; i++) {
			CORE[i].DAT[addr] = GPR[R2];
		}
	}
	return;
}

void Core::op_swi(void) {
	if (PARALLEL) {
		DAT[C] = GPR[R2];
	} else {
		for(int i=0; i<N_CORE; i++) {
			CORE[i].DAT[C] = GPR[R2];
		}
	}
	return;
}

void Core::op_fsw(void) {
	int addr;
	addr = GPR[R1]+C;
	if (!(0 <= addr && addr < DAT_WORD_SIZE)) {
		fprintf(stderr, "overflow: trying to fsw to %X\n", addr);
		STOP = 1;
		return;
	}
	if (PARALLEL) {
		*((float*)&DAT[addr]) = FPR[R2];
	} else {
		for(int i=0; i<N_CORE; i++) {
			*((float*)&CORE[i].DAT[addr]) = FPR[R2];
		}
	}
	return;
}

void Core::op_fswi(void) {
	if (PARALLEL) {
		*((float*)&DAT[C]) = FPR[R2];
	} else {
		for(int i=0; i<N_CORE; i++) {
			*((float*)&CORE[i].DAT[C]) = FPR[R2];
		}
	}
	return;
}



void Core::op_ftoi(void) {
	GPR[R0] = lrintf(FPR[R1]);
	return;
}

void Core::op_itof(void) {
	FPR[R0] = (float) ((int32_t)GPR[R1]);
	return;
}

void Core::op_in(void) {
	int ret;
	ret = fread(GPR + R0, 4, 1, IFILE);
	if (!ret) {
		if (feof(IFILE)) {
			fprintf(stderr, "input file reached the last\n");
		} else {
			fprintf(stderr, "op_in: fread: some error has occurred\n");
		}
		STOP = 1;
	}
	return;
}

void Core::op_fin(void) {
	int ret;
	ret = fread(FPR + R0, 4, 1, IFILE);
	if (!ret) {
		if (feof(IFILE)) {
			fprintf(stderr, "input file reached the last\n");
		} else {
			fprintf(stderr, "op_in: fread: some error has occurred\n");
		}
		STOP = 1;
	}
	return;
}

void Core::op_out(void) {
	fwrite((char*)&GPR[R1], 1, 1, stdout);
	return;
}

void Core::op_jr(void) {
	if (current_func_depth == 0) {
		fprintf(stderr, "CORE[%d] jr: return address stack is empty\n", my_num);
		STOP = 1;
		return;
	}
	uint32_t PC_from = PC - 1;
	current_func_depth--;
	PC = addr_stack[current_func_depth];
	if (PJ) print_jump(PC_from, PC);
	return;
}

void Core::op_acc(void) {
	if (!(29<=R0 && R0<=31)) {
		fprintf(stderr, "acc: 1st operand must be between %%f29 to %%f31, but %%f%d was specified\n", R0);
		STOP = 1;
		return;
	}
	if (!(0<=R1 && R1<=28)) {
		fprintf(stderr, "acc: 2nd operand must be between %%f0 to %%f28, but %%f%d was specified\n", R1);
		STOP = 1;
		return;
	}
	CORE[0].FPR[R0] += FPR[R1];
	return;
}

void Core::op_fork(void) {
	PARALLEL = 1;
	PARALLEL_END_PC = PC;
	GC = GPR[R1];
	GD = GPR[R2];
	if (PJ) print_fork(PC-1);
	for(int i=0; i<N_CORE; i++) {
		CORE[i].PC = 0;
	}
	for(int i=1; i<N_CORE; i++) {
		memcpy(CORE[i].GPR, CORE[0].GPR, 4*32);
		memcpy(CORE[i].FPR, CORE[0].FPR, 4*32);
	}
	return;
}

void Core::op_end(void) {
	PC--;
	ended = 1;
	if (PJ) print_end(PC);
	return;
}

void Core::op_ended(void) {
	PC--;
	return;
}

void Core::op_j(void) {
	uint32_t PC_from = PC - 1;
	PC = C;
	if (PJ) print_jump(PC_from, PC);
	return;
}

void Core::op_jal(void) {
	if (current_func_depth == ADDR_STACK_SIZE) {
		fprintf(stderr, "CORE[%d] jal: return address stack overflow\n", my_num);
		STOP = 1;
		return;
	}
	addr_stack[current_func_depth] = PC;
	current_func_depth++;
	if (current_func_depth > max_func_depth) {
		max_func_depth = current_func_depth;
	}
	uint32_t PC_from = PC - 1;
	PC = C;
	if (PJ) print_jump(PC_from, PC);
	return;
}

void Core::op_fbz(void) {
	if (!(*((uint32_t*)&FPR[R1]) & 0x7fe00000)) {
		uint32_t PC_from = PC - 1;
		PC = C;
		if (PJ) print_jump(PC_from, PC);
	}
	return;
}

void Core::op_fble(void) {
	if (FPR[R1] <= FPR[R2]) {
		uint32_t PC_from = PC - 1;
		PC = C;
		if (PJ) print_jump(PC_from, PC);
	}
	return;
}

void Core::op_be(void) {
	if ((int32_t)GPR[R1] == (int32_t)GPR[R2]) {
		uint32_t PC_from = PC - 1;
		PC = C;
		if (PJ) print_jump(PC_from, PC);
	}
	return;
}

void Core::op_bei(void) {
	if ((int32_t)GPR[R1] == (int32_t)C2) {
		uint32_t PC_from = PC - 1;
		PC = C;
		if (PJ) print_jump(PC_from, PC);
	}
	return;
}

void Core::op_ble(void) {
	if ((int32_t)GPR[R1] <= (int32_t)GPR[R2]) {
		uint32_t PC_from = PC - 1;
		PC = C;
		if (PJ) print_jump(PC_from, PC);
	}
	return;
}

void Core::op_blei(void) {
	if ((int32_t)GPR[R1] <= (int32_t)C2) {
		uint32_t PC_from = PC - 1;
		PC = C;
		if (PJ) print_jump(PC_from, PC);
	}
	return;
}


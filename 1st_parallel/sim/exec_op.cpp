#include<stdio.h>
#include<stdint.h>
#include<math.h>
#include<pthread.h>
#include"sim.h"
#include"examine_op.h"
#include"types.h"
#include"child_core.h"

void print_jump(uint32_t PC_from, uint32_t PC_to, unsigned long long int counts) {
	fprintf(stderr, "%8llu jump from %5u to %5u\n", counts, PC_from, PC_to);
}
/*
void branch_prediction(uint32_t PC, int taken) {
	uint32_t index = GH ^ PC;
	int prediction = PHT[index] >= 2;
	BP_COUNT[prediction == taken]++;
	if ( taken && PHT[index]!=3) PHT[index]++;
	if (!taken && PHT[index]!=0) PHT[index]--;
	GH = ((GH << 1) & MASK_MSBs) | (taken << N_LSBs);
}
*/
void op_add(coreenv& ce, arg& ag) {
	ce.GPR[ag.RD] = ce.GPR[ag.RS] + ce.GPR[ag.RT];
	return;
}

void op_addi(coreenv& ce, arg& ag) {
	ce.GPR[ag.RT] = ce.GPR[ag.RS] + ag.C;
	return;
}

void op_sub(coreenv& ce, arg& ag) {
	ce.GPR[ag.RD] = ce.GPR[ag.RS] - ce.GPR[ag.RT];
	return;
}

void op_and(coreenv& ce, arg& ag) {
	ce.GPR[ag.RD] = ce.GPR[ag.RS] & ce.GPR[ag.RT];
	return;
}

void op_andi(coreenv& ce, arg& ag) {
	ce.GPR[ag.RT] = ce.GPR[ag.RS] & ag.C;
	return;
}

void op_or(coreenv& ce, arg& ag) {
	ce.GPR[ag.RD] = ce.GPR[ag.RS] | ce.GPR[ag.RT];
	return;
}

void op_ori(coreenv& ce, arg& ag) {
	ce.GPR[ag.RT] = ce.GPR[ag.RS] | ag.C;
	return;
}

void op_nor(coreenv& ce, arg& ag) {
	ce.GPR[ag.RD] = ~(ce.GPR[ag.RS] | ce.GPR[ag.RT]);
	return;
}

void op_sll(coreenv& ce, arg& ag) {
	ce.GPR[ag.RD] = ce.GPR[ag.RT] << ag.C;
	return;
}

void op_srl(coreenv& ce, arg& ag) {
	ce.GPR[ag.RD] = ce.GPR[ag.RT] >> ag.C;
	return;
}

void op_slt(coreenv& ce, arg& ag) {
	ce.GPR[ag.RD] = (int32_t)ce.GPR[ag.RS] <= (int32_t)ce.GPR[ag.RT];
	return;
}

void op_slti(coreenv& ce, arg& ag) {
	ce.GPR[ag.RT] = (int32_t)ce.GPR[ag.RS] <= (int32_t)ag.C;
	return;
}

void op_beq(coreenv& ce, arg& ag) {
	uint32_t PC_from = ce.PC - 1;
	int taken = ce.GPR[ag.RS] == ce.GPR[ag.RT];
	//branch_prediction(PC_from, taken);
	if (taken) {
		ce.PC += ag.C - 1;
		if (ce.PJ) print_jump(PC_from, ce.PC, ag.COUNTS);
	}
	return;
}

void op_bne(coreenv& ce, arg& ag) {
	uint32_t PC_from = ce.PC - 1;
	int taken = ce.GPR[ag.RS] != ce.GPR[ag.RT];
	//branch_prediction(PC_from, taken);
	if (taken) {
		ce.PC += ag.C - 1;
		if (ce.PJ) print_jump(PC_from, ce.PC, ag.COUNTS);
	}
	return;
}

void op_j(coreenv& ce, arg& ag) {
	uint32_t PC_from = ce.PC - 1;
	ce.PC = ag.C;
	if (ce.PJ) print_jump(PC_from, ce.PC, ag.COUNTS);
	return;
}

void op_jal(coreenv& ce, arg& ag) {
	uint32_t PC_from = ce.PC - 1;
	ce.GPR[31] = ce.PC;
	ce.PC = ag.C;
	if (ce.PJ) print_jump(PC_from, ce.PC, ag.COUNTS);
	return;
}

void op_jr(coreenv& ce, arg& ag) {
	uint32_t PC_from = ce.PC - 1;
	ce.PC = ce.GPR[ag.RS];
	if (ce.PJ) print_jump(PC_from, ce.PC, ag.COUNTS);
	return;
}

void op_jalr(coreenv& ce, arg& ag) {
	uint32_t PC_from = ce.PC - 1;
	int temp;
	temp = ce.PC;
	ce.PC = ce.GPR[ag.RS];
	ce.GPR[31] = temp;
	if (ce.PJ) print_jump(PC_from, ce.PC, ag.COUNTS);
	return;
}

void op_lw(coreenv& ce, arg& ag) {
	int addr;
	addr = ce.GPR[ag.RS] + ag.C;
	if (!(0 <= addr && addr < 0x80000)) {
		fprintf(stderr, "overflow: trying to lw from %X\n", addr);
		ag.STOP = 1;
		return;
	}
	ce.GPR[ag.RT] = ce.DAT[addr];
	return;
}

void op_lui(coreenv& ce, arg& ag) {
	ce.GPR[ag.RT] = ((uint32_t)ag.C) << 16;
	return;
}

void op_sw_parent(coreenv& ce, arg& ag) {
	int addr;
	addr = CE[0].GPR[ag.RS] + ag.C;
	if (!(0 <= addr && addr < 0x80000)) {
		fprintf(stderr, "overflow: trying to sw to %X\n", addr);
		ag.STOP = 1;
		return;
	}
	for(int i = 0; i < CORE_NUM; i++) {
		CE[i].DAT[addr] = CE[0].GPR[ag.RT];
	}
	return;
}

void op_sw_child(coreenv& ce, arg& ag) {
	int addr;
	addr = ce.GPR[ag.RS] + ag.C;
	if (!(0 <= addr && addr < 0x80000)) {
		fprintf(stderr, "overflow: trying to sw to %X\n", addr);
		ag.STOP = 1;
		return;
	}
	ce.DAT[addr] = ce.GPR[ag.RT];
	return;
}

void op_in(coreenv& ce, arg& ag) {
	int inp;
	inp = getc(IFILE);
	if (inp == EOF) {
		fprintf(stderr, "input file reached the last");
		ag.STOP = 1;
		return;
	}
	*((unsigned char*)&ce.GPR[ag.RT]) = (unsigned char) inp;
	return;
}

void op_out(coreenv& ce, arg& ag) {
	fwrite((char*)&ce.GPR[ag.RT], 1, 1, stdout);
	return;
}

void op_bt_s(coreenv& ce, arg& ag) {
	uint32_t PC_from = ce.PC - 1;
	int taken = ce.FPCC[ag.CC] == '1';
	//branch_prediction(PC_from, taken);
	if (taken) {
		ce.PC += ag.C - 1;
		if (ce.PJ) print_jump(PC_from, ce.PC, ag.COUNTS);
	}
	return;
}

void op_bf_s(coreenv& ce, arg& ag) {
	uint32_t PC_from = ce.PC - 1;
	int taken = ce.FPCC[ag.CC] == '0';
	//branch_prediction(PC_from, taken);
	if (taken) {
		ce.PC += ag.C - 1;
		if (ce.PJ) print_jump(PC_from, ce.PC, ag.COUNTS);
	}
	return;
}


void op_add_s(coreenv& ce, arg& ag) {
	ce.FPR[ag.FD] = ce.FPR[ag.FS] + ce.FPR[ag.FT];
	return;
}

void op_sub_s(coreenv& ce, arg& ag) {
	ce.FPR[ag.FD] = ce.FPR[ag.FS] - ce.FPR[ag.FT];
	return;
}

void op_mul_s(coreenv& ce, arg& ag) {
	ce.FPR[ag.FD] = ce.FPR[ag.FS] * ce.FPR[ag.FT];
	return;
}

void op_div_s(coreenv& ce, arg& ag) {
	ce.FPR[ag.FD] = ce.FPR[ag.FS] / ce.FPR[ag.FT];
	return;
}

void op_mov_s(coreenv& ce, arg& ag) {
	ce.FPR[ag.FD] = ce.FPR[ag.FT];
	return;
}

void op_neg_s(coreenv& ce, arg& ag) {
	ce.FPR[ag.FD] = -ce.FPR[ag.FT];
	return;
}

void op_abs_s(coreenv& ce, arg& ag) {
	ce.FPR[ag.FD] = (ce.FPR[ag.FT] < 0) ? -ce.FPR[ag.FT] : ce.FPR[ag.FT];
	return;
}

void op_sqrt_s(coreenv& ce, arg& ag) {
	ce.FPR[ag.FD] = (float)sqrt((double)ce.FPR[ag.FT]);
	return;
}
void op_c_eq_s(coreenv& ce, arg& ag) {
	ce.FPCC[ag.CC] = ce.FPR[ag.FS] == ce.FPR[ag.FT] ? '1' : '0';
	return;
}

void op_c_lt_s(coreenv& ce, arg& ag) {
	ce.FPCC[ag.CC] = ce.FPR[ag.FS] < ce.FPR[ag.FT] ? '1' : '0';
	return;
}

void op_c_le_s(coreenv& ce, arg& ag) {
	ce.FPCC[ag.CC] = ce.FPR[ag.FS] <= ce.FPR[ag.FT] ? '1' : '0';
	return;
}

void op_lw_s(coreenv& ce, arg& ag) {
	int addr;
	addr = ce.GPR[ag.RS] + ag.C;
	if (!(0 <= addr && addr < 0x80000)) {
		fprintf(stderr, "overflow: trying to lw_s from %X\n", addr);
		ag.STOP = 1;
		return;
	}
	ce.FPR[ag.FT] = *((float*)&ce.DAT[addr]);
	return;
}

void op_sw_s_parent(coreenv& ce, arg& ag) {
	int addr;
	addr = CE[0].GPR[ag.RS] + ag.C;
	if (!(0 <= addr && addr < 0x80000)) {
		fprintf(stderr, "overflow: trying to sw_s to %X\n", addr);
		ag.STOP = 1;
		return;
	}
	for(int i = 0; i < CORE_NUM; i++) {
		*((float*)&CE[i].DAT[addr]) = CE[0].FPR[ag.FT];
	}
	return;
}

void op_sw_s_child(coreenv& ce, arg& ag) {
	int addr;
	addr = ce.GPR[ag.RS] + ag.C;
	if (!(0 <= addr && addr < 0x80000)) {
		fprintf(stderr, "overflow: trying to sw_s to %X\n", addr);
		ag.STOP = 1;
		return;
	}
	*((float*)&ce.DAT[addr]) = ce.FPR[ag.FT];
	return;
}

void op_ftoi(coreenv& ce, arg& ag) {
	ce.GPR[ag.RT] = lrintf(ce.FPR[ag.FS]);
	return;
}

void op_itof(coreenv& ce, arg& ag) {
	ce.FPR[ag.FT] = (float) ((int32_t)ce.GPR[ag.RS]);
	return;
}
pthread_mutex_t mtx1 = PTHREAD_MUTEX_INITIALIZER, mtx2 = PTHREAD_MUTEX_INITIALIZER;
void op_next(coreenv& ce, arg& ag) {
	//next rs
	pthread_mutex_lock(&mtx1);
	ce.GPR[ag.RS] = AC;
	AC += AD;
	pthread_mutex_unlock(&mtx1);
	return;
}

void op_acc(coreenv& ce, arg& ag) {
	//acc rs, ft
	pthread_mutex_lock(&mtx2);
	CE[0].FPR[29+ag.RS] = ce.FPR[ag.FT];
	pthread_mutex_unlock(&mtx2);
	return;
}
	
void op_fork(coreenv& ce, arg& ag) {
	//fork rt, rd
	AC = CE[0].GPR[ag.RT];
	AD = CE[0].GPR[ag.RD];
	int spc = CE[0].PC;
	for(int i = 0; i < 32; i++) {
		for(int j = 1; j < CORE_NUM; j++) {
			CE[j].GPR[i] = CE[0].GPR[i];
			CE[j].FPR[i] = CE[0].FPR[i];
		}
	}
	pthread_t tid[CORE_NUM];
	thread_arg ta[CORE_NUM];
	for(int i = 0; i < CORE_NUM; i++) {
		ta[i].index = i;
		ta[i].repeat = ag.REPEAT;
		int rv = pthread_create(&tid[i], NULL, invoke_child_core, (void*)&ta[i]);
		if (rv != 0) {
			perror("fork pthread_create");
			ag.STOP = 1;
			return;
		}
	}
	for(int i = 0; i < CORE_NUM; i++) {
		void *vrp;
		int rv = pthread_join(tid[i], &vrp);
		if (rv != 0 || vrp != NULL) {
			if (rv != 0) {
				perror("fork");
			} else {
				fprintf(stderr, "forked core stopped unexpectedly");
			}
			ag.STOP = 1;
			return;
		}
		if (ta[i].repeat < ag.REPEAT) {
			ag.REPEAT = ta[i].repeat;
		}
	}
	CE[0].PC = spc;
	return;
}

void op_end(coreenv& ce, arg& ag) {
	ag.STOP = 2;
	return;
}

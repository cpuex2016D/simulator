#include"sim.h"
#define WNSZ 0x100

void print_state(coreenv& ce, arg& ag) {
	fprintf(stderr, "%d\t", ce.PC);
	ag.OP_PRNT(ag);
	fprintf(stderr, "GPR\n0-3\t%d\t%d\t%d\t%d\n4-7\t%d\t%d\t%d\t%d\n8-11\t%d\t%d\t%d\t%d\n12-15\t%d\t%d\t%d\t%d\n16-19\t%d\t%d\t%d\t%d\n20-23\t%d\t%d\t%d\t%d\n24-27\t%d\t%d\t%d\t%d\n28-31\t%d\t%X\t%X\t%d\nFPR FPCC: %s\n0-3\t%f\t%f\t%f\t%f\n4-7\t%f\t%f\t%f\t%f\n8-11\t%f\t%f\t%f\t%f\n12-15\t%f\t%f\t%f\t%f\n16-19\t%f\t%f\t%f\t%f\n20-23\t%f\t%f\t%f\t%f\n24-27\t%f\t%f\t%f\t%f\n28-31\t%f\t%f\t%f\t%f\nAC = %d\t AD = %d\n\n", ce.GPR[0], ce.GPR[1], ce.GPR[2], ce.GPR[3], ce.GPR[4], ce.GPR[5], ce.GPR[6], ce.GPR[7], ce.GPR[8], ce.GPR[9], ce.GPR[10], ce.GPR[11], ce.GPR[12], ce.GPR[13], ce.GPR[14], ce.GPR[15], ce.GPR[16], ce.GPR[17], ce.GPR[18], ce.GPR[19], ce.GPR[20], ce.GPR[21], ce.GPR[22], ce.GPR[23], ce.GPR[24], ce.GPR[25], ce.GPR[26], ce.GPR[27], ce.GPR[28], ce.GPR[29], ce.GPR[30], ce.GPR[31], ce.FPCC, ce.FPR[0], ce.FPR[1], ce.FPR[2], ce.FPR[3], ce.FPR[4], ce.FPR[5], ce.FPR[6], ce.FPR[7], ce.FPR[8], ce.FPR[9], ce.FPR[10], ce.FPR[11], ce.FPR[12], ce.FPR[13], ce.FPR[14], ce.FPR[15], ce.FPR[16], ce.FPR[17], ce.FPR[18], ce.FPR[19], ce.FPR[20], ce.FPR[21], ce.FPR[22], ce.FPR[23], ce.FPR[24], ce.FPR[25], ce.FPR[26], ce.FPR[27], ce.FPR[28], ce.FPR[29], ce.FPR[30], ce.FPR[31], AC, AD);
	return;
}

/*
void print_stats(coreenv& ce, arg& ag) {
	fprintf(stderr, "\nadd:\t%llu\naddi:\t%llu\nsub:\t%llu\nand:\t%llu\nandi:\t%llu\nor:\t%llu\nori:\t%llu\nnor:\t%llu\nsll:\t%llu\nsrl:\t%llu\nslt:\t%llu\nslti:\t%llu\nbeq:\t%llu\nbne:\t%llu\nj:\t%llu\njal:\t%llu\njr:\t%llu\njalr:\t%llu\nlw:\t%llu\nlui:\t%llu\nsw:\t%llu\nin:\t%llu\nout:\t%llu\nbt.s:\t%llu\nbf.s:\t%llu\nadd.s:\t%llu\nsub.s:\t%llu\nmul.s:\t%llu\ndiv.s:\t%llu\nmov.s:\t%llu\nneg.s:\t%llu\nabs.s:\t%llu\nsqrt.s:\t%llu\nc.eq.s:\t%llu\nc.lt.s:\t%llu\nc.le.s:\t%llu\nlw.s:\t%llu\nsw.s:\t%llu\nftoi:\t%llu\nitof:\t%llu\nprediction success:\t%llu\nprediction failure:\t%llu\n", OP_COUNT[ADD_L], OP_COUNT[ADDI_L], OP_COUNT[SUB_L], OP_COUNT[AND_L], OP_COUNT[ANDI_L], OP_COUNT[OR_L], OP_COUNT[ORI_L], OP_COUNT[NOR_L], OP_COUNT[SLL_L], OP_COUNT[SRL_L], OP_COUNT[SLT_L], OP_COUNT[SLTI_L], OP_COUNT[BEQ_L], OP_COUNT[BNE_L], OP_COUNT[J_L], OP_COUNT[JAL_L], OP_COUNT[JR_L], OP_COUNT[JALR_L], OP_COUNT[LW_L], OP_COUNT[LUI_L], OP_COUNT[SW_L], OP_COUNT[IN_L], OP_COUNT[OUT_L], OP_COUNT[BT_S_L], OP_COUNT[BF_S_L], OP_COUNT[ADD_S_L], OP_COUNT[SUB_S_L], OP_COUNT[MUL_S_L], OP_COUNT[DIV_S_L], OP_COUNT[MOV_S_L], OP_COUNT[NEG_S_L], OP_COUNT[ABS_S_L], OP_COUNT[SQRT_S_L], OP_COUNT[C_EQ_S_L], OP_COUNT[C_LT_S_L], OP_COUNT[C_LE_S_L], OP_COUNT[LW_S_L], OP_COUNT[SW_S_L], OP_COUNT[FTOI_L], OP_COUNT[ITOF_L], BP_COUNT[1], BP_COUNT[0]);
	return;
}
*/

void print_memory(uint32_t *dat, int addr, int dhf) {
	int line;
	if (dhf == 0) {
		for(line = addr; line < addr + WNSZ; line += 8) {
			fprintf(stderr, "%5X %8d %8d %8d %8d %8d %8d %8d %8d\n", line, 
				dat[line], dat[line+1], dat[line+2], dat[line+3], 
				dat[line+4], dat[line+5], dat[line+6], dat[line+7]);
		}
	}
	if (dhf == 1) {
		for(line = addr; line < addr + WNSZ; line += 8) {
			fprintf(stderr, "%5X %8X %8X %8X %8X %8X %8X %8X %8X\n", line, 
				dat[line], dat[line+1], dat[line+2], dat[line+3], 
				dat[line+4], dat[line+5], dat[line+6], dat[line+7]);
		}
	}
	if (dhf == 2) {
		for(line = addr; line < addr + WNSZ; line += 8) {
			fprintf(stderr, "%5X %8f %8f %8f %8f %8f %8f %8f %8f\n", line, 
				*((float*)(dat+line)), *((float*)(dat+line+1)), *((float*)(dat+line+2)), *((float*)(dat+line+3)), 
				*((float*)(dat+line+4)), *((float*)(dat+line+5)), *((float*)(dat+line+6)), *((float*)(dat+line+7)));
		}
	}
}

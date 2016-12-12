#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"prnt_op.h"
#include"exec_op.h"

#define GET_RS(x) ((0x03E00000 & (x)) >> 21)
#define GET_RT(x) ((0x001F0000 & (x)) >> 16)
#define GET_RD(x) ((0x0000F800 & (x)) >> 11)
#define GET_LC(x) (0x03FFFFFF & (x))
#define GET_SC(x) (0x0000FFFF & (x))
#define GET_SSC(x) ((0x000007C0 & (x)) >> 6)
#define GET_BCC(x) ((0x001C0000 & (x)) >> 18)
#define GET_FT(x) ((0x001F0000 & (x)) >> 16)
#define GET_FS(x) ((0x0000F800 & (x)) >> 11)
#define GET_FD(x) ((0x000007C0 & (x)) >> 6)
#define GET_CCC(x) ((0x00000700 & (x)) >> 8)
#define GET_COND(x) (0x0000000F & (x))

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
int32_t RD, RS, RT, C, FD, FS, FT, CC;


void examine_op(void) {
	switch(OP>>29) {
	case 0: // add sub and or nor slt j jal beq bne
		switch(OP>>26) {
		case 0:
			if (OP&0x20) {
				RD = GET_RD(OP);
				RS = GET_RS(OP);
				RT = GET_RT(OP);
				switch(OP&0x3F) {
				case 0x20:
					OP_EX = op_add;
					OP_PRNT = prnt_add;
					return;
				case 0x22:
					OP_EX = op_sub;
					OP_PRNT = prnt_sub;
					return;
				case 0x24:
					OP_EX = op_and;
					OP_PRNT = prnt_and;
					return;
				case 0x25:
					OP_EX = op_or;
					OP_PRNT = prnt_or;
					return;
				case 0x27:
					OP_EX = op_nor;
					OP_PRNT = prnt_nor;
					return;
				case 0x2A:
					OP_EX = op_slt;
					OP_PRNT = prnt_slt;
					return;
				default:
					break;
				}
			} else if ((OP&0x3F) == 0) {
				RD = GET_RD(OP);
				RT = GET_RT(OP);
				C = GET_SSC(OP);
				OP_EX = op_sll;
				OP_PRNT = prnt_sll;
				return;
			} else if ((OP&0x3F) == 2) {
				RD = GET_RD(OP);
				RT = GET_RT(OP);
				C = GET_SSC(OP);
				OP_EX = op_srl;
				OP_PRNT = prnt_srl;
				return;
			} else if ((OP&0x3F) == 8) {
				RS = GET_RS(OP);
				OP_EX = op_jr;
				OP_PRNT = prnt_jr;
				return;
			} else if ((OP&0x3F) == 9) {
				RS = GET_RS(OP);
				OP_EX = op_jalr;
				OP_PRNT = prnt_jalr;
				return;
			}
			break;
		case 2:
			C = GET_LC(OP);
			OP_EX = op_j;
			OP_PRNT = prnt_j;
			return;
		case 3:
			C = GET_LC(OP);
			OP_EX = op_jal;
			OP_PRNT = prnt_jal;
			return;
		case 4:
			RS = GET_RS(OP);
			RT = GET_RT(OP);
			C = (int16_t)OP;
			OP_EX = op_beq;
			OP_PRNT = prnt_beq;
			return;
		case 5:
			RS = GET_RS(OP);
			RT = GET_RT(OP);
			C = (int16_t)OP;
			OP_EX = op_bne;
			OP_PRNT = prnt_bne;
			return;
		default:
			break;
		}
		break;
	case 1: // addi slti andi ori lui
		switch((OP>>26)&7) {
		case 0:
			RT = GET_RT(OP);
			RS = GET_RS(OP);
			C = (int16_t)OP;
			OP_EX = op_addi;
			OP_PRNT = prnt_addi;
			return;
		case 2:
			RT = GET_RT(OP);
			RS = GET_RS(OP);
			C = (int16_t)OP;
			OP_EX = op_slti;
			OP_PRNT = prnt_slti;
			return;
		case 4:
			RT = GET_RT(OP);
			RS = GET_RS(OP);
			C = (uint16_t)OP;
			OP_EX = op_andi;
			OP_PRNT = prnt_andi;
			return;
		case 5:
			RT = GET_RT(OP);
			RS = GET_RS(OP);
			C = (uint16_t)OP;
			OP_EX = op_ori;
			OP_PRNT = prnt_ori;
			return;
		case 7:
			RT = GET_RT(OP);
			C = (uint16_t)OP;
			OP_EX = op_lui;
			OP_PRNT = prnt_lui;
			return;
		default:
			break;
		}
		break;
	case 2:
		if ((OP&0xFFE00000) == 0x46000000) {
			//add.s sub.s mul.s div.s mov.s neg.s abs.s sqrt.s c.eq.s c.lt.s c.le.s
			FT = GET_FT(OP);
			if (OP&0x4) {
				switch(OP&0x3F) {
				case 6:
					FD = GET_FD(OP);
					OP_EX = op_mov_s;
					OP_PRNT = prnt_mov_s;
					return;
				case 7:
					FD = GET_FD(OP);
					OP_EX = op_neg_s;
					OP_PRNT = prnt_neg_s;
					return;
				case 5:
					FD = GET_FD(OP);
					OP_EX = op_abs_s;
					OP_PRNT = prnt_abs_s;
					return;
				case 4:
					FD = GET_FD(OP);
					OP_EX = op_sqrt_s;
					OP_PRNT = prnt_sqrt_s;
					return;
				case 0x3C:
					FD = GET_FD(OP);
					FS = GET_FS(OP);
					CC = GET_CCC(OP);
					OP_EX = op_c_lt_s;
					OP_PRNT = prnt_c_lt_s;
					return;
				case 0x3E:
					FD = GET_FD(OP);
					FS = GET_FS(OP);
					CC = GET_CCC(OP);
					OP_EX = op_c_le_s;
					OP_PRNT = prnt_c_le_s;
					return;
				default:
					break;
				}
			} else {
				FS = GET_FS(OP);
				switch(OP&0x3F) {
				case 0:
					FD = GET_FD(OP);
					OP_EX = op_add_s;
					OP_PRNT = prnt_add_s;
					return;
				case 1:
					FD = GET_FD(OP);
					OP_EX = op_sub_s;
					OP_PRNT = prnt_sub_s;
					return;
				case 2:
					FD = GET_FD(OP);
					OP_EX = op_mul_s;
					OP_PRNT = prnt_mul_s;
					return;
				case 3:
					FD = GET_FD(OP);
					OP_EX = op_div_s;
					OP_PRNT = prnt_div_s;
					return;
				case 0x32:
					CC = GET_CCC(OP);
					OP_EX = op_c_eq_s;
					OP_PRNT = prnt_c_eq_s;
					return;
				default:
					break;
				}
			}
		} else if ((OP&0xFFE30000) == 0x45010000) {
			CC = GET_BCC(OP);
			C = (int16_t)OP;
			OP_EX = op_bt_s;
			OP_PRNT = prnt_bt_s;
			return;
		} else if ((OP&0xFFE30000) == 0x45000000) {
			CC = GET_BCC(OP);
			C = (int16_t)OP;
			OP_EX = op_bf_s;
			OP_PRNT = prnt_bf_s;
			return;
		}
		break;
	case 3:
		if ((OP&0xFC000000) == 0x68000000) {
			RT = GET_RT(OP);
			OP_EX = op_in;
			OP_PRNT = prnt_in;
			return;
		} else if ((OP&0xFC000000) == 0x6C000000) {
			RT = GET_RT(OP);
			OP_EX = op_out;
			OP_PRNT = prnt_out;
			return;
		}
		break;
	case 4:
		if ((OP&0xFC000000) == 0x8C000000) {
			RT = GET_RT(OP);
			RS = GET_RS(OP);
			C = (int16_t)OP;
			OP_EX = op_lw;
			OP_PRNT = prnt_lw;
			return;
		}
		break;
	case 5:
		if ((OP&0xFC000000) == 0xAC000000) {
			RT = GET_RT(OP);
			RS = GET_RS(OP);
			C = (int16_t)OP;
			OP_EX = op_sw;
			OP_PRNT = prnt_sw;
			return;
		}
		break;
	case 6:
		if ((OP&0xFC000000) == 0xC0000000) {
			RS = GET_RS(OP);
			FT = GET_RT(OP);
			OP_EX = op_itof;
			OP_PRNT = prnt_itof;
			return;
		} else if ((OP&0xFC000000) == 0xC4000000) {
			RS = GET_RS(OP);
			FT = GET_FT(OP);
			C = (int16_t)OP;
			OP_EX = op_lw_s;
			OP_PRNT = prnt_lw_s;
			return;
		}
		break;
	case 7:
		if ((OP&0xFC000000) == 0xE0000000) {
			FS = GET_RS(OP);
			RT = GET_RT(OP);
			OP_EX = op_ftoi;
			OP_PRNT = prnt_ftoi;
			return;
		} else if ((OP&0xFC000000) == 0xE4000000) {
			RS = GET_RS(OP);
			FT = GET_FT(OP);
			C = (int16_t)OP;
			OP_EX = op_sw_s;
			OP_PRNT = prnt_sw_s;
			return;
		}
		break;
	default:
		break;
	}
	fprintf(stderr, "unknown operator\n");
	STOP = 1;
	return;
}


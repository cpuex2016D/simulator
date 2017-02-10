#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"prnt_op.h"
#include"exec_op.h"
#include"sim.h"

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


void Core::examine_op(void) {
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
					OP_TYPE = ADD_L;
					OP_EX = &Core::op_add;
					OP_PRNT = &Core::prnt_add;
					return;
				case 0x22:
					OP_TYPE = SUB_L;
					OP_EX = &Core::op_sub;
					OP_PRNT = &Core::prnt_sub;
					return;
				case 0x24:
					OP_TYPE = AND_L;
					OP_EX = &Core::op_and;
					OP_PRNT = &Core::prnt_and;
					return;
				case 0x25:
					OP_TYPE = OR_L;
					OP_EX = &Core::op_or;
					OP_PRNT = &Core::prnt_or;
					return;
				case 0x27:
					OP_TYPE = NOR_L;
					OP_EX = &Core::op_nor;
					OP_PRNT = &Core::prnt_nor;
					return;
				case 0x2A:
					OP_TYPE = SLT_L;
					OP_EX = &Core::op_slt;
					OP_PRNT = &Core::prnt_slt;
					return;
				default:
					break;
				}
			} else if ((OP&0x3F) == 0) {
				RD = GET_RD(OP);
				RT = GET_RT(OP);
				C = GET_SSC(OP);
				OP_TYPE = SLL_L;
				OP_EX = &Core::op_sll;
				OP_PRNT = &Core::prnt_sll;
				return;
			} else if ((OP&0x3F) == 2) {
				RD = GET_RD(OP);
				RT = GET_RT(OP);
				C = GET_SSC(OP);
				OP_TYPE = SRL_L;
				OP_EX = &Core::op_srl;
				OP_PRNT = &Core::prnt_srl;
				return;
			} else if ((OP&0x3F) == 8) {
				RS = GET_RS(OP);
				OP_TYPE = JR_L;
				OP_EX = &Core::op_jr;
				OP_PRNT = &Core::prnt_jr;
				return;
			} else if ((OP&0x3F) == 9) {
				RS = GET_RS(OP);
				OP_TYPE = JALR_L;
				OP_EX = &Core::op_jalr;
				OP_PRNT = &Core::prnt_jalr;
				return;
			}
			break;
		case 2:
			C = GET_LC(OP);
			OP_TYPE = J_L;
			OP_EX = &Core::op_j;
			OP_PRNT = &Core::prnt_j;
			return;
		case 3:
			C = GET_LC(OP);
			OP_TYPE = JAL_L;
			OP_EX = &Core::op_jal;
			OP_PRNT = &Core::prnt_jal;
			return;
		case 4:
			RS = GET_RS(OP);
			RT = GET_RT(OP);
			C = (int16_t)OP;
			OP_TYPE = BEQ_L;
			OP_EX = &Core::op_beq;
			OP_PRNT = &Core::prnt_beq;
			return;
		case 5:
			RS = GET_RS(OP);
			RT = GET_RT(OP);
			C = (int16_t)OP;
			OP_TYPE = BNE_L;
			OP_EX = &Core::op_bne;
			OP_PRNT = &Core::prnt_bne;
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
			OP_TYPE = ADDI_L;
			OP_EX = &Core::op_addi;
			OP_PRNT = &Core::prnt_addi;
			return;
		case 2:
			RT = GET_RT(OP);
			RS = GET_RS(OP);
			C = (int16_t)OP;
			OP_TYPE = SLTI_L;
			OP_EX = &Core::op_slti;
			OP_PRNT = &Core::prnt_slti;
			return;
		case 4:
			RT = GET_RT(OP);
			RS = GET_RS(OP);
			C = (uint16_t)OP;
			OP_TYPE = ANDI_L;
			OP_EX = &Core::op_andi;
			OP_PRNT = &Core::prnt_andi;
			return;
		case 5:
			RT = GET_RT(OP);
			RS = GET_RS(OP);
			C = (uint16_t)OP;
			OP_TYPE = ORI_L;
			OP_EX = &Core::op_ori;
			OP_PRNT = &Core::prnt_ori;
			return;
		case 7:
			RT = GET_RT(OP);
			C = (uint16_t)OP;
			OP_TYPE = LUI_L;
			OP_EX = &Core::op_lui;
			OP_PRNT = &Core::prnt_lui;
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
					OP_TYPE = MOV_S_L;
					OP_EX = &Core::op_mov_s;
					OP_PRNT = &Core::prnt_mov_s;
					return;
				case 7:
					FD = GET_FD(OP);
					OP_TYPE = NEG_S_L;
					OP_EX = &Core::op_neg_s;
					OP_PRNT = &Core::prnt_neg_s;
					return;
				case 5:
					FD = GET_FD(OP);
					OP_TYPE = ABS_S_L;
					OP_EX = &Core::op_abs_s;
					OP_PRNT = &Core::prnt_abs_s;
					return;
				case 4:
					FD = GET_FD(OP);
					OP_TYPE = SQRT_S_L;
					OP_EX = &Core::op_sqrt_s;
					OP_PRNT = &Core::prnt_sqrt_s;
					return;
				case 0x3C:
					FD = GET_FD(OP);
					FS = GET_FS(OP);
					CC = GET_CCC(OP);
					OP_TYPE = C_LT_S_L;
					OP_EX = &Core::op_c_lt_s;
					OP_PRNT = &Core::prnt_c_lt_s;
					return;
				case 0x3E:
					FD = GET_FD(OP);
					FS = GET_FS(OP);
					CC = GET_CCC(OP);
					OP_TYPE = C_LE_S_L;
					OP_EX = &Core::op_c_le_s;
					OP_PRNT = &Core::prnt_c_le_s;
					return;
				default:
					break;
				}
			} else {
				FS = GET_FS(OP);
				switch(OP&0x3F) {
				case 0:
					FD = GET_FD(OP);
					OP_TYPE = ADD_S_L;
					OP_EX = &Core::op_add_s;
					OP_PRNT = &Core::prnt_add_s;
					return;
				case 1:
					FD = GET_FD(OP);
					OP_TYPE = SUB_S_L;
					OP_EX = &Core::op_sub_s;
					OP_PRNT = &Core::prnt_sub_s;
					return;
				case 2:
					FD = GET_FD(OP);
					OP_TYPE = MUL_S_L;
					OP_EX = &Core::op_mul_s;
					OP_PRNT = &Core::prnt_mul_s;
					return;
				case 3:
					FD = GET_FD(OP);
					OP_TYPE = DIV_S_L;
					OP_EX = &Core::op_div_s;
					OP_PRNT = &Core::prnt_div_s;
					return;
				case 0x32:
					CC = GET_CCC(OP);
					OP_TYPE = C_EQ_S_L;
					OP_EX = &Core::op_c_eq_s;
					OP_PRNT = &Core::prnt_c_eq_s;
					return;
				default:
					break;
				}
			}
		} else if ((OP&0xFFE30000) == 0x45010000) {
			CC = GET_BCC(OP);
			C = (int16_t)OP;
			OP_TYPE = BT_S_L;
			OP_EX = &Core::op_bt_s;
			OP_PRNT = &Core::prnt_bt_s;
			return;
		} else if ((OP&0xFFE30000) == 0x45000000) {
			CC = GET_BCC(OP);
			C = (int16_t)OP;
			OP_TYPE = BF_S_L;
			OP_EX = &Core::op_bf_s;
			OP_PRNT = &Core::prnt_bf_s;
			return;
		}
		break;
	case 3:
		if ((OP&0xFC000000) == 0x68000000) {
			RT = GET_RT(OP);
			OP_TYPE = IN_L;
			OP_EX = &Core::op_in;
			OP_PRNT = &Core::prnt_in;
			return;
		} else if ((OP&0xFC000000) == 0x6C000000) {
			RT = GET_RT(OP);
			OP_TYPE = OUT_L;
			OP_EX = &Core::op_out;
			OP_PRNT = &Core::prnt_out;
			return;
		} else if ((OP&0xFC000000) == 0x74000000) {
			RS = GET_RS(OP);
			OP_TYPE = NEXT_L;
			OP_EX = &Core::op_next;
			OP_PRNT = &Core::prnt_next;
			return;
		} else if ((OP&0xFC000000) == 0x78000000) {
			RS = GET_RS(OP);
			RT = GET_RT(OP);
			OP_TYPE = ACC_L;
			OP_EX = &Core::op_acc;
			OP_PRNT = &Core::prnt_acc;
			return;
		} else if ((OP&0xFC000000) == 0x7C000000) {
			if (!PARALLEL) {
				RT = GET_RT(OP);
				RD = GET_RD(OP);
				OP_TYPE = FORK_L;
				OP_EX = &Core::op_fork;
				OP_PRNT = &Core::prnt_fork;
			} else {
				OP_TYPE = END_L;
				OP_EX = &Core::op_end;
				OP_PRNT = &Core::prnt_end;
			}
			return;
		}
		break;
	case 4:
		if ((OP&0xFC000000) == 0x8C000000) {
			RT = GET_RT(OP);
			RS = GET_RS(OP);
			C = (int16_t)OP;
			OP_TYPE = LW_L;
			OP_EX = &Core::op_lw;
			OP_PRNT = &Core::prnt_lw;
			return;
		}
		break;
	case 5:
		if ((OP&0xFC000000) == 0xAC000000) {
			RT = GET_RT(OP);
			RS = GET_RS(OP);
			C = (int16_t)OP;
			OP_TYPE = SW_L;
			OP_EX = &Core::op_sw;
			OP_PRNT = &Core::prnt_sw;
			return;
		}
		break;
	case 6:
		if ((OP&0xFC000000) == 0xC0000000) {
			RS = GET_RS(OP);
			FT = GET_RT(OP);
			OP_TYPE = ITOF_L;
			OP_EX = &Core::op_itof;
			OP_PRNT = &Core::prnt_itof;
			return;
		} else if ((OP&0xFC000000) == 0xC4000000) {
			RS = GET_RS(OP);
			FT = GET_FT(OP);
			C = (int16_t)OP;
			OP_TYPE = LW_S_L;
			OP_EX = &Core::op_lw_s;
			OP_PRNT = &Core::prnt_lw_s;
			return;
		}
		break;
	case 7:
		if ((OP&0xFC000000) == 0xE0000000) {
			FS = GET_RS(OP);
			RT = GET_RT(OP);
			OP_TYPE = FTOI_L;
			OP_EX = &Core::op_ftoi;
			OP_PRNT = &Core::prnt_ftoi;
			return;
		} else if ((OP&0xFC000000) == 0xE4000000) {
			RS = GET_RS(OP);
			FT = GET_FT(OP);
			C = (int16_t)OP;
			OP_TYPE = SW_S_L;
			OP_EX = &Core::op_sw_s;
			OP_PRNT = &Core::prnt_sw_s;
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


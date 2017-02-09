#include"exec_op.h"
#include"prnt_op.h"
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


void examine_op_parent(uint32_t op, arg& ag) {
	if (op>>26 == 0b011111) {
		ag.RT = GET_RT(op);
		ag.RD = GET_RD(op);
		ag.OP_EX = op_fork;
		ag.OP_PRNT = prnt_fork;
		return;
	}
	switch(op>>29) {
	case 0: // add sub and or nor slt j jal beq bne
		switch(op>>26) {
		case 0:
			if (op&0x20) {
				ag.RD = GET_RD(op);
				ag.RS = GET_RS(op);
				ag.RT = GET_RT(op);
				switch(op&0x3F) {
				case 0x20:
					//OP_TYPE = ADD_L;
					ag.OP_EX = op_add;
					ag.OP_PRNT = prnt_add;
					return;
				case 0x22:
					//OP_TYPE = SUB_L;
					ag.OP_EX = op_sub;
					ag.OP_PRNT = prnt_sub;
					return;
				case 0x24:
					//OP_TYPE = AND_L;
					ag.OP_EX = op_and;
					ag.OP_PRNT = prnt_and;
					return;
				case 0x25:
					//OP_TYPE = OR_L;
					ag.OP_EX = op_or;
					ag.OP_PRNT = prnt_or;
					return;
				case 0x27:
					//OP_TYPE = NOR_L;
					ag.OP_EX = op_nor;
					ag.OP_PRNT = prnt_nor;
					return;
				case 0x2A:
					//OP_TYPE = SLT_L;
					ag.OP_EX = op_slt;
					ag.OP_PRNT = prnt_slt;
					return;
				default:
					break;
				}
			} else if ((op&0x3F) == 0) {
				ag.RD = GET_RD(op);
				ag.RT = GET_RT(op);
				ag.C = GET_SSC(op);
				//OP_TYPE = SLL_L;
				ag.OP_EX = op_sll;
				ag.OP_PRNT = prnt_sll;
				return;
			} else if ((op&0x3F) == 2) {
				ag.RD = GET_RD(op);
				ag.RT = GET_RT(op);
				ag.C = GET_SSC(op);
				//OP_TYPE = SRL_L;
				ag.OP_EX = op_srl;
				ag.OP_PRNT = prnt_srl;
				return;
			} else if ((op&0x3F) == 8) {
				ag.RS = GET_RS(op);
				//OP_TYPE = JR_L;
				ag.OP_EX = op_jr;
				ag.OP_PRNT = prnt_jr;
				return;
			} else if ((op&0x3F) == 9) {
				ag.RS = GET_RS(op);
				//OP_TYPE = JALR_L;
				ag.OP_EX = op_jalr;
				ag.OP_PRNT = prnt_jalr;
				return;
			}
			break;
		case 2:
			ag.C = GET_LC(op);
			//OP_TYPE = J_L;
			ag.OP_EX = op_j;
			ag.OP_PRNT = prnt_j;
			return;
		case 3:
			ag.C = GET_LC(op);
			//OP_TYPE = JAL_L;
			ag.OP_EX = op_jal;
			ag.OP_PRNT = prnt_jal;
			return;
		case 4:
			ag.RS = GET_RS(op);
			ag.RT = GET_RT(op);
			ag.C = (int16_t)op;
			//OP_TYPE = BEQ_L;
			ag.OP_EX = op_beq;
			ag.OP_PRNT = prnt_beq;
			return;
		case 5:
			ag.RS = GET_RS(op);
			ag.RT = GET_RT(op);
			ag.C = (int16_t)op;
			//OP_TYPE = BNE_L;
			ag.OP_EX = op_bne;
			ag.OP_PRNT = prnt_bne;
			return;
		default:
			break;
		}
		break;
	case 1: // addi slti andi ori lui
		switch((op>>26)&7) {
		case 0:
			ag.RT = GET_RT(op);
			ag.RS = GET_RS(op);
			ag.C = (int16_t)op;
			//OP_TYPE = ADDI_L;
			ag.OP_EX = op_addi;
			ag.OP_PRNT = prnt_addi;
			return;
		case 2:
			ag.RT = GET_RT(op);
			ag.RS = GET_RS(op);
			ag.C = (int16_t)op;
			//OP_TYPE = SLTI_L;
			ag.OP_EX = op_slti;
			ag.OP_PRNT = prnt_slti;
			return;
		case 4:
			ag.RT = GET_RT(op);
			ag.RS = GET_RS(op);
			ag.C = (uint16_t)op;
			//OP_TYPE = ANDI_L;
			ag.OP_EX = op_andi;
			ag.OP_PRNT = prnt_andi;
			return;
		case 5:
			ag.RT = GET_RT(op);
			ag.RS = GET_RS(op);
			ag.C = (uint16_t)op;
			//OP_TYPE = ORI_L;
			ag.OP_EX = op_ori;
			ag.OP_PRNT = prnt_ori;
			return;
		case 7:
			ag.RT = GET_RT(op);
			ag.C = (uint16_t)op;
			//OP_TYPE = LUI_L;
			ag.OP_EX = op_lui;
			ag.OP_PRNT = prnt_lui;
			return;
		default:
			break;
		}
		break;
	case 2:
		if ((op&0xFFE00000) == 0x46000000) {
			//add.s sub.s mul.s div.s mov.s neg.s abs.s sqrt.s c.eq.s c.lt.s c.le.s
			ag.FT = GET_FT(op);
			if (op&0x4) {
				switch(op&0x3F) {
				case 6:
					ag.FD = GET_FD(op);
					//OP_TYPE = MOV_S_L;
					ag.OP_EX = op_mov_s;
					ag.OP_PRNT = prnt_mov_s;
					return;
				case 7:
					ag.FD = GET_FD(op);
					//OP_TYPE = NEG_S_L;
					ag.OP_EX = op_neg_s;
					ag.OP_PRNT = prnt_neg_s;
					return;
				case 5:
					ag.FD = GET_FD(op);
					//OP_TYPE = ABS_S_L;
					ag.OP_EX = op_abs_s;
					ag.OP_PRNT = prnt_abs_s;
					return;
				case 4:
					ag.FD = GET_FD(op);
					//OP_TYPE = SQRT_S_L;
					ag.OP_EX = op_sqrt_s;
					ag.OP_PRNT = prnt_sqrt_s;
					return;
				case 0x3C:
					ag.FD = GET_FD(op);
					ag.FS = GET_FS(op);
					ag.CC = GET_CCC(op);
					//OP_TYPE = C_LT_S_L;
					ag.OP_EX = op_c_lt_s;
					ag.OP_PRNT = prnt_c_lt_s;
					return;
				case 0x3E:
					ag.FD = GET_FD(op);
					ag.FS = GET_FS(op);
					ag.CC = GET_CCC(op);
					//OP_TYPE = C_LE_S_L;
					ag.OP_EX = op_c_le_s;
					ag.OP_PRNT = prnt_c_le_s;
					return;
				default:
					break;
				}
			} else {
				ag.FS = GET_FS(op);
				switch(op&0x3F) {
				case 0:
					ag.FD = GET_FD(op);
					//OP_TYPE = ADD_S_L;
					ag.OP_EX = op_add_s;
					ag.OP_PRNT = prnt_add_s;
					return;
				case 1:
					ag.FD = GET_FD(op);
					//OP_TYPE = SUB_S_L;
					ag.OP_EX = op_sub_s;
					ag.OP_PRNT = prnt_sub_s;
					return;
				case 2:
					ag.FD = GET_FD(op);
					//OP_TYPE = MUL_S_L;
					ag.OP_EX = op_mul_s;
					ag.OP_PRNT = prnt_mul_s;
					return;
				case 3:
					ag.FD = GET_FD(op);
					//OP_TYPE = DIV_S_L;
					ag.OP_EX = op_div_s;
					ag.OP_PRNT = prnt_div_s;
					return;
				case 0x32:
					ag.CC = GET_CCC(op);
					//OP_TYPE = C_EQ_S_L;
					ag.OP_EX = op_c_eq_s;
					ag.OP_PRNT = prnt_c_eq_s;
					return;
				default:
					break;
				}
			}
		} else if ((op&0xFFE30000) == 0x45010000) {
			ag.CC = GET_BCC(op);
			ag.C = (int16_t)op;
			//OP_TYPE = BT_S_L;
			ag.OP_EX = op_bt_s;
			ag.OP_PRNT = prnt_bt_s;
			return;
		} else if ((op&0xFFE30000) == 0x45000000) {
			ag.CC = GET_BCC(op);
			ag.C = (int16_t)op;
			//OP_TYPE = BF_S_L;
			ag.OP_EX = op_bf_s;
			ag.OP_PRNT = prnt_bf_s;
			return;
		}
		break;
	case 3:
		if ((op&0xFC000000) == 0x68000000) {
			ag.RT = GET_RT(op);
			//OP_TYPE = IN_L;
			ag.OP_EX = op_in;
			ag.OP_PRNT = prnt_in;
			return;
		} else if ((op&0xFC000000) == 0x6C000000) {
			ag.RT = GET_RT(op);
			//OP_TYPE = OUT_L;
			ag.OP_EX = op_out;
			ag.OP_PRNT = prnt_out;
			return;
		}
		break;
	case 4:
		if ((op&0xFC000000) == 0x8C000000) {
			ag.RT = GET_RT(op);
			ag.RS = GET_RS(op);
			ag.C = (int16_t)op;
			//OP_TYPE = LW_L;
			ag.OP_EX = op_lw;
			ag.OP_PRNT = prnt_lw;
			return;
		}
		break;
	case 5:
		if ((op&0xFC000000) == 0xAC000000) {
			ag.RT = GET_RT(op);
			ag.RS = GET_RS(op);
			ag.C = (int16_t)op;
			//OP_TYPE = SW_L;
			ag.OP_EX = op_sw_parent;
			ag.OP_PRNT = prnt_sw;
			return;
		}
		break;
	case 6:
		if ((op&0xFC000000) == 0xC0000000) {
			ag.RS = GET_RS(op);
			ag.FT = GET_RT(op);
			//OP_TYPE = ITOF_L;
			ag.OP_EX = op_itof;
			ag.OP_PRNT = prnt_itof;
			return;
		} else if ((op&0xFC000000) == 0xC4000000) {
			ag.RS = GET_RS(op);
			ag.FT = GET_FT(op);
			ag.C = (int16_t)op;
			//OP_TYPE = LW_S_L;
			ag.OP_EX = op_lw_s;
			ag.OP_PRNT = prnt_lw_s;
			return;
		}
		break;
	case 7:
		if ((op&0xFC000000) == 0xE0000000) {
			ag.FS = GET_RS(op);
			ag.RT = GET_RT(op);
			//OP_TYPE = FTOI_L;
			ag.OP_EX = op_ftoi;
			ag.OP_PRNT = prnt_ftoi;
			return;
		} else if ((op&0xFC000000) == 0xE4000000) {
			ag.RS = GET_RS(op);
			ag.FT = GET_FT(op);
			ag.C = (int16_t)op;
			//OP_TYPE = SW_S_L;
			ag.OP_EX = op_sw_s_parent;
			ag.OP_PRNT = prnt_sw_s;
			return;
		}
		break;
	default:
		break;
	}
	fprintf(stderr, "unknown operator\n");
	ag.STOP = 1;
	return;
}


void examine_op_child(uint32_t op, arg& ag) {
	if (op>>26 == 0b011111) {
		ag.OP_EX = op_end;
		ag.OP_PRNT = prnt_end;
		return;
	} else if (op>>26 == 0b011101) {
		ag.RS = GET_RS(op);
		ag.OP_EX = op_next;
		ag.OP_PRNT = prnt_next;
		return;
	} else if (op>>26 == 0b011110) {
		ag.RS = GET_RS(op);
		ag.FT = GET_FT(op);
		ag.OP_EX = op_acc;
		ag.OP_PRNT = prnt_acc;
		return;
	}
	
	switch(op>>29) {
	case 0: // add sub and or nor slt j jal beq bne
		switch(op>>26) {
		case 0:
			if (op&0x20) {
				ag.RD = GET_RD(op);
				ag.RS = GET_RS(op);
				ag.RT = GET_RT(op);
				switch(op&0x3F) {
				case 0x20:
					//OP_TYPE = ADD_L;
					ag.OP_EX = op_add;
					ag.OP_PRNT = prnt_add;
					return;
				case 0x22:
					//OP_TYPE = SUB_L;
					ag.OP_EX = op_sub;
					ag.OP_PRNT = prnt_sub;
					return;
				case 0x24:
					//OP_TYPE = AND_L;
					ag.OP_EX = op_and;
					ag.OP_PRNT = prnt_and;
					return;
				case 0x25:
					//OP_TYPE = OR_L;
					ag.OP_EX = op_or;
					ag.OP_PRNT = prnt_or;
					return;
				case 0x27:
					//OP_TYPE = NOR_L;
					ag.OP_EX = op_nor;
					ag.OP_PRNT = prnt_nor;
					return;
				case 0x2A:
					//OP_TYPE = SLT_L;
					ag.OP_EX = op_slt;
					ag.OP_PRNT = prnt_slt;
					return;
				default:
					break;
				}
			} else if ((op&0x3F) == 0) {
				ag.RD = GET_RD(op);
				ag.RT = GET_RT(op);
				ag.C = GET_SSC(op);
				//OP_TYPE = SLL_L;
				ag.OP_EX = op_sll;
				ag.OP_PRNT = prnt_sll;
				return;
			} else if ((op&0x3F) == 2) {
				ag.RD = GET_RD(op);
				ag.RT = GET_RT(op);
				ag.C = GET_SSC(op);
				//OP_TYPE = SRL_L;
				ag.OP_EX = op_srl;
				ag.OP_PRNT = prnt_srl;
				return;
			} else if ((op&0x3F) == 8) {
				ag.RS = GET_RS(op);
				//OP_TYPE = JR_L;
				ag.OP_EX = op_jr;
				ag.OP_PRNT = prnt_jr;
				return;
			} else if ((op&0x3F) == 9) {
				ag.RS = GET_RS(op);
				//OP_TYPE = JALR_L;
				ag.OP_EX = op_jalr;
				ag.OP_PRNT = prnt_jalr;
				return;
			}
			break;
		case 2:
			ag.C = GET_LC(op);
			//OP_TYPE = J_L;
			ag.OP_EX = op_j;
			ag.OP_PRNT = prnt_j;
			return;
		case 3:
			ag.C = GET_LC(op);
			//OP_TYPE = JAL_L;
			ag.OP_EX = op_jal;
			ag.OP_PRNT = prnt_jal;
			return;
		case 4:
			ag.RS = GET_RS(op);
			ag.RT = GET_RT(op);
			ag.C = (int16_t)op;
			//OP_TYPE = BEQ_L;
			ag.OP_EX = op_beq;
			ag.OP_PRNT = prnt_beq;
			return;
		case 5:
			ag.RS = GET_RS(op);
			ag.RT = GET_RT(op);
			ag.C = (int16_t)op;
			//OP_TYPE = BNE_L;
			ag.OP_EX = op_bne;
			ag.OP_PRNT = prnt_bne;
			return;
		default:
			break;
		}
		break;
	case 1: // addi slti andi ori lui
		switch((op>>26)&7) {
		case 0:
			ag.RT = GET_RT(op);
			ag.RS = GET_RS(op);
			ag.C = (int16_t)op;
			//OP_TYPE = ADDI_L;
			ag.OP_EX = op_addi;
			ag.OP_PRNT = prnt_addi;
			return;
		case 2:
			ag.RT = GET_RT(op);
			ag.RS = GET_RS(op);
			ag.C = (int16_t)op;
			//OP_TYPE = SLTI_L;
			ag.OP_EX = op_slti;
			ag.OP_PRNT = prnt_slti;
			return;
		case 4:
			ag.RT = GET_RT(op);
			ag.RS = GET_RS(op);
			ag.C = (uint16_t)op;
			//OP_TYPE = ANDI_L;
			ag.OP_EX = op_andi;
			ag.OP_PRNT = prnt_andi;
			return;
		case 5:
			ag.RT = GET_RT(op);
			ag.RS = GET_RS(op);
			ag.C = (uint16_t)op;
			//OP_TYPE = ORI_L;
			ag.OP_EX = op_ori;
			ag.OP_PRNT = prnt_ori;
			return;
		case 7:
			ag.RT = GET_RT(op);
			ag.C = (uint16_t)op;
			//OP_TYPE = LUI_L;
			ag.OP_EX = op_lui;
			ag.OP_PRNT = prnt_lui;
			return;
		default:
			break;
		}
		break;
	case 2:
		if ((op&0xFFE00000) == 0x46000000) {
			//add.s sub.s mul.s div.s mov.s neg.s abs.s sqrt.s c.eq.s c.lt.s c.le.s
			ag.FT = GET_FT(op);
			if (op&0x4) {
				switch(op&0x3F) {
				case 6:
					ag.FD = GET_FD(op);
					//OP_TYPE = MOV_S_L;
					ag.OP_EX = op_mov_s;
					ag.OP_PRNT = prnt_mov_s;
					return;
				case 7:
					ag.FD = GET_FD(op);
					//OP_TYPE = NEG_S_L;
					ag.OP_EX = op_neg_s;
					ag.OP_PRNT = prnt_neg_s;
					return;
				case 5:
					ag.FD = GET_FD(op);
					//OP_TYPE = ABS_S_L;
					ag.OP_EX = op_abs_s;
					ag.OP_PRNT = prnt_abs_s;
					return;
				case 4:
					ag.FD = GET_FD(op);
					//OP_TYPE = SQRT_S_L;
					ag.OP_EX = op_sqrt_s;
					ag.OP_PRNT = prnt_sqrt_s;
					return;
				case 0x3C:
					ag.FD = GET_FD(op);
					ag.FS = GET_FS(op);
					ag.CC = GET_CCC(op);
					//OP_TYPE = C_LT_S_L;
					ag.OP_EX = op_c_lt_s;
					ag.OP_PRNT = prnt_c_lt_s;
					return;
				case 0x3E:
					ag.FD = GET_FD(op);
					ag.FS = GET_FS(op);
					ag.CC = GET_CCC(op);
					//OP_TYPE = C_LE_S_L;
					ag.OP_EX = op_c_le_s;
					ag.OP_PRNT = prnt_c_le_s;
					return;
				default:
					break;
				}
			} else {
				ag.FS = GET_FS(op);
				switch(op&0x3F) {
				case 0:
					ag.FD = GET_FD(op);
					//OP_TYPE = ADD_S_L;
					ag.OP_EX = op_add_s;
					ag.OP_PRNT = prnt_add_s;
					return;
				case 1:
					ag.FD = GET_FD(op);
					//OP_TYPE = SUB_S_L;
					ag.OP_EX = op_sub_s;
					ag.OP_PRNT = prnt_sub_s;
					return;
				case 2:
					ag.FD = GET_FD(op);
					//OP_TYPE = MUL_S_L;
					ag.OP_EX = op_mul_s;
					ag.OP_PRNT = prnt_mul_s;
					return;
				case 3:
					ag.FD = GET_FD(op);
					//OP_TYPE = DIV_S_L;
					ag.OP_EX = op_div_s;
					ag.OP_PRNT = prnt_div_s;
					return;
				case 0x32:
					ag.CC = GET_CCC(op);
					//OP_TYPE = C_EQ_S_L;
					ag.OP_EX = op_c_eq_s;
					ag.OP_PRNT = prnt_c_eq_s;
					return;
				default:
					break;
				}
			}
		} else if ((op&0xFFE30000) == 0x45010000) {
			ag.CC = GET_BCC(op);
			ag.C = (int16_t)op;
			//OP_TYPE = BT_S_L;
			ag.OP_EX = op_bt_s;
			ag.OP_PRNT = prnt_bt_s;
			return;
		} else if ((op&0xFFE30000) == 0x45000000) {
			ag.CC = GET_BCC(op);
			ag.C = (int16_t)op;
			//OP_TYPE = BF_S_L;
			ag.OP_EX = op_bf_s;
			ag.OP_PRNT = prnt_bf_s;
			return;
		}
		break;
	case 3:
		if ((op&0xFC000000) == 0x68000000) {
			ag.RT = GET_RT(op);
			//OP_TYPE = IN_L;
			ag.OP_EX = op_in;
			ag.OP_PRNT = prnt_in;
			return;
		} else if ((op&0xFC000000) == 0x6C000000) {
			ag.RT = GET_RT(op);
			//OP_TYPE = OUT_L;
			ag.OP_EX = op_out;
			ag.OP_PRNT = prnt_out;
			return;
		}
		break;
	case 4:
		if ((op&0xFC000000) == 0x8C000000) {
			ag.RT = GET_RT(op);
			ag.RS = GET_RS(op);
			ag.C = (int16_t)op;
			//OP_TYPE = LW_L;
			ag.OP_EX = op_lw;
			ag.OP_PRNT = prnt_lw;
			return;
		}
		break;
	case 5:
		if ((op&0xFC000000) == 0xAC000000) {
			ag.RT = GET_RT(op);
			ag.RS = GET_RS(op);
			ag.C = (int16_t)op;
			//OP_TYPE = SW_L;
			ag.OP_EX = op_sw_child;
			ag.OP_PRNT = prnt_sw;
			return;
		}
		break;
	case 6:
		if ((op&0xFC000000) == 0xC0000000) {
			ag.RS = GET_RS(op);
			ag.FT = GET_RT(op);
			//OP_TYPE = ITOF_L;
			ag.OP_EX = op_itof;
			ag.OP_PRNT = prnt_itof;
			return;
		} else if ((op&0xFC000000) == 0xC4000000) {
			ag.RS = GET_RS(op);
			ag.FT = GET_FT(op);
			ag.C = (int16_t)op;
			//OP_TYPE = LW_S_L;
			ag.OP_EX = op_lw_s;
			ag.OP_PRNT = prnt_lw_s;
			return;
		}
		break;
	case 7:
		if ((op&0xFC000000) == 0xE0000000) {
			ag.FS = GET_RS(op);
			ag.RT = GET_RT(op);
			//OP_TYPE = FTOI_L;
			ag.OP_EX = op_ftoi;
			ag.OP_PRNT = prnt_ftoi;
			return;
		} else if ((op&0xFC000000) == 0xE4000000) {
			ag.RS = GET_RS(op);
			ag.FT = GET_FT(op);
			ag.C = (int16_t)op;
			//OP_TYPE = SW_S_L;
			ag.OP_EX = op_sw_s_child;
			ag.OP_PRNT = prnt_sw_s;
			return;
		}
		break;
	default:
		break;
	}
	fprintf(stderr, "unknown operator\n");
	ag.STOP = 1;
	return;
}


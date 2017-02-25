#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"prnt_op.h"
#include"exec_op.h"
#include"sim.h"

#define GET_R0(x) ((0x03E00000 & (x)) >> 21)
#define GET_R1(x) ((0x001F0000 & (x)) >> 16)
#define GET_R2(x) ((0x0000F800 & (x)) >> 11)
#define GET_C_LWI(x) (0x0001FFFF & (x))
#define GET_C_SW(x) ((int16_t)(((0x03E00000 & (x)) >> 10) | (0x000007FF & (x))))
#define GET_C_SWI(x) (((0x03E00000 & (x)) >> 10) | (0x000107FF & (x)))
#define GET_C_J(x) (((0x00E00000 & (x)) >> 10) | (0x000007FF & (x)))
#define GET_C2_SUB(x) ((((0x0F000000 & (x)) << 4 >> 12) | (0x0000F800 & (x))) >> 11)
#define GET_C2(x) GET_C2_SUB((int32_t)x)


void Core::examine_op(void) {
	switch((OP>>29)&7) {
	case 0:
		switch((OP>>26)&7) {
		case 0:
			OP_TYPE = ADD_L;
			OP_EX = &Core::op_add;
			OP_PRNT = &Core::prnt_add;
			R0 = GET_R0(OP);
			R1 = GET_R1(OP);
			R2 = GET_R2(OP);
			return;
		case 1:
			OP_TYPE = ADDI_L;
			OP_EX = &Core::op_addi;
			OP_PRNT = &Core::prnt_addi;
			R0 = GET_R0(OP);
			R1 = GET_R1(OP);
			C = (int16_t)OP;
			return;
		case 2:
			OP_TYPE = SUB_L;
			OP_EX = &Core::op_sub;
			OP_PRNT = &Core::prnt_sub;
			R0 = GET_R0(OP);
			R1 = GET_R2(OP);  //R1とR2の位置が逆
			R2 = GET_R1(OP);
			return;
		case 3:
			OP_TYPE = NEXT_L;
			OP_EX = &Core::op_next;
			OP_PRNT = &Core::prnt_next;
			R0 = GET_R0(OP);
			return;
		case 4:
			OP_TYPE = SL2ADD_L;
			OP_EX = &Core::op_sl2add;
			OP_PRNT = &Core::prnt_sl2add;
			R0 = GET_R0(OP);
			R1 = GET_R1(OP);
			R2 = GET_R2(OP);
			return;
		case 5:
			OP_TYPE = SL2ADDI_L;
			OP_EX = &Core::op_sl2addi;
			OP_PRNT = &Core::prnt_sl2addi;
			R0 = GET_R0(OP);
			R1 = GET_R1(OP);
			C = (int16_t)OP;
			return;
		case 6:
			OP_TYPE = MOV_L;
			OP_EX = &Core::op_mov;
			OP_PRNT = &Core::prnt_mov;
			R0 = GET_R0(OP);
			R1 = GET_R1(OP);
			return;
		case 7:
			OP_TYPE = MOVI_L;
			OP_EX = &Core::op_movi;
			OP_PRNT = &Core::prnt_movi;
			R0 = GET_R0(OP);
			C = (int16_t)OP;
			return;
		}
		break;
	case 1:
		switch((OP>>26)&7) {
		case 0:
			OP_TYPE = FADD_L;
			OP_EX = &Core::op_fadd;
			OP_PRNT = &Core::prnt_fadd;
			R0 = GET_R0(OP);
			R1 = GET_R1(OP);
			R2 = GET_R2(OP);
			return;
		case 1:
			OP_TYPE = FSUB_L;
			OP_EX = &Core::op_fsub;
			OP_PRNT = &Core::prnt_fsub;
			R0 = GET_R0(OP);
			R1 = GET_R1(OP);
			R2 = GET_R2(OP);
			return;
		case 2:
			OP_TYPE = FMUL_L;
			OP_EX = &Core::op_fmul;
			OP_PRNT = &Core::prnt_fmul;
			R0 = GET_R0(OP);
			R1 = GET_R1(OP);
			R2 = GET_R2(OP);
			return;
		case 3:
			OP_TYPE = FDIV_L;
			OP_EX = &Core::op_fdiv;
			OP_PRNT = &Core::prnt_fdiv;
			R0 = GET_R0(OP);
			R1 = GET_R1(OP);
			R2 = GET_R2(OP);
			return;
		case 4:
			OP_TYPE = FMOV_L;
			OP_EX = &Core::op_fmov;
			OP_PRNT = &Core::prnt_fmov;
			R0 = GET_R0(OP);
			R1 = GET_R1(OP);
			return;
		case 5:
			OP_TYPE = FNEG_L;
			OP_EX = &Core::op_fneg;
			OP_PRNT = &Core::prnt_fneg;
			R0 = GET_R0(OP);
			R1 = GET_R1(OP);
			return;
		case 6:
			OP_TYPE = FABS_L;
			OP_EX = &Core::op_fabs;
			OP_PRNT = &Core::prnt_fabs;
			R0 = GET_R0(OP);
			R1 = GET_R1(OP);
			return;
		case 7:
			OP_TYPE = FSQRT_L;
			OP_EX = &Core::op_fsqrt;
			OP_PRNT = &Core::prnt_fsqrt;
			R0 = GET_R0(OP);
			R1 = GET_R1(OP);
			return;
		}
		break;
	case 2:
		switch((OP>>26)&7) {
		case 0:
			OP_TYPE = LW_L;
			OP_EX = &Core::op_lw;
			OP_PRNT = &Core::prnt_lw;
			R0 = GET_R0(OP);
			R1 = GET_R1(OP);
			C = (int16_t)OP;
			return;
		case 1:
			OP_TYPE = LWI_L;
			OP_EX = &Core::op_lwi;
			OP_PRNT = &Core::prnt_lwi;
			R0 = GET_R0(OP);
			C = GET_C_LWI(OP);
			return;
		case 2:
			OP_TYPE = FLW_L;
			OP_EX = &Core::op_flw;
			OP_PRNT = &Core::prnt_flw;
			R0 = GET_R0(OP);
			R1 = GET_R1(OP);
			C = (int16_t)OP;
			return;
		case 3:
			OP_TYPE = FLWI_L;
			OP_EX = &Core::op_flwi;
			OP_PRNT = &Core::prnt_flwi;
			R0 = GET_R0(OP);
			C = GET_C_LWI(OP);
			return;
		case 4:
			OP_TYPE = SW_L;
			OP_EX = &Core::op_sw;
			OP_PRNT = &Core::prnt_sw;
			R1 = GET_R1(OP);
			R2 = GET_R2(OP);
			C = GET_C_SW(OP);
			return;
		case 5:
			OP_TYPE = SWI_L;
			OP_EX = &Core::op_swi;
			OP_PRNT = &Core::prnt_swi;
			R2 = GET_R2(OP);
			C = GET_C_SWI(OP);
			return;
		case 6:
			OP_TYPE = FSW_L;
			OP_EX = &Core::op_fsw;
			OP_PRNT = &Core::prnt_fsw;
			R1 = GET_R1(OP);
			R2 = GET_R2(OP);
			C = GET_C_SW(OP);
			return;
		case 7:
			OP_TYPE = FSWI_L;
			OP_EX = &Core::op_fswi;
			OP_PRNT = &Core::prnt_fswi;
			R2 = GET_R2(OP);
			C = GET_C_SWI(OP);
			return;
		}
		break;
	case 3:
		switch((OP>>26)&7) {
		case 0:
			OP_TYPE = FTOI_L;
			OP_EX = &Core::op_ftoi;
			OP_PRNT = &Core::prnt_ftoi;
			R0 = GET_R0(OP);
			R1 = GET_R1(OP);
			return;
		case 1:
			OP_TYPE = ITOF_L;
			OP_EX = &Core::op_itof;
			OP_PRNT = &Core::prnt_itof;
			R0 = GET_R0(OP);
			R1 = GET_R1(OP);
			return;
		case 2:
			OP_TYPE = IN_L;
			OP_EX = &Core::op_in;
			OP_PRNT = &Core::prnt_in;
			R0 = GET_R0(OP);
			return;
		case 3:
			OP_TYPE = FIN_L;
			OP_EX = &Core::op_fin;
			OP_PRNT = &Core::prnt_fin;
			R0 = GET_R0(OP);
			return;
		case 4:
			OP_TYPE = OUT_L;
			OP_EX = &Core::op_out;
			OP_PRNT = &Core::prnt_out;
			R1 = GET_R1(OP);
			return;
		case 5:
			OP_TYPE = JR_L;
			OP_EX = &Core::op_jr;
			OP_PRNT = &Core::prnt_jr;
			return;
		case 6:
			OP_TYPE = ACC_L;
			OP_EX = &Core::op_acc;
			OP_PRNT = &Core::prnt_acc;
			switch((OP>>21)&7) {
			case 1:
				R0 = 29;
				break;
			case 2:
				R0 = 30;
				break;
			case 4:
				R0 = 31;
				break;
			default:
				fprintf(stderr, "examine_op: acc: a0 invalid\n");
				STOP = 1;
			}
			R1 = GET_R1(OP);
			return;
		case 7:
			if (!PARALLEL) {
				OP_TYPE = FORK_L;
				OP_EX = &Core::op_fork;
				OP_PRNT = &Core::prnt_fork;
				R1 = GET_R1(OP);
				R2 = GET_R2(OP);
			} else {
				if (!ended) {
					OP_TYPE = END_L;
					OP_EX = &Core::op_end;
					OP_PRNT = &Core::prnt_end;
				} else {
					OP_TYPE = ENDED_L;
					OP_EX = &Core::op_ended;
					OP_PRNT = &Core::prnt_ended;
				}
			}
			return;
		}
		break;
	default:
		switch((OP>>28)&7) {
		case 0:
			OP_TYPE = J_L;
			OP_EX = &Core::op_j;
			OP_PRNT = &Core::prnt_j;
			C = GET_C_J(OP);
			return;
		case 1:
			OP_TYPE = JAL_L;
			OP_EX = &Core::op_jal;
			OP_PRNT = &Core::prnt_jal;
			C = GET_C_J(OP);
			return;
		case 2:
			OP_TYPE = FBZ_L;
			OP_EX = &Core::op_fbz;
			OP_PRNT = &Core::prnt_fbz;
			R1 = GET_R1(OP);
			C = GET_C_J(OP);
			return;
		case 3:
			OP_TYPE = FBLE_L;
			OP_EX = &Core::op_fble;
			OP_PRNT = &Core::prnt_fble;
			R1 = GET_R1(OP);
			R2 = GET_R2(OP);
			C = GET_C_J(OP);
			return;
		case 4:
			OP_TYPE = BE_L;
			OP_EX = &Core::op_be;
			OP_PRNT = &Core::prnt_be;
			R1 = GET_R1(OP);
			R2 = GET_R2(OP);
			C = GET_C_J(OP);
			return;
		case 5:
			OP_TYPE = BEI_L;
			OP_EX = &Core::op_bei;
			OP_PRNT = &Core::prnt_bei;
			R1 = GET_R1(OP);
			C2 = GET_C2(OP);
			C = GET_C_J(OP);
			return;
		case 6:
			OP_TYPE = BLE_L;
			OP_EX = &Core::op_ble;
			OP_PRNT = &Core::prnt_ble;
			R1 = GET_R1(OP);
			R2 = GET_R2(OP);
			C = GET_C_J(OP);
			return;
		case 7:
			OP_TYPE = BLEI_L;
			OP_EX = &Core::op_blei;
			OP_PRNT = &Core::prnt_blei;
			R1 = GET_R1(OP);
			C2 = GET_C2(OP);
			C = GET_C_J(OP);
			return;
		}
	}
	fprintf(stderr, "unknown operator\n");
	STOP = 1;
	return;
}


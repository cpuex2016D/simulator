#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

extern uint32_t PC;
extern uint32_t GPR[];
extern float FPR[];
extern uint32_t OP;
extern uint32_t *TEX;
extern uint32_t *DAT;
extern unsigned char FPCC;
extern FILE *IFILE;

typedef struct {
	int (*is_op) (void);
	void (*op) (void);
} op_set;

#define R_MASK 0xFC00003F
#define IJ_MASK 0xFC000000
#define FL_MASK 0xFFE00000

int is_add(void) {
	return (OP & R_MASK) == 0x00000020;
}
int is_addi(void) {
	return (OP & IJ_MASK) == 0x20000000;
}
int is_sub(void) {
	return (OP & R_MASK) == 0x00000022;
}
int is_and(void) {
	return (OP & R_MASK) == 0x00000024;
}
int is_andi(void) {
	return (OP & IJ_MASK) == 0x30000000;
}
int is_or(void) {
	return (OP & R_MASK) == 0x00000025;
}
int is_ori(void) {
	return (OP & IJ_MASK) == 0x34000000;
}
int is_nor(void) {
	return (OP & R_MASK) == 0x00000027;
}
int is_sll(void) {
	return (OP & R_MASK) == 0x00000000;
}
int is_srl(void) {
	return (OP & R_MASK) == 0x00000002;
}
int is_slt(void) {
	return (OP & R_MASK) == 0x0000002A;
}
int is_slti(void) {
	return (OP & IJ_MASK) == 0x28000000;
}
int is_beq(void) {
	return (OP & IJ_MASK) == 0x10000000;
}
int is_bne(void) {
	return (OP & IJ_MASK) == 0x14000000;
}
int is_j(void) {
    return (OP & IJ_MASK) == 0x08000000;
}
int is_jal(void) {
	return (OP & IJ_MASK) == 0x0C000000;
}
int is_jr(void) {
	return (OP & R_MASK) == 0x00000008;
}
int is_jalr(void) {
	return (OP & R_MASK) == 0x00000009;
}
int is_lw(void) {
	return (OP & IJ_MASK) == 0x8C000000;
}
int is_sw(void) {
	return (OP & IJ_MASK) == 0xAC000000;
}
int is_in(void) {
	return (OP & IJ_MASK) == 0x68000000;
}
int is_out(void) {
	return (OP & IJ_MASK) == 0x6C000000;
}
int is_bt_s(void) {
	return (OP & (FL_MASK|0x00030000)) == 0x45010000;
}
int is_bf_s(void) {
	return (OP & (FL_MASK|0x00030000)) == 0x45000000;
}
int is_add_s(void) {
	return (OP & (FL_MASK|0x0000003F)) == 0x46000000;
}
int is_sub_s(void) {
	return (OP & (FL_MASK|0x0000003F)) == 0x46000001;
}
int is_mul_s(void) {
    return (OP & (FL_MASK|0x0000003F)) == 0x46000002;
}

int is_div_s(void) {
    return (OP & (FL_MASK|0x0000003F)) == 0x46000003;
}
/*
// replaced below with above
int is_inv_s(void) {
    return (OP & (FL_MASK|0x0000003F)) == 0x46000003;
}
*/
int is_c_s(void) {
	return (OP & (FL_MASK|0x00000030)) == 0x46000030;
}
int is_lw_s(void) {
	return (OP & IJ_MASK) == 0xC4000000;
}
int is_sw_s(void) {
	return (OP & IJ_MASK) == 0xE4000000;
}


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

void op_add(void) {
	int rs = GET_RS(OP), rt = GET_RT(OP), rd = GET_RD(OP);
	GPR[rd] = GPR[rs] + GPR[rt];
	return;
}

void op_addi(void) {
	int rs = GET_RS(OP), rt = GET_RT(OP), c = GET_SC(OP);
	GPR[rt] = GPR[rs] + ((c&0x8000?0xFFFF0000:0) | c);
	return;
}

void op_sub(void) {
	int rs = GET_RS(OP), rt = GET_RT(OP), rd = GET_RD(OP);
	GPR[rd] = GPR[rs] - GPR[rt];
	return;
}

void op_and(void) {
	int rs = GET_RS(OP), rt = GET_RT(OP), rd = GET_RD(OP);
	GPR[rd] = GPR[rs] & GPR[rt];
	return;
}

void op_andi(void) {
	int rs = GET_RS(OP), rt = GET_RT(OP), c = GET_SC(OP);
	GPR[rt] = GPR[rs] & c;
	return;
}

void op_or(void) {
	int rs = GET_RS(OP), rt = GET_RT(OP), rd = GET_RD(OP);
	GPR[rd] = GPR[rs] | GPR[rt];
	return;
}

void op_ori(void) {
	int rs = GET_RS(OP), rt = GET_RT(OP), c = GET_SC(OP);
	GPR[rt] = GPR[rs] | c;
	return;
}

void op_nor(void) {
	int rs = GET_RS(OP), rt = GET_RT(OP), rd = GET_RD(OP);
	GPR[rd] = ~(GPR[rs] | GPR[rt]);
	return;
}

void op_sll(void) {
	int rt = GET_RT(OP), rd = GET_RD(OP), c = GET_SSC(OP);
	GPR[rd] = GPR[rt] << c;
	return;
}

void op_srl(void) {
	int rt = GET_RT(OP), rd = GET_RD(OP), c = GET_SSC(OP);
	GPR[rd] = GPR[rt] >> c;
	return;
}

void op_slt(void) {
	int rs = GET_RS(OP), rt = GET_RT(OP), rd = GET_RD(OP);
	GPR[rd] = (int32_t)GPR[rs] <= (int32_t)GPR[rt];
	return;
}

void op_slti(void) {
	int rs = GET_RS(OP), rt = GET_RT(OP), c = GET_SC(OP);
	GPR[rt] = (int32_t)GPR[rs] <= (int32_t)((c&0x8000?0xFFFF0000:0) | c);
	return;
}

void op_beq(void) {
	int rs = GET_RS(OP), rt = GET_RT(OP), c = GET_SC(OP);
	if (GPR[rs] == GPR[rt]) {
		PC += ((c&0x8000?0xFFFF0000:0) | c) - 1;
	}
	return;
}

void op_bne(void) {
	int rs = GET_RS(OP), rt = GET_RT(OP), c = GET_SC(OP);
	if (GPR[rs] != GPR[rt]){
		PC += ((c&0x8000?0xFFFF0000:0) | c) - 1;
	}
	return;
}

void op_j(void) {
	int c = GET_LC(OP);
	PC = c;
	return;
}

void op_jal(void) {
	int c = GET_LC(OP);
	GPR[31] = PC; // PC is already incremented before
	PC = c;
	return;
}

void op_jr(void) {
	int rs = GET_RS(OP);
	PC = GPR[rs];
	return;
}

void op_jalr(void) {
	int rs = GET_RS(OP), temp;
	temp = PC;
	PC = GPR[rs];
	GPR[31] = temp;
	return;
}

void op_lw(void) {
	int rs = GET_RS(OP), rt = GET_RT(OP), c = GET_SC(OP);
	int addr;
	addr = GPR[rs]+((c&0x8000?0xFFFF0000:0) | c);
	if (!(0 <= addr && addr < 0x80000)) {
		fprintf(stderr, "overflow: trying to lw from %X\n", addr);
		exit(1);
	}
	GPR[rt] = DAT[addr];
	return;
}

void op_sw(void) {
	int rs = GET_RS(OP), rt = GET_RT(OP), c = GET_SC(OP);
	int addr;
	addr = GPR[rs]+((c&0x8000?0xFFFF0000:0) | c);
	if (!(0 <= addr && addr < 0x80000)) {
		fprintf(stderr, "overflow: trying to sw to %X\n", addr);
		exit(1);
	}
	DAT[addr] = GPR[rt];
	return;
}

void op_in(void) {
	int rt = GET_RT(OP), inp;
	fflush(stdout);
	/*
	fprintf(stderr, ">");
	fread(&GPR[rt], 1, 1, stdin);
	fprintf(stderr, "\n");
	*/
	inp = getc(IFILE);
	if (inp == EOF) {
		fprintf(stderr, "PC: %d\tOP:%08X\tFPCC: %02X\nGPR\n0-3\t%d\t%d\t%d\t%d\n4-7\t%d\t%d\t%d\t%d\n8-11\t%d\t%d\t%d\t%d\n12-15\t%d\t%d\t%d\t%d\n16-19\t%d\t%d\t%d\t%d\n20-23\t%d\t%d\t%d\t%d\n24-27\t%d\t%d\t%d\t%d\n28-31\t%d\t%d\t%d\t%d\nFPR\n0-3\t%f\t%f\t%f\t%f\n4-7\t%f\t%f\t%f\t%f\n8-11\t%f\t%f\t%f\t%f\n12-15\t%f\t%f\t%f\t%f\n16-19\t%f\t%f\t%f\t%f\n20-23\t%f\t%f\t%f\t%f\n24-27\t%f\t%f\t%f\t%f\n28-31\t%f\t%f\t%f\t%f\n\n\n", PC-1, OP, FPCC, GPR[0], GPR[1], GPR[2], GPR[3], GPR[4], GPR[5], GPR[6], GPR[7], GPR[8], GPR[9], GPR[10], GPR[11], GPR[12], GPR[13], GPR[14], GPR[15], GPR[16], GPR[17], GPR[18], GPR[19], GPR[20], GPR[21], GPR[22], GPR[23], GPR[24], GPR[25], GPR[26], GPR[27], GPR[28], GPR[29], GPR[30], GPR[31], FPR[0], FPR[1], FPR[2], FPR[3], FPR[4], FPR[5], FPR[6], FPR[7], FPR[8], FPR[9], FPR[10], FPR[11], FPR[12], FPR[13], FPR[14], FPR[15], FPR[16], FPR[17], FPR[18], FPR[19], FPR[20], FPR[21], FPR[22], FPR[23], FPR[24], FPR[25], FPR[26], FPR[27], FPR[28], FPR[29], FPR[30], FPR[31]);
		exit(0);
	}
	*((unsigned char*)&GPR[rt]) = (unsigned char) inp;
	return;
}

void op_out(void) {
	int rt = GET_RT(OP);
	fwrite((char*)&GPR[rt], 1, 1, stdout);
	return;
}

void op_bt_s(void) {
	int cc = GET_BCC(OP), c = GET_SC(OP);
	if (FPCC&(1<<cc)) {
		PC += ((c&0x8000?0xFFFF0000:0) | c) - 1;
	}
	return;
}

void op_bf_s(void) {
	int cc = GET_BCC(OP), c = GET_SC(OP);
	if (!(FPCC&(1<<cc))) {
		PC += ((c&0x8000?0xFFFF0000:0) | c) - 1;
	}
	return;
}

void op_add_s(void) {
	int ft = GET_FT(OP), fs = GET_FS(OP), fd = GET_FD(OP);
	FPR[fd] = FPR[fs] + FPR[ft];
	return;
}

void op_sub_s(void) {
	int ft = GET_FT(OP), fs = GET_FS(OP), fd = GET_FD(OP);
	FPR[fd] = FPR[fs] - FPR[ft];
	return;
}

void op_mul_s(void) {
	int ft = GET_FT(OP), fs = GET_FS(OP), fd = GET_FD(OP);
	FPR[fd] = FPR[fs] * FPR[ft];
	return;
}

void op_div_s(void) {
	int ft = GET_FT(OP), fs = GET_FS(OP), fd = GET_FD(OP);
	FPR[fd] = FPR[fs] / FPR[ft];
	return;
}

/*
// change below to div func (above)
void op_inv_s(void) {
	int ft = GET_FT(OP), fd = GET_FD(OP);
	FPR[fd] = 1.0/FPR[ft];
	return;
}
*/


void op_c_s(void) {
	int ft = GET_FT(OP), fs = GET_FS(OP), cc = GET_CCC(OP), cond = GET_COND(OP);
	FPCC |= 1 << cc;
	if (cond == 2) {
		if (!(FPR[fs] == FPR[ft])) FPCC ^= 1 << cc;
	} else if (cond == 12) {
		if (!(FPR[fs] < FPR[ft])) FPCC ^= 1 << cc;
	} else if (cond == 14) {
		if (!(FPR[fs] <= FPR[ft])) FPCC ^= 1 << cc;
	} else {
		fprintf(stderr, "undefined c._.s operation: %d\n", cond);
		exit(1);
	}
	return;
}

void op_lw_s(void) {
	int rs = GET_RS(OP), ft = GET_FT(OP), c = GET_SC(OP);
	int addr;
	addr = GPR[rs]+((c&0x8000?0xFFFF0000:0) | c);
	if (!(0 <= addr && addr < 0x80000)) {
		fprintf(stderr, "overflow: trying to lw_s from %X\n", addr);
		exit(1);
	}
	FPR[ft] = *((float*)&DAT[addr]);
	return;
}

void op_sw_s(void) {
	int rs = GET_RS(OP), ft = GET_FT(OP), c = GET_SC(OP);
	int addr;
	addr = GPR[rs]+((c&0x8000?0xFFFF0000:0) | c);
	if (!(0 <= addr && addr < 0x80000)) {
		fprintf(stderr, "overflow: trying to sw_s to %X\n", addr);
		exit(1);
	}
	*((float*)&DAT[addr]) = FPR[ft];
	return;
}

/* code below is added in accordance with newly added ops. */

int is_lui(void) {
	return (OP & IJ_MASK) == 0x3C000000;
}

void op_lui(void) {
	int rt = GET_RT(OP), c = GET_SC(OP);
	GPR[rt] = c << 16;
	return;
}

int is_mov_s(void) {
	return (OP & (FL_MASK|0x0000003F)) == 0x46000006;
}

int is_neg_s(void) {
	return (OP & (FL_MASK|0x0000003F)) == 0x46000007;
}

void op_mov_s(void) {
	int ft = GET_FT(OP), fd = GET_FD(OP);
	FPR[fd] = FPR[ft];
	return;
}

void op_neg_s(void) {
	int ft = GET_FT(OP), fd = GET_FD(OP);
	FPR[fd] = -FPR[ft];
	return;
}

/* added below (12/1) */
int is_abs_s(void) {
	return (OP & (FL_MASK|0x0000003F)) == 0x46000005;
}

int is_sqrt_s(void) {
	return (OP & (FL_MASK|0x0000003F)) == 0x46000004;
}

void op_abs_s(void) {
	int ft = GET_FT(OP), fd = GET_FD(OP);
	FPR[fd] = (FPR[ft] < 0) ? -FPR[ft] : FPR[ft];
	return;
}

void op_sqrt_s(void) {
	int ft = GET_FT(OP), fd = GET_FD(OP);
	FPR[fd] = (float)sqrt((double)FPR[ft]);
	return;
}

int is_ftoi(void) {
	return (OP & IJ_MASK) == 0xE0000000;
}

int is_itof(void) {
	return (OP & IJ_MASK) == 0xC0000000;
}

void op_ftoi(void) {
	int fs = GET_RS(OP), rt = GET_RT(OP);
	GPR[rt] = (int)FPR[fs];
	return;
}

void op_itof(void) {
	int rs = GET_RS(OP), ft = GET_RT(OP);
	FPR[ft] = (float)GPR[rs];
	return;
}


op_set op_array[] = {{is_add, op_add}, {is_addi, op_addi}, {is_sub, op_sub}, {is_and, op_and}, {is_andi, op_andi}, {is_or, op_or}, {is_ori, op_ori}, {is_nor, op_nor}, {is_sll, op_sll}, {is_srl, op_srl}, {is_slt, op_slt}, {is_slti, op_slti}, {is_beq, op_beq}, {is_bne, op_bne}, {is_j, op_j}, {is_jal, op_jal}, {is_jr, op_jr}, {is_jalr, op_jalr}, {is_lw, op_lw}, {is_sw, op_sw}, {is_in, op_in}, {is_out, op_out}, {is_bt_s, op_bt_s}, {is_bf_s, op_bf_s}, {is_add_s, op_add_s}, {is_sub_s, op_sub_s}, {is_mul_s, op_mul_s}, {is_div_s, op_div_s}, /*{is_inv_s, op_inv_s}, */{is_c_s, op_c_s}, {is_lw_s, op_lw_s}, {is_sw_s, op_sw_s}, {is_lui, op_lui}, {is_mov_s, op_mov_s}, {is_neg_s, op_neg_s}, {is_abs_s, op_abs_s}, {is_sqrt_s, op_sqrt_s}, {is_ftoi, op_ftoi}, {is_itof, op_itof}, {NULL, NULL}};

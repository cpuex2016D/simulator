#include<stdint.h>
#include<stdio.h>
extern uint32_t PC;
extern uint32_t GPR[];
extern float FPR[];
extern uint32_t OP;
extern void *MEM;
extern unsigned char FPCC;


#define R_MASK 0xFC00003F
#define IJ_MASK 0xFC000000
#define FL_MASK 0xFFE00000


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

int op_add(void) {
	int rs = GET_RS(OP), rt = GET_RT(OP), rd = GET_RD(OP);
	if ((OP & R_MASK) != 0x00000020) return 0;
	GPR[rd] = GPR[rs] + GPR[rt];
	return 1;
}

int op_addi(void) {
	int rs = GET_RS(OP), rt = GET_RT(OP), c = GET_SC(OP);
	if ((OP & IJ_MASK) != 0x20000000) return 0;
	GPR[rt] = GPR[rs] + ((c&0x8000?0xFFFF0000:0) | c);
	return 1;
}

int op_sub(void) {
	int rs = GET_RS(OP), rt = GET_RT(OP), rd = GET_RD(OP);
	if ((OP & R_MASK) != 0x00000022) return 0;
	GPR[rd] = GPR[rs] - GPR[rt];
	return 1;
}

int op_and(void) {
	int rs = GET_RS(OP), rt = GET_RT(OP), rd = GET_RD(OP);
	if ((OP & R_MASK) != 0x00000024) return 0;
	GPR[rd] = GPR[rs] & GPR[rt];
	return 1;
}

int op_andi(void) {
	int rs = GET_RS(OP), rt = GET_RT(OP), c = GET_SC(OP);
	if ((OP & IJ_MASK) != 0x30000000) return 0;
	GPR[rt] = GPR[rs] & c;
	return 1;
}

int op_or(void) {
	int rs = GET_RS(OP), rt = GET_RT(OP), rd = GET_RD(OP);
	if ((OP & R_MASK) != 0x00000025) return 0;
	GPR[rd] = GPR[rs] | GPR[rt];
	return 1;
}

int op_ori(void) {
	int rs = GET_RS(OP), rt = GET_RT(OP), c = GET_SC(OP);
	if ((OP & IJ_MASK) != 0x34000000) return 0;
	GPR[rt] = GPR[rs] | c;
	return 1;
}

int op_nor(void) {
	int rs = GET_RS(OP), rt = GET_RT(OP), rd = GET_RD(OP);
	if ((OP & R_MASK) != 0x00000027) return 0;
	GPR[rd] = ~(GPR[rs] | GPR[rt]);
	return 1;
}

int op_sll(void) {
	int rt = GET_RT(OP), rd = GET_RD(OP), c = GET_SSC(OP);
	if ((OP & R_MASK) != 0x00000000) return 0;
	GPR[rd] = GPR[rt] << c;
	return 1;
}

int op_srl(void) {
	int rt = GET_RT(OP), rd = GET_RD(OP), c = GET_SSC(OP);
	if ((OP & R_MASK) != 0x00000002) return 0;
	GPR[rd] = GPR[rt] >> c;
	return 1;
}

int op_slt(void) {
	int rs = GET_RS(OP), rt = GET_RT(OP), rd = GET_RD(OP);
	if ((OP & R_MASK) != 0x0000002A) return 0;
	GPR[rd] = GPR[rs] < GPR[rt];
	return 1;
}

int op_slti(void) {
	int rs = GET_RS(OP), rt = GET_RT(OP), c = GET_SC(OP);
	if ((OP & IJ_MASK) != 0x28000000) return 0;
	GPR[rt] = GPR[rs] < c;
	return 1;
}

int op_beq(void) {
	int rs = GET_RS(OP), rt = GET_RT(OP), c = GET_SC(OP);
	if ((OP & IJ_MASK) != 0x10000000) return 0;
	if (GPR[rs] == GPR[rt])
		PC += 4*c;
	return 1;
}

int op_bne(void) {
	int rs = GET_RS(OP), rt = GET_RT(OP), c = GET_SC(OP);
	if ((OP & IJ_MASK) != 0x14000000) return 0;
	if (GPR[rs] != GPR[rt])
		PC += 4*c;
	return 1;
}

int op_j(void) {
	int c = GET_LC(OP);
	if ((OP & IJ_MASK) != 0x08000000) return 0;
	PC = c * 4;
	return 1;
}

int op_jal(void) {
	int c = GET_LC(OP);
	if ((OP & IJ_MASK) != 0x0C000000) return 0;
	GPR[31] = PC + 4;
	PC = c * 4;
	return 1;
}

int op_jr(void) {
	int rs = GET_RS(OP);
	if ((OP & R_MASK) != 0x00000008) return 0;
	PC = GPR[rs];
	return 1;
}

int op_jalr(void) {
	int rs = GET_RS(OP);
	if ((OP & R_MASK) != 0x00000009) return 0;
	GPR[31] = PC + 4;
	PC = GPR[rs];
	return 1;
}

int op_lw(void) {
	int rs = GET_RS(OP), rt = GET_RT(OP), c = GET_SC(OP);
	if ((OP & IJ_MASK) != 0x8C000000) return 0;
	GPR[rt] = *((uint32_t*)(MEM+GPR[rs]+c));
	return 1;
}

int op_sw(void) {
	int rs = GET_RS(OP), rt = GET_RT(OP), c = GET_SC(OP);
	if ((OP & IJ_MASK) != 0xAC000000) return 0;
	*((uint32_t*)(MEM+GPR[rs]+c)) = GPR[rt];
	return 1;
}

int op_in(void) {
	int rt = GET_RT(OP);
	if ((OP & IJ_MASK) != 0x68000000) return 0;
	fread(&GPR[rt], 4, 1, stdin);
	return 1;
}

int op_out(void) {
	int rt = GET_RT(OP);
	if ((OP & IJ_MASK) != 0x69000000) return 0;
	fwrite(&GPR[rt], 4, 1, stdout);
	return 1;
}

int op_bt_s(void) {
	int cc = GET_BCC(OP), c = GET_SC(OP);
	if ((OP & (FL_MASK|0x00030000)) != 0x45010000) return 0;
	if (FPCC&(1<<cc))
		PC += 4*c;
	return 1;
}

int op_bf_s(void) {
	int cc = GET_BCC(OP), c = GET_SC(OP);
	if ((OP & (FL_MASK|0x00030000)) != 0x45000000) return 0;
	if (!(FPCC&(1<<cc)))
		PC += 4*c;
	return 1;
}

int op_add_s(void) {
	int ft = GET_FT(OP), fs = GET_FS(OP), fd = GET_FD(OP);
	if ((OP & (FL_MASK|0x0000003F)) != 0x46000000) return 0;
	FPR[fd] = FPR[fs] + FPR[ft];
	return 1;
}

int op_sub_s(void) {
	int ft = GET_FT(OP), fs = GET_FS(OP), fd = GET_FD(OP);
	if ((OP & (FL_MASK|0x0000003F)) != 0x46000001) return 0;
	FPR[fd] = FPR[fs] - FPR[ft];
	return 1;
}

int op_mul_s(void) {
	int ft = GET_FT(OP), fs = GET_FS(OP), fd = GET_FD(OP);
	if ((OP & (FL_MASK|0x0000003F)) != 0x46000002) return 0;
	FPR[fd] = FPR[fs] * FPR[ft];
	return 1;
}

int op_inv_s(void) {
	int ft = GET_FT(OP), fd = GET_FD(OP);
	if ((OP & (FL_MASK|0x0000003F)) != 0x46000003) return 0;
	FPR[fd] = 1.0/FPR[ft];
	return 1;
}

int op_c_s(void) {
	int ft = GET_FT(OP), fs = GET_FS(OP), cc = GET_CCC(OP), cond = GET_COND(OP);
	if ((OP & (FL_MASK|0x00000030)) != 0x46000030) return 0;
	FPCC |= 1 << cc;
	if (cond == 0x2) {
		if (!(FPR[fs] == FPR[ft])) FPCC ^= 1 << cc;
	} else if (cond == 0xC) {
		if (!(FPR[fs] < FPR[ft])) FPCC ^= 1 << cc;
	} else if (cond == 0xE) {
		if (!(FPR[fs] <= FPR[ft])) FPCC ^= 1 << cc;
	}
	return 1;
}

int op_lw_s(void) {
	int rs = GET_RS(OP), ft = GET_FT(OP), c = GET_SC(OP);
	if ((OP & IJ_MASK) != 0xC4000000) return 0;
	FPR[ft] = *((float*)(MEM+GPR[rs]+c));
	return 1;
}

int op_sw_s(void) {
	int rs = GET_RS(OP), ft = GET_FT(OP), c = GET_SC(OP);
	if ((OP & IJ_MASK) != 0xE4000000) return 0;
	*((float*)(MEM+GPR[rs]+c)) = FPR[ft];
	return 1;
}

int (*op_array[]) (void) = {op_add, op_addi, op_sub, op_and, op_andi, op_or, op_ori, op_nor, op_sll, op_srl, op_slt, op_slti, op_beq, op_bne, op_j, op_jal, op_jr, op_jalr, op_lw, op_sw, op_in, op_out, op_bt_s, op_bf_s, op_add_s, op_sub_s, op_mul_s, op_inv_s, op_c_s, op_lw_s, op_sw_s, NULL};



#include<cstdlib>
#include<map>
#include<string>
#include<stdint.h>
using namespace std;
#define MV2TKN(p) while(*p == ' ' || *p == '\t' || *p == ',') p++
#define MV2TKNB(p) while(*p == ' ' || *p == '\t') p++
#define MV2SP(p) while(*p != ' ' && *p != '\t' && *p != ',' && *p != '\n') p++
#define MV2NXTKN(p) MV2SP(p); MV2TKN(p)
#define MV2NXTKNB(p) MV2SP(p); MV2TKNB(p)
#define GO2EL(p) while(*p != '\n') p++


#define STORE_OP \
	*((uint32_t *)&TEX[TA*4]) = op; \
	TA += 1

#define TECHK if (*p != ' ' && *p != '\t' && *p != ',' && *p != '\n' && *p != ')') throw "invalid arguments"
#define NLCHK if (*p != '\n') throw "too many tokens"


#define GETRN(reg) \
	if (*(p++) != '%' || *(p++) != 'r') throw "not valid register"; \
	if ('0' == *p || ('4' <= *p && *p <= '9')) { \
		reg = *p - '0'; \
		p++; \
		TECHK; \
	} else if (*p == '1' || *p == '2') { \
		p++; \
		if ('0' <= *p && *p <= '9') { \
			reg = (*(p-1) - '0') * 10 + *p - '0'; \
			p++; \
			TECHK; \
		} else { \
			reg = *(p-1) - '0'; \
			TECHK; \
		} \
	} else if (*p == '3') { \
		p++; \
		if ('0' == *p || *p == '1') { \
			reg = 30 + *p - '0'; \
			p++; \
			TECHK; \
		} else { \
			reg = 3; \
			TECHK; \
		} \
	} else throw "invalid arguments"
		

#define GETFN(reg) \
	if (*(p++) != '%' || *(p++) != 'f') throw "not valid register"; \
	if ('0' == *p || ('4' <= *p && *p <= '9')) { \
		reg = *p - '0'; \
		p++; \
		TECHK; \
	} else if (*p == '1' || *p == '2') { \
		p++; \
		if ('0' <= *p && *p <= '9') { \
			reg = (*(p-1) - '0') * 10 + *p - '0'; \
			p++; \
			TECHK; \
		} else { \
			reg = *(p-1) - '0'; \
			TECHK; \
		} \
	} else if (*p == '3') { \
		p++; \
		if ('0' == *p || *p == '1') { \
			reg = 30 + *p - '0'; \
			p++; \
			TECHK; \
		} else { \
			reg = 3; \
			TECHK; \
		} \
	} else throw "invalid arguments"


#define GETC(c) { \
	char *q; \
	c = (int)strtol(p, &q, 0); \
	if (p == q) throw "invalid arguments"; \
	p = q; \
}

#define GETL(c) { \
	char *q = p, tmp; \
	map<string, int>::iterator itr; \
	MV2SP(q); \
	tmp = *q; \
	*q = '\0'; \
	itr = LBM.find(p); \
	*q = tmp; \
	p = q; \
	if (itr == LBM.end()) throw "unknown lable referenced"; \
	c = itr->second; \
}

#define GETCL(c) \
	if ('0' <= *p && *p <= '9') GETC(c) \
	else GETL(c)


extern int TA, DA;
extern char *TEX, *DAT;
typedef map<string, int (*) (char *) >::iterator mi;
extern map<string, int> LBM;



void nop(char *p) {
	return;
}

void sz_gop(char *p) {
	TA += 1;
	return;
}

void op_add(char *p) {
	uint32_t op;
	char r0, r1, r2;
	GETRN(r0);
	MV2TKN(p);
	GETRN(r1);
	MV2TKN(p);
	GETRN(r2);
	MV2TKNB(p);
	NLCHK;
	op = (r0 << 21) | (r1 << 16) | (r2 << 11);
	STORE_OP;
	return;
}

void op_addi(char *p) {
	uint32_t op;
	char r0, r1;
	int c;
	GETRN(r0);
	MV2TKN(p);
	GETRN(r1);
	MV2TKN(p);
	GETC(c);
	MV2TKNB(p);
	NLCHK;
	op = 0x04000000 | (r0 << 21) | (r1 << 16) | (c & 0xffff);
	STORE_OP;
	return;
}

void op_sub(char *p) {
	uint32_t op;
	char r0, r1, r2;
	GETRN(r0);
	MV2TKN(p);
	GETRN(r1);
	MV2TKN(p);
	GETRN(r2);
	MV2TKNB(p);
	NLCHK;
	op = 0x08000000 | (r0 << 21) | (r2 << 16) | (r1 << 11);
	STORE_OP;
	return;
}

void op_subi(char *p) {
	uint32_t op;
	char r0, r2;
	int c;
	GETRN(r0);
	MV2TKN(p);
	GETC(c);
	MV2TKN(p);
	GETRN(r2);
	MV2TKNB(p);
	NLCHK;
	op = 0x0c000000 | (r0 << 21) | (r2 << 16) | (c & 0xffff);
	STORE_OP;
	return;
}

void op_sl2add(char *p) {
	uint32_t op;
	char r0, r1, r2;
	GETRN(r0);
	MV2TKN(p);
	GETRN(r1);
	MV2TKN(p);
	GETRN(r2);
	MV2TKNB(p);
	NLCHK;
	op = 0x10000000 | (r0 << 21) | (r1 << 16) | (r2 << 11);
	STORE_OP;
	return;
}

void op_sl2addi(char *p) {
	uint32_t op;
	char r0, r1;
	int c;
	GETRN(r0);
	MV2TKN(p);
	GETRN(r1);
	MV2TKN(p);
	GETC(c);
	MV2TKNB(p);
	NLCHK;
	op = 0x14000000 | (r0 << 21) | (r1 << 16) | (c & 0xffff);
	STORE_OP;
	return;
}

void op_mov(char *p) {
	uint32_t op;
	char r0, r1;
	GETRN(r0);
	MV2TKN(p);
	GETRN(r1);
	MV2TKNB(p);
	NLCHK;
	op = 0x18000000 | (r0 << 21) | (r1 << 16);
	STORE_OP;
	return;
}

void op_movi(char *p) {
	uint32_t op;
	char r0;
	int c;
	GETRN(r0);
	MV2TKN(p);
	GETC(c);
	MV2TKNB(p);
	NLCHK;
	op = 0x1c000000 | (r0 << 21) | (c & 0xffff);
	STORE_OP;
	return;
}

#define OP_FADD(opcode) \
	uint32_t op; \
	char f0, f1, f2; \
	GETFN(f0); \
	MV2TKN(p); \
	GETFN(f1); \
	MV2TKN(p); \
	GETFN(f2); \
	MV2TKNB(p); \
	NLCHK; \
	op = opcode | (f0 << 21) | (f1 << 16) | (f2 << 11); \
	STORE_OP; \
	return
void op_fadd(char *p) { OP_FADD(0x20000000); }
void op_fsub(char *p) { OP_FADD(0x24000000); }
void op_fmul(char *p) { OP_FADD(0x28000000); }
void op_fdiv(char *p) { OP_FADD(0x2c000000); }

#define OP_FMOV(opcode) \
	uint32_t op; \
	char f0, f1; \
	GETFN(f0); \
	MV2TKN(p); \
	GETFN(f1); \
	MV2TKNB(p); \
	NLCHK; \
	op = opcode | (f0 << 21) | (f1 << 16); \
	STORE_OP; \
	return
void op_fmov(char *p) { OP_FMOV(0x30000000); }
void op_fneg(char *p) { OP_FMOV(0x34000000); }
void op_fabs(char *p) { OP_FMOV(0x38000000); }
void op_fsqrt(char *p) { OP_FMOV(0x3c000000); }

void op_lw(char *p) {
	uint32_t op;
	char r0, r1;
	int c;
	GETRN(r0);
	MV2TKN(p);
	GETC(c);
	if (*(p++) != '(') throw "invalid arguments";
	GETRN(r1);
	if (*(p++) != ')') throw "invalid arguments";
	MV2TKNB(p);
	NLCHK;
	op = 0x40000000 | (r0 << 21) | (r1 << 16) | (c & 0xffff);
	STORE_OP;
	return;
}

void op_lwi(char *p) {
	uint32_t op;
	char r0;
	int c;
	GETRN(r0);
	MV2TKN(p);
	GETCL(c);
	MV2TKNB(p);
	NLCHK;
	op = 0x44000000 | (r0 << 21) | (c & 0x3ffff);
	STORE_OP;
	return;
}

void op_flw(char *p) {
	uint32_t op;
	char f0, r1;
	int c;
	GETFN(f0);
	MV2TKN(p);
	GETC(c);
	if (*(p++) != '(') throw "invalid arguments";
	GETRN(r1);
	if (*(p++) != ')') throw "invalid arguments";
	MV2TKNB(p);
	NLCHK;
	op = 0x48000000 | (f0 << 21) | (r1 << 16) | (c & 0xffff);
	STORE_OP;
	return;
}

void op_flwi(char *p) {
	uint32_t op;
	char f0;
	int c;
	GETFN(f0);
	MV2TKN(p);
	GETCL(c);
	MV2TKNB(p);
	NLCHK;
	op = 0x4c000000 | (f0 << 21) | (c & 0x3ffff);
	STORE_OP;
	return;
}

void op_sw(char *p) {
	uint32_t op;
	char r2, r1;
	int c;
	GETRN(r2);
	MV2TKN(p);
	GETC(c);
	if (*(p++) != '(') throw "invalid arguments";
	GETRN(r1);
	if (*(p++) != ')') throw "invalid arguments";
	MV2TKNB(p);
	NLCHK;
	op = 0x50000000 | ((c & 0xf800) << 10) | (r1 << 16) | (r2 << 11) | (c & 0x7ff);
	STORE_OP;
	return;
}

void op_swi(char *p) {
	uint32_t op;
	char r2;
	int c;
	GETRN(r2);
	MV2TKN(p);
	GETCL(c);
	MV2TKNB(p);
	NLCHK;
	op = 0x54000000 | ((c & 0xf800) << 10) | (c & 0x307ff) | (r2 << 11);
	STORE_OP;
	return;
}

void op_fsw(char *p) {
	uint32_t op;
	char f2, r1;
	int c;
	GETFN(f2);
	MV2TKN(p);
	GETC(c);
	if (*(p++) != '(') throw "invalid arguments";
	GETRN(r1);
	if (*(p++) != ')') throw "invalid arguments";
	MV2TKNB(p);
	NLCHK;
	op = 0x58000000 | ((c & 0xf800) << 10) | (r1 << 16) | (f2 << 11) | (c & 0x7ff);
	STORE_OP;
	return;
}

void op_fswi(char *p) {
	uint32_t op;
	char f2;
	int c;
	GETFN(f2);
	MV2TKN(p);
	GETCL(c);
	MV2TKNB(p);
	NLCHK;
	op = 0x5c000000 | ((c & 0xf800) << 10) | (c & 0x307ff) | (f2 << 11);
	STORE_OP;
	return;
}

void op_ftoi(char *p) {
	uint32_t op;
	char r0, f1;
	GETRN(r0);
	MV2TKN(p);
	GETFN(f1);
	MV2TKNB(p);
	NLCHK;
	op = 0x60000000 | (r0 << 21) | (f1 << 16);
	STORE_OP;
	return;
}

void op_itof(char *p) {
	uint32_t op;
	char f0, r1;
	GETFN(f0);
	MV2TKN(p);
	GETRN(r1);
	MV2TKNB(p);
	NLCHK;
	op = 0x64000000 | (f0 << 21) | (r1 << 16);
	STORE_OP;
	return;
}

void op_in(char *p) {
	uint32_t op;
	char r0;
	GETRN(r0);
	MV2TKNB(p);
	NLCHK;
	op = 0x68000000 | (r0 << 21);
	STORE_OP;
	return;
}

void op_fin(char *p) {
	uint32_t op;
	char f0;
	GETFN(f0);
	MV2TKNB(p);
	NLCHK;
	op = 0x6c000000 | (f0 << 21);
	STORE_OP;
	return;
}

void op_out(char *p) {
	uint32_t op;
	char r1;
	GETRN(r1);
	MV2TKNB(p);
	NLCHK;
	op = 0x70000000 | (r1 << 16);
	STORE_OP;
	return;
}

void op_jr(char *p) {
	uint32_t op;
	NLCHK;
	op = 0x74000000;
	STORE_OP;
	return;
}

void op_j(char *p) {
	uint32_t op;
	int c;
	GETCL(c);
	MV2TKNB(p);
	NLCHK;
	op = 0x80000000 | ((c & 0x3800) << 10) | (c & 0x7ff);
	STORE_OP;
	return;
}

void op_jal(char *p) {
	uint32_t op;
	int c;
	GETCL(c);
	MV2TKNB(p);
	NLCHK;
	op = 0x90000000 | ((c & 0x3800) << 10) | (c & 0x7ff);
	STORE_OP;
	return;
}

void op_fbz(char *p) {
	uint32_t op;
	char f1;
	int c;
	GETFN(f1);
	MV2TKN(p);
	GETCL(c);
	MV2TKNB(p);
	NLCHK;
	op = 0xa0000000 | ((c & 0x3800) << 10) | (f1 << 16) | (c & 0x7ff);
	STORE_OP;
	return;
}

void op_fble(char *p) {
	uint32_t op;
	char f1, f2;
	int c;
	GETFN(f1);
	MV2TKN(p);
	GETFN(f2);
	MV2TKN(p);
	GETCL(c);
	MV2TKNB(p);
	NLCHK;
	op = 0xb0000000 | ((c & 0x3800) << 10) | (f1 << 16) | (f2 << 11) | (c & 0x7ff);
	STORE_OP;
	return;
}

#define OP_BE(opcode) \
	uint32_t op; \
	char r1, r2; \
	int c; \
	GETRN(r1); \
	MV2TKN(p); \
	GETRN(r2); \
	MV2TKN(p); \
	GETCL(c); \
	MV2TKNB(p); \
	NLCHK; \
	op = opcode | ((c & 0x3800) << 10) | (r1 << 16) | (r2 << 11) | (c & 0x7ff); \
	STORE_OP; \
	return
void op_be(char *p) { OP_BE(0xc0000000); }

#define OP_BEI(opcode) \
	uint32_t op; \
	char r1; \
	int c2, c; \
	GETRN(r1); \
	MV2TKN(p); \
	GETC(c2); \
	if (!(-256 <= c2 && c2 <= 255)) throw "C2 is out of range"; \
	MV2TKN(p); \
	GETCL(c); \
	MV2TKNB(p); \
	NLCHK; \
	op = opcode | ((c2 & 0x1e0) << 19) | ((c & 0x3800) << 10) | (r1 << 16) | ((c2 & 0x1f) << 11) | (c & 0x7ff); \
	STORE_OP; \
	return
void op_bei(char *p) { OP_BEI(0xd0000000); }

void op_ble(char *p) { OP_BE(0xe0000000); }

void op_blei(char *p) { OP_BEI(0xf0000000); }




map<string, void (*) (char *) > SZM = {
	{".align", nop},
	{".global", nop},
	{"add", sz_gop},
	{"addi", sz_gop},
	{"sub", sz_gop},
	{"subi", sz_gop},
	{"sl2add", sz_gop},
	{"sl2addi", sz_gop},
	{"mov", sz_gop},
	{"movi", sz_gop},
	{"fadd", sz_gop},
	{"fsub", sz_gop},
	{"fmul", sz_gop},
	{"fdiv", sz_gop},
	{"fmov", sz_gop},
	{"fneg", sz_gop},
	{"fabs", sz_gop},
	{"fsqrt", sz_gop},
	{"lw", sz_gop},
	{"lwi", sz_gop},
	{"flw", sz_gop},
	{"flwi", sz_gop},
	{"sw", sz_gop},
	{"swi", sz_gop},
	{"fsw", sz_gop},
	{"fswi", sz_gop},
	{"ftoi", sz_gop},
	{"itof", sz_gop},
	{"in", sz_gop},
	{"fin", sz_gop},
	{"out", sz_gop},
	{"jr", sz_gop},
	{"j", sz_gop},
	{"jal", sz_gop},
	{"fbz", sz_gop},
	{"fble", sz_gop},
	{"be", sz_gop},
	{"bei", sz_gop},
	{"ble", sz_gop},
	{"blei", sz_gop},
};

map<string, void (*) (char *) > OPM = {
	{".align", nop},
	{".global", nop},
	{"add", op_add},
	{"addi", op_addi},
	{"sub", op_sub},
	{"subi", op_subi},
	{"sl2add", op_sl2add},
	{"sl2addi", op_sl2addi},
	{"mov", op_mov},
	{"movi", op_movi},
	{"fadd", op_fadd},
	{"fsub", op_fsub},
	{"fmul", op_fmul},
	{"fdiv", op_fdiv},
	{"fmov", op_fmov},
	{"fneg", op_fneg},
	{"fabs", op_fabs},
	{"fsqrt", op_fsqrt},
	{"lw", op_lw},
	{"lwi", op_lwi},
	{"flw", op_flw},
	{"flwi", op_flwi},
	{"sw", op_sw},
	{"swi", op_swi},
	{"fsw", op_fsw},
	{"fswi", op_fswi},
	{"ftoi", op_ftoi},
	{"itof", op_itof},
	{"in", op_in},
	{"fin", op_fin},
	{"out", op_out},
	{"jr", op_jr},
	{"j", op_j},
	{"jal", op_jal},
	{"fbz", op_fbz},
	{"fble", op_fble},
	{"be", op_be},
	{"bei", op_bei},
	{"ble", op_ble},
	{"blei", op_blei},
};








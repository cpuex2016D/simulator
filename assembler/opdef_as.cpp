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
	if (STATE == 2)\
		throw "trying to write an op to undesirable area"; \
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
		



#define SET_RDRSRT \
	GETRN(rd); \
	MV2TKN(p); \
	GETRN(rs); \
	MV2TKN(p); \
	GETRN(rt); \
	MV2TKNB(p); \
	NLCHK

#define SET_RTRSC \
	GETRN(rt); \
	MV2TKN(p); \
	GETRN(rs); \
	MV2TKN(p); \
	c = (int)strtol(p, &p, 0); \
	MV2TKNB(p); \
	NLCHK

#define SET_RTCRS \
	GETRN(rt); \
	MV2TKN(p); \
	if (!('0' <= *p && *p <= '9')) throw "invalid arguments"; \
	c = (int)strtol(p, &p, 0); \
	if (*(p++) != '(') throw "invalid arguments"; \
	GETRN(rs); \
	if (*(p++) != ')') throw "invalid arguments"; \
	MV2TKNB(p); \
	NLCHK


	/*
	{\
		char *f93ghc, bv9w20bv;\
		map<string, int>::iterator i3ga;\
		for(f93ghc = p; *f93ghc != ' ' && *f93ghc != '\t' && *f93ghc != '\n';) f93ghc++;\
		bv9w20bv = *f93ghc;\
		i3ga = LBM.find(p);\
		*f93ghc = bv9w20bv;\
		if (p == LBM.end()) \
			throw "unknown lable";\
		c = i3ga->second;\
		while(*p != ' ' && *p != '\t' && *p != '\n') p++; while(*p == ' ' || *p == '\t') p++;\
		if (*p != '\n') throw "unknown arguments";\
	}\
	if (rt & 0xE0 || rs & 0xE0) throw "out of valid reg range"\
	*/


extern int LN;
extern int TA, DA;
extern int STATE;
extern char *TEX, *DAT;
typedef map<string, int (*) (char *) >::iterator mi;
extern map<string, int> LBM;



int my_strcmp(char *p, char const *s) {
	while(*s != '\0') {
		if (*p != *s) return 0;
		p++;
		s++;
	}
	if (*p != ' ' && *p != '\n' && *p != '\t') return 0;
	else return 1;
}


void sz_section(char *p) {
	if (my_strcmp(p, "\".text\"")) {
		STATE = 1;
	} else if (my_strcmp(p, "\".rodata\"")) {
		STATE = 2;
	} else throw "unknown section type";
	MV2NXTKNB(p);
	if (*p != '\n')
			throw "unknown arguments";
	return;
}

void sz_align(char *p) {
	int i;
	i = (int)strtol(p, &p, 0);
	if (STATE == 1) {
		int r = TA%i;
		if (r != 0)
			TA += i - r;
	} else {
		int r = DA%i;
		if (r != 0)
			DA += i - r;
	}
	return;
}

void sz_byte(char *p) {
	/*
	int i = 0;
	while(*p != '\n') {
		i++;
		strtol(p, &p, 0);
		MV2TKN(p);
	}
	if (STATE == 1)
		TA += i;
	else
		DA += i;
	*/
	DA += 1;
	return;
}

void op_byte(char *p) {
	char c;
	/*
	if (STATE == 1) {
		while(*p != '\n') {
			c = (char)strtol(p, &p, 0);
			TEX[TA] = c;
			TA++;
			MV2TKN(p);
		}
	} else {
		while(*p != '\n') {
			c = (char)strtol(p, &p, 0);
			DAT[DA] = c;
			DA++;
			MV2TKN(p);
		}
	}
	*/
	if ('0' <= *p && *p <= '9')
		c = (char)strtol(p, &p, 0);
	else throw "not implemented byte notation";
	MV2TKNB(p);
	NLCHK;
	return;
}

void nop(char *p) {
	return;
}

void sz_gop(char *p) {
	/*
	if (STATE == 2) {
		DA += 4;
	} else {
		TA += 4;
	}
	*/
	TA += 1;
	return;
}

void op_add(char *p) {
	uint32_t op;
	char rs, rd, rt;
	SET_RDRSRT;
	op = (rs << 21) | (rt << 16) | (rd << 11) | 0x20;
	STORE_OP;
	return;
}

void op_addi(char *p) {
	uint32_t op;
	char rs, rt;
	int c;
	SET_RTRSC;
	op = 1 << 29 | (rs << 21) | (rt << 16) | (c & 0xFFFF);
	STORE_OP;
	return;
}

void op_sub(char *p) {
	uint32_t op;
	char rs, rd, rt;
	SET_RDRSRT;
	op = (rs << 21) | (rt << 16) | (rd << 11) | 0x22;
	STORE_OP;
	return;
}

void op_and(char *p) {
	uint32_t op;
	char rs, rd, rt;
	SET_RDRSRT;
	op = (rs << 21) | (rt << 16) | (rd << 11) | 0x24;
	STORE_OP;
	return;
}

void op_andi(char *p) {
	uint32_t op;
	char rs, rt;
	int c;
	SET_RTRSC;
	op = 0x30000000 | (rs << 21) | (rt << 16) | (c & 0xFFFF);
	STORE_OP;
	return;
}

void op_or(char *p) {
	uint32_t op;
	char rs, rd, rt;
	SET_RDRSRT;
	op = (rs << 21) | (rt << 16) | (rd << 11) | 0x25;
	STORE_OP;
	return;
}

void op_ori(char *p) {
	uint32_t op;
	char rs, rt;
	int c;
	SET_RTRSC;
	op = 0x38000000 | (rs << 21) | (rt << 16) | (c & 0xFFFF);
	STORE_OP;
	return;
}

void op_nor(char *p) {
	uint32_t op;
	char rs, rd, rt;
	SET_RDRSRT;
	op = (rs << 21) | (rt << 16) | (rd << 11) | 0x27;
	STORE_OP;
	return;
}

void op_sll(char *p) {
	uint32_t op;
	char rs, rt, c;
	SET_RTRSC; // rd in spec. is rt, rt in spec. is rs
	op = (rs << 16) | (rt << 11) | ((c & 0x1F) << 6);
	STORE_OP;
	return;
}


void op_srl(char *p) {
	uint32_t op;
	char rs, rt, c;
	SET_RTRSC;
	op = (rs << 16) | (rt << 11) | ((c & 0x1F) << 6) | 0x2;
	STORE_OP;
	return;
}

void op_slt(char *p) {
	uint32_t op;
	char rs, rd, rt;
	SET_RDRSRT;
	op = (rs << 21) | (rt << 16) | (rd << 11) | 0x2A;
	STORE_OP;
	return;
}

void op_slti(char *p) {
	uint32_t op;
	char rs, rt;
	int c;
	SET_RTRSC;
	op = 0x28000000 | (rs << 21) | (rt << 16) | (c & 0xFFFF);
	STORE_OP;
	return;
}
	
void op_beq(char *p) {
	uint32_t op;
	char rs, rt;
	int c;
	GETRN(rs);
	MV2TKN(p);
	GETRN(rt);
	MV2TKN(p);
	if ('0' <= *p && *p <= '9') {
		c = (int)strtol(p, &p, 0);
	} else {
		char *q = p, tmp;
		map<string, int>::iterator itr;
		MV2SP(q);
		tmp = *q;
		*q = '\0';
		itr = LBM.find(p);
		*q = tmp;
		p = q;
		if (itr == LBM.end()) throw "unknown lable referenced";
		c = (itr->second) - TA;
	}
	MV2TKNB(p);
	NLCHK;
	op = 0x10000000 | (rs << 21) | (rt << 16) | (c & 0xFFFF);
	STORE_OP;
	return;
}

void op_bne(char *p) {
	uint32_t op;
	char rs, rt;
	int c;
	GETRN(rs);
	MV2TKN(p);
	GETRN(rt);
	MV2TKN(p);
	if ('0' <= *p && *p <= '9') {
		c = (int)strtol(p, &p, 0);
	} else {
		char *q = p, tmp;
		map<string, int>::iterator itr;
		MV2SP(q);
		tmp = *q;
		*q = '\0';
		itr = LBM.find(p);
		*q = tmp;
		p = q;
		if (itr == LBM.end()) throw "unknown lable referenced";
		c = (itr->second) - TA;
	}
	MV2TKNB(p);
	NLCHK;
	op = 0x14000000 | (rs << 21) | (rt << 16) | (c & 0xFFFF);
	STORE_OP;
	return;
}

void op_j(char *p) {
	uint32_t op;
	int c;
	if ('0' <= *p && *p <= '9') {
		c = (int)strtol(p, &p, 0);
	} else if (*p == '\n') {
		throw "invalid arguments";
	} else {
		char *q = p, tmp;
		map<string, int>::iterator itr;
		MV2SP(q);
		tmp = *q;
		*q = '\0';
		itr = LBM.find(p);
		*q = tmp;
		p = q;
		if (itr == LBM.end()) throw "unknown lable referenced";
		c = itr->second;
	}
	MV2TKNB(p);
	NLCHK;
	op = 0x08000000 | (c & 0x03FFFFFF);
	STORE_OP;
	return;
}
		

void op_jal(char *p) {
	uint32_t op;
	int c;
	if ('0' <= *p && *p <= '9') {
		c = (int)strtol(p, &p, 0);
	} else if (*p == '\n') {
		throw "invalid arguments";
	} else {
		char *q = p, tmp;
		map<string, int>::iterator itr;
		MV2SP(q);
		tmp = *q;
		*q = '\0';
		itr = LBM.find(p);
		*q = tmp;
		p = q;
		if (itr == LBM.end()) throw "unknown lable referenced";
		c = itr->second;
	}
	MV2TKNB(p);
	NLCHK;
	op = 0x0C000000 | (c & 0x03FFFFFF);
	STORE_OP;
	return;
}

void op_jr(char *p) {
	uint32_t op;
	char rs;
	GETRN(rs);
	MV2TKNB(p);
	NLCHK;
	op = (rs << 21) | 0x8;
	STORE_OP;
	return;
}

void op_jalr(char *p) {
	uint32_t op;
	char rs;
	GETRN(rs);
	MV2TKNB(p);
	NLCHK;
	op = (rs << 21) | 0x9;
	STORE_OP;
	return;
}


void op_lw(char *p) {
	uint32_t op;
	char rt, rs;
	int c;
	SET_RTCRS;
	op = 0x8C000000 | (rs << 21) | (rt << 16) | (c & 0xFFFF);
	STORE_OP;
	return;
}

void op_lui(char *p) {
	uint32_t op;
	char rt;
	int c;
	GETRN(rt);
	MV2TKN(p);
	if ('0' <= *p && *p <= '9')
		c = (int) strtol(p, &p, 0);
	else {
		char *q = p, tmp;
		map<string, int>::iterator itr;
		MV2SP(q);
		tmp = *q;
		*q = '\0';
		itr = LBM.find(p);
		*q = tmp;
		p = q;
		if (itr == LBM.end()) throw "unknown lable referenced";
		c = itr->second;
	}
	MV2TKNB(p);
	NLCHK;
	op = 0x3C000000 | (rt << 16) | (c & 0xFFFF);
	STORE_OP;
	return;
}

void op_sw(char *p) {
	uint32_t op;
	char rt, rs;
	int c;
	SET_RTCRS;
	op = 0xAC000000 | (rs << 21) | (rt << 16) | (c & 0xFFFF);
	STORE_OP;
	return;
}

void op_in(char *p) {
	uint32_t op;
	char rt;
	GETRN(rt);
	MV2TKNB(p);
	NLCHK;
	op = 0x68000000 | (rt << 16);
	STORE_OP;
	return;
}

void op_out(char *p) {
	uint32_t op;
	char rt;
	GETRN(rt);
	MV2TKNB(p);
	NLCHK;
	op = 0x6C000000 | (rt << 16);
	STORE_OP;
	return;
}

void op_bt_s(char *p) {
	uint32_t op;
	char cc;
	int c;
	if ('0' <= *p && *p <= '9')
		cc = (char)strtol(p, &p, 0);
	else throw "invalid arguments";
	MV2TKN(p);
	if (*p == '\n' || cc < 0 || cc >= 8) throw "invalid arguments";
	if ('0' <= *p && *p <= '9')
		c = (int)strtol(p, &p, 0);
	else {
		char *q = p, tmp;
		map<string, int>::iterator itr;
		MV2SP(q);
		tmp = *q;
		*q = '\0';
		itr = LBM.find(p);
		*q = tmp;
		p = q;
		if (itr == LBM.end()) throw "unknown lable referenced";
		c = (itr->second) - TA;
	}
	MV2TKNB(p);
	NLCHK;
	op = 0x45010000 | (cc << 18) | (c & 0xFFFF);
	STORE_OP;
	return;
}

void op_bf_s(char *p) {
	uint32_t op;
	char cc;
	int c;
	if ('0' <= *p && *p <= '9')
		cc = (char)strtol(p, &p, 0);
	else throw "invalid arguments";
	MV2TKN(p);
	if (*p == '\n' || cc < 0 || cc >= 8) throw "invalid arguments";
	if ('0' <= *p && *p <= '9')
		c = (int)strtol(p, &p, 0);
	else {
		char *q = p, tmp;
		map<string, int>::iterator itr;
		MV2SP(q);
		tmp = *q;
		*q = '\0';
		itr = LBM.find(p);
		*q = tmp;
		p = q;
		if (itr == LBM.end()) throw "unknown lable referenced";
		c = (itr->second) - TA;
	}
	MV2TKNB(p);
	NLCHK;
	op = 0x45000000 | (cc << 18) | (c & 0xFFFF);
	STORE_OP;
	return;
}

void op_add_s(char *p) {
	uint32_t op;
	char rs, rd, rt;
	SET_RDRSRT;
	op = 0x46000000 | (rt << 16) | (rs << 11) | (rd << 6);
	STORE_OP;
	return;
}

void op_sub_s(char *p) {
	uint32_t op;
	char rs, rd, rt;
	SET_RDRSRT;
	op = 0x46000001 | (rt << 16) | (rs << 11) | (rd << 6);
	STORE_OP;
	return;
}


void op_mul_s(char *p) {
	uint32_t op;
	char rs, rd, rt;
	SET_RDRSRT;
	op = 0x46000002 | (rt << 16) | (rs << 11) | (rd << 6);
	STORE_OP;
	return;
}


void op_div_s(char *p) {
	uint32_t op;
	char rs, rd, rt;
	SET_RDRSRT;
	op = 0x46000003 | (rt << 16) | (rs << 11) | (rd << 6);
	STORE_OP;
	return;
}

void op_mov_s(char *p) {
	uint32_t op;
	char fd, ft;
	GETRN(fd);
	MV2TKN(p);
	GETRN(ft);
	MV2TKNB(p);
	NLCHK;
	op = 0x46000006 | (ft << 16) | (fd << 6);
	STORE_OP;
	return;
}


void op_neg_s(char *p) {
	uint32_t op;
	char fd, ft;
	GETRN(fd);
	MV2TKN(p);
	GETRN(ft);
	MV2TKNB(p);
	NLCHK;
	op = 0x46000007 | (ft << 16) | (fd << 6);
	STORE_OP;
	return;
}

void op_c_eq_s(char *p) {
	uint32_t op;
	char cc, fs, ft;
	if (!('0' <= *p && *p <= '9')) throw "invalid arguments";
	cc = (char) strtol(p, &p, 0);
	MV2TKN(p);
	GETRN(fs);
	MV2TKN(p);
	GETRN(ft);
	MV2TKNB(p);
	NLCHK;
	if (cc < 0 || cc >= 8) "invalid arguments";
	op = 0x46000032 | (ft << 16) | (fs << 11) | (cc << 8);
	STORE_OP;
	return;
}
void op_c_lt_s(char *p) {
	uint32_t op;
	char cc, fs, ft;
	if (!('0' <= *p && *p <= '9')) throw "invalid arguments";
	cc = (char) strtol(p, &p, 0);
	MV2TKN(p);
	GETRN(fs);
	MV2TKN(p);
	GETRN(ft);
	MV2TKNB(p);
	NLCHK;
	if (cc < 0 || cc >= 8) "invalid arguments";
	op = 0x4600003C | (ft << 16) | (fs << 11) | (cc << 8);
	STORE_OP;
	return;
}
void op_c_le_s(char *p) {
	uint32_t op;
	char cc, fs, ft;
	if (!('0' <= *p && *p <= '9')) throw "invalid arguments";
	cc = (char) strtol(p, &p, 0);
	MV2TKN(p);
	GETRN(fs);
	MV2TKN(p);
	GETRN(ft);
	MV2TKNB(p);
	NLCHK;
	if (cc < 0 || cc >= 8) "invalid arguments";
	op = 0x4600003E | (ft << 16) | (fs << 11) | (cc << 8);
	STORE_OP;
	return;
}

void op_lw_s(char *p) {
	uint32_t op;
	char rt, rs;
	int c;
	SET_RTCRS;
	op = 0xC4000000 | (rs << 21) | (rt << 16) | (c & 0xFFFF);
	STORE_OP;
	return;
}

void op_sw_s(char *p) {
	uint32_t op;
	char rt, rs;
	int c;
	SET_RTCRS;
	op = 0xE4000000 | (rs << 21) | (rt << 16) | (c & 0xFFFF);
	STORE_OP;
	return;
}



map<string, void (*) (char *) > SZM = {{".section", sz_section}, {".align", nop}, {".byte", sz_byte}, {".global", nop}, {"add", sz_gop}, {"addi", sz_gop}, {"sub", sz_gop}, {"and", sz_gop}, {"andi", sz_gop}, {"or", sz_gop}, {"ori", sz_gop}, {"nor", sz_gop}, {"sll", sz_gop}, {"srl", sz_gop}, {"slt", sz_gop}, {"slti", sz_gop}, {"beq", sz_gop}, {"bne", sz_gop}, {"j", sz_gop}, {"jal", sz_gop}, {"jr", sz_gop}, {"jalr", sz_gop}, {"lw", sz_gop}, {"lui", sz_gop}, {"sw", sz_gop}, {"in", sz_gop}, {"out", sz_gop}, {"bt.s", sz_gop}, {"bf.s", sz_gop}, {"add.s", sz_gop}, {"sub.s", sz_gop}, {"mul.s", sz_gop}, {"div.s", sz_gop}, {"mov.s", sz_gop}, {"neg.s", sz_gop}, {"c.eq.s", sz_gop}, {"c.lt.s", sz_gop}, {"c.le.s", sz_gop}, {"lw.s", sz_gop}, {"sw.s", sz_gop}/*, {"la", sz_gop}*/};

map<string, void (*) (char *) > OPM = {{".section", sz_section}, {".align", sz_align}, {".byte", op_byte}, {".global", nop}, {"add", op_add}, {"addi", op_addi}, {"sub", op_sub}, {"and", op_and}, {"andi", op_andi}, {"or", op_or}, {"ori", op_ori}, {"nor", op_nor}, {"sll", op_sll}, {"srl", op_srl}, {"slt", op_slt}, {"slti", op_slti}, {"beq", op_beq}, {"bne", op_bne}, {"j", op_j}, {"jal", op_jal}, {"jr", op_jr}, {"jalr", op_jalr}, {"lw", op_lw}, {"lui", op_lui}, {"sw", op_sw}, {"in", op_in}, {"out", op_out}, {"bt.s", op_bt_s}, {"bf.s", op_bf_s}, {"add.s", op_add_s}, {"sub.s", op_sub_s}, {"mul.s", op_mul_s}, {"div.s", op_div_s}, {"mov.s", op_mov_s}, {"neg.s", op_neg_s}, {"c.eq.s", op_c_eq_s}, {"c.lt.s", op_c_lt_s}, {"c.le.s", op_c_le_s}, {"lw.s", op_lw_s}, {"sw.s", op_sw_s}};








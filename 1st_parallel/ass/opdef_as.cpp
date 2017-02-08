#include<cstdlib>
#include<cstdio>
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

extern int TA, DA;
extern char *TEX, *DAT;
typedef map<char*, int, bool (*) (char*, char*)>::iterator lmi;
extern map<char*, int, bool (*) (char*, char*)> LBM;


#define STORE_OP(op) \
	*((uint32_t *)&TEX[TA*4]) = (op); \
	TA += 1

#define NLCHK if (*p != '\n') throw "too many tokens"


#define GETRN(rn) \
	int rn; \
	if (*(p++) != '%' || *(p++) != 'r') throw "invalid register"; \
	if ('0' == *p || ('4' <= *p && *p <= '9')) { \
		rn = *p - '0'; \
		p++; \
	} else if (*p == '1' || *p == '2') { \
		p++; \
		if ('0' <= *p && *p <= '9') { \
			rn = (*(p-1) - '0') * 10 + *p - '0'; \
			p++; \
		} else { \
			rn = *(p-1) - '0'; \
		} \
	} else if (*p == '3') { \
		p++; \
		if ('0' == *p || *p == '1') { \
			rn = 30 + *p - '0'; \
			p++; \
		} else { \
			rn = 3; \
		} \
	} else throw "invalid register"; \
	if (*p != ',' && *p != ' ' && *p != '\n') throw "invalid register"; \
	MV2TKN(p)


#define GETFN(fn) \
	int fn; \
	if (*(p++) != '%' || *(p++) != 'f') throw "invalid register"; \
	if ('0' == *p || ('4' <= *p && *p <= '9')) { \
		fn = *p - '0'; \
		p++; \
	} else if (*p == '1' || *p == '2') { \
		p++; \
		if ('0' <= *p && *p <= '9') { \
			fn = (*(p-1) - '0') * 10 + *p - '0'; \
			p++; \
		} else { \
			fn = *(p-1) - '0'; \
		} \
	} else if (*p == '3') { \
		p++; \
		if ('0' == *p || *p == '1') { \
			fn = 30 + *p - '0'; \
			p++; \
		} else { \
			fn = 3; \
		} \
	} else throw "invalid register"; \
	if (*p != ',' && *p != ' ' && *p != '\n') throw "invalid register"; \
	MV2TKN(p)

#define GETAN(an) \
	int an; \
	if (*(p++) != '%' || *(p++) != 'f') throw "invalid register"; \
	if ('0' == *p || ('4' <= *p && *p <= '9')) { \
		an = *p - '0'; \
		p++; \
	} else if (*p == '1' || *p == '2') { \
		p++; \
		if ('0' <= *p && *p <= '9') { \
			an = (*(p-1) - '0') * 10 + *p - '0'; \
			p++; \
		} else { \
			an = *(p-1) - '0'; \
		} \
	} else if (*p == '3') { \
		p++; \
		if ('0' == *p || *p == '1') { \
			an = 30 + *p - '0'; \
			p++; \
		} else { \
			an = 3; \
		} \
	} else throw "invalid register"; \
	if (*p != ',' && *p != ' ' && *p != '\n') throw "invalid register"; \
	MV2TKN(p)


#define GETC(c) \
	int c; \
	c = strtol(p, &p, 0); \
	MV2TKN(p)

#define GETCRN(c, rn) \
	int c, rn; \
	c = strtol(p, &p, 0); \
	if (*(p++) != '(' || *(p++) != '%' || *(p++) != 'r') throw "invalid argument"; \
	if ('0' == *p || ('4' <= *p && *p <= '9')) { \
		rn = *p - '0'; \
		p++; \
	} else if (*p == '1' || *p == '2') { \
		p++; \
		if ('0' <= *p && *p <= '9') { \
			rn = (*(p-1) - '0') * 10 + *p - '0'; \
			p++; \
		} else { \
			rn = *(p-1) - '0'; \
		} \
	} else if (*p == '3') { \
		p++; \
		if ('0' == *p || *p == '1') { \
			rn = 30 + *p - '0'; \
			p++; \
		} else { \
			rn = 3; \
		} \
	} else throw "invalid register"; \
	if (*(p++) != ')') throw "invalid argument"; \
	if (*p != ',' && *p != ' ' && *p != '\n') throw "invalid register"; \
	MV2TKN(p)

#define GETLABLE(c) \
	int c; \
	{\
		lmi _lable_temp__itr_ = LBM.find(p); \
		if (_lable_temp__itr_ == LBM.end()) throw "unknown lable referenced"; \
			c = (_lable_temp__itr_->second); \
	} \
	MV2NXTKN(p)



void nop(char *p) {
	return;
}

void sz_gop(char *p) {
	TA += 1;
}

void sz_la(char *p) {
	TA += 2;
}

void op_add(char *p) {
	GETRN(rd); GETRN(rs); GETRN(rt); NLCHK;
	STORE_OP((rs << 21) | (rt << 16) | (rd << 11) | 0x20);
}

void op_addi(char *p) {
	GETRN(rt); GETRN(rs); GETC(c); NLCHK;
	STORE_OP(1 << 29 | (rs << 21) | (rt << 16) | (c & 0xFFFF));
}

void op_sub(char *p) {
	GETRN(rd); GETRN(rs); GETRN(rt); NLCHK;
	STORE_OP((rs << 21) | (rt << 16) | (rd << 11) | 0x22);
}

void op_and(char *p) {
	GETRN(rd); GETRN(rs); GETRN(rt); NLCHK;
	STORE_OP((rs << 21) | (rt << 16) | (rd << 11) | 0x24);
}

void op_andi(char *p) {
	GETRN(rt); GETRN(rs); GETC(c); NLCHK;
	STORE_OP(0x30000000 | (rs << 21) | (rt << 16) | (c & 0xFFFF));
}

void op_or(char *p) {
	GETRN(rd); GETRN(rs); GETRN(rt); NLCHK;
	STORE_OP((rs << 21) | (rt << 16) | (rd << 11) | 0x25);
}

void op_ori(char *p) {
	GETRN(rt); GETRN(rs); GETC(c); NLCHK;
	STORE_OP(0x34000000 | (rs << 21) | (rt << 16) | (c & 0xFFFF));
}

void op_nor(char *p) {
	GETRN(rd); GETRN(rs); GETRN(rt); NLCHK;
	STORE_OP((rs << 21) | (rt << 16) | (rd << 11) | 0x27);
}

void op_sll(char *p) {
	GETRN(rd); GETRN(rt); GETC(c); NLCHK;
	STORE_OP((rt << 16) | (rd << 11) | ((c & 0x1F) << 6));
}


void op_srl(char *p) {
	GETRN(rd); GETRN(rt); GETC(c); NLCHK;
	STORE_OP((rt << 16) | (rd << 11) | ((c & 0x1F) << 6) | 0x2);
}

void op_slt(char *p) {
	GETRN(rd); GETRN(rs); GETRN(rt); NLCHK;
	STORE_OP((rs << 21) | (rt << 16) | (rd << 11) | 0x2A);
}

void op_slti(char *p) {
	GETRN(rt); GETRN(rs); GETC(c); NLCHK;
	STORE_OP(0x28000000 | (rs << 21) | (rt << 16) | (c & 0xFFFF));
}
	
void op_beq(char *p) {
	GETRN(rs); GETRN(rt); GETLABLE(c); NLCHK;
	c -= TA;
	STORE_OP(0x10000000 | (rs << 21) | (rt << 16) | (c & 0xFFFF));
}

void op_bne(char *p) {
	GETRN(rs); GETRN(rt); GETLABLE(c); NLCHK;
	c -= TA;
	STORE_OP(0x14000000 | (rs << 21) | (rt << 16) | (c & 0xFFFF));
}

void op_j(char *p) {
	GETLABLE(c); NLCHK;
	STORE_OP(0x08000000 | (c & 0x03FFFFFF));
}
		

void op_jal(char *p) {
	GETLABLE(c); NLCHK;
	STORE_OP(0x0C000000 | (c & 0x03FFFFFF));
}

void op_jr(char *p) {
	GETRN(rs); NLCHK;
	STORE_OP((rs << 21) | 0x8);
}

void op_jalr(char *p) {
	GETRN(rs); NLCHK;
	STORE_OP((rs << 21) | 0x9);
}

void op_lw(char *p) {
	GETRN(rt); GETCRN(c, rs); NLCHK;
	STORE_OP(0x8C000000 | (rs << 21) | (rt << 16) | (c & 0xFFFF));
}

void op_lui(char *p) {
	GETRN(rt); GETC(c); NLCHK;
	STORE_OP(0x3C000000 | (rt << 16) | (c & 0xFFFF));
}

void op_sw(char *p) {
	GETRN(rt); GETCRN(c, rs); NLCHK;
	STORE_OP(0xAC000000 | (rs << 21) | (rt << 16) | (c & 0xFFFF));
}

void op_in(char *p) {
	GETRN(rt); NLCHK;
	STORE_OP(0x68000000 | (rt << 16));
}

void op_out(char *p) {
	GETRN(rt); NLCHK;
	STORE_OP(0x6C000000 | (rt << 16));
}

void op_bt_s(char *p) {
	GETC(cc); GETLABLE(c); NLCHK;
	if (cc < 0 || cc >= 8) throw "invalid arguments";
	c -= TA;
	STORE_OP(0x45010000 | (cc << 18) | (c & 0xFFFF));
}

void op_bf_s(char *p) {
	GETC(cc); GETLABLE(c); NLCHK;
	if (cc < 0 || cc >= 8) throw "invalid arguments";
	c -= TA;
	STORE_OP(0x45000000 | (cc << 18) | (c & 0xFFFF));
}

void op_add_s(char *p) {
	GETFN(fd); GETFN(fs); GETFN(ft); NLCHK;
	STORE_OP(0x46000000 | (ft << 16) | (fs << 11) | (fd << 6));
}

void op_sub_s(char *p) {
	GETFN(fd); GETFN(fs); GETFN(ft); NLCHK;
	STORE_OP(0x46000001 | (ft << 16) | (fs << 11) | (fd << 6));
}

void op_mul_s(char *p) {
	GETFN(fd); GETFN(fs); GETFN(ft); NLCHK;
	STORE_OP(0x46000002 | (ft << 16) | (fs << 11) | (fd << 6));
}

void op_div_s(char *p) {
	GETFN(fd); GETFN(fs); GETFN(ft); NLCHK;
	STORE_OP(0x46000003 | (ft << 16) | (fs << 11) | (fd << 6));
}

void op_mov_s(char *p) {
	GETFN(fd); GETFN(ft); NLCHK;
	STORE_OP(0x46000006 | (ft << 16) | (fd << 6));
}


void op_neg_s(char *p) {
	GETFN(fd); GETFN(ft); NLCHK;
	STORE_OP(0x46000007 | (ft << 16) | (fd << 6));
}

void op_abs_s(char *p) {
	GETFN(fd); GETFN(ft); NLCHK;
	STORE_OP(0x46000005 | (ft << 16) | (fd << 6));
}

void op_sqrt_s(char *p) {
	GETFN(fd); GETFN(ft); NLCHK;
	STORE_OP(0x46000004 | (ft << 16) | (fd << 6));
}

void op_c_eq_s(char *p) {
	GETC(cc); GETFN(fs); GETFN(ft); NLCHK;
	if (cc < 0 || cc >= 8) throw "invalid arguments";
	STORE_OP(0x46000032 | (ft << 16) | (fs << 11) | (cc << 8));
}

void op_c_lt_s(char *p) {
	GETC(cc); GETFN(fs); GETFN(ft); NLCHK;
	if (cc < 0 || cc >= 8) throw "invalid arguments";
	STORE_OP(0x4600003C | (ft << 16) | (fs << 11) | (cc << 8));
}

void op_c_le_s(char *p) {
	GETC(cc); GETFN(fs); GETFN(ft); NLCHK;
	if (cc < 0 || cc >= 8) throw "invalid arguments";
	STORE_OP(0x4600003E | (ft << 16) | (fs << 11) | (cc << 8));
}

void op_lw_s(char *p) {
	GETFN(ft); GETCRN(c, rs); NLCHK;
	STORE_OP(0xC4000000 | (rs << 21) | (ft << 16) | (c & 0xFFFF));
}

void op_sw_s(char *p) {
	GETFN(ft); GETCRN(c, rs); NLCHK;
	STORE_OP(0xE4000000 | (rs << 21) | (ft << 16) | (c & 0xFFFF));
}

void op_ftoi(char *p) {
	GETRN(rt); GETFN(fs); NLCHK;
	STORE_OP(0xE0000000 | (fs << 21) | (rt << 16));
}

void op_itof(char *p) {
	GETFN(ft); GETRN(rs); NLCHK;
	STORE_OP(0xC0000000 | (rs << 21) | (ft << 16));
}

void op_la(char *p) {
	GETRN(rt); GETLABLE(l); NLCHK;
	STORE_OP(0x3C000000 | (rt << 16) | (l >> 16));
	STORE_OP(0x34000000 | (rt << 21) | (rt << 16) | (l & 0xFFFF));
}

void op_next(char *p) {
	GETRN(r0); NLCHK;
	STORE_OP(0b01110100000000000000000000000000 | r0 << 21);
}

void op_acc(char *p) {
	GETAN(a0); GETFN(f1); NLCHK;
	STORE_OP(0b01111000000000000000000000000000 | a0 << 21 | f1 << 16);
}

void op_fork(char *p) {
	GETRN(r1); GETRN(r2); NLCHK;
	STORE_OP(0b01111100000000000000000000000000 | r1 << 16 | r2 << 11);
}

void op_end(char *p) {
	NLCHK;
	STORE_OP(0b01111100000000000000000000000000);
}


bool cpcmp(const char* p1, const char* p2) {
    while(1) {
        if (*p1 == ' ' || *p1 == '\n' || *p1 == '\t' || *p1 == '\0') {
            if (*p2 == ' ' || *p2 == '\n' || *p2 == '\t' || *p2 == '\0') {
                return 0;
            }
            return 1;
        }
        if (*p2 == ' ' || *p2 == '\n' || *p2 == '\t' || *p2 == '\0') {
            return 0;
        }
        if (*p1 < *p2) {
            return 1;
        } else if (*p1 > *p2) {
            return 0;
        }
        p1++; p2++;
    }
}

map<const char*, void (*) (char*), bool (*) (const char*, const char*)> SZM({
	{".align", nop},
	{".global", nop},
	{"add", sz_gop},
	{"addi", sz_gop},
	{"sub", sz_gop},
	{"and", sz_gop},
	{"andi", sz_gop},
	{"or", sz_gop},
	{"ori", sz_gop},
	{"nor", sz_gop},
	{"sll", sz_gop},
	{"srl", sz_gop},
	{"slt", sz_gop},
	{"slti", sz_gop},
	{"beq", sz_gop},
	{"bne", sz_gop},
	{"j", sz_gop},
	{"jal", sz_gop},
	{"jr", sz_gop},
	{"jalr", sz_gop},
	{"lw", sz_gop},
	{"lui", sz_gop},
	{"sw", sz_gop},
	{"in", sz_gop},
	{"out", sz_gop},
	{"bt.s", sz_gop},
	{"bf.s", sz_gop},
	{"add.s", sz_gop},
	{"sub.s", sz_gop},
	{"mul.s", sz_gop},
	{"div.s", sz_gop},
	{"mov.s", sz_gop},
	{"neg.s", sz_gop},
	{"abs.s", sz_gop},
	{"sqrt.s", sz_gop},
	{"c.eq.s", sz_gop},
	{"c.lt.s", sz_gop},
	{"c.le.s", sz_gop},
	{"lw.s", sz_gop},
	{"sw.s", sz_gop},
	{"ftoi", sz_gop},
	{"itof", sz_gop},
	{"la", sz_la},
	{"next", sz_gop},
	{"acc", sz_gop},
	{"fork", sz_gop},
	{"end", sz_gop}
}, cpcmp);

map<const char*, void (*) (char*), bool (*) (const char*, const char*)> OPM({
	{".align", nop},
	{".global", nop},
	{"add", op_add},
	{"addi", op_addi},
	{"sub", op_sub},
	{"and", op_and},
	{"andi", op_andi},
	{"or", op_or},
	{"ori", op_ori},
	{"nor", op_nor},
	{"sll", op_sll},
	{"srl", op_srl},
	{"slt", op_slt},
	{"slti", op_slti},
	{"beq", op_beq},
	{"bne", op_bne},
	{"j", op_j},
	{"jal", op_jal},
	{"jr", op_jr},
	{"jalr", op_jalr},
	{"lw", op_lw},
	{"lui", op_lui},
	{"sw", op_sw},
	{"in", op_in},
	{"out", op_out},
	{"bt.s", op_bt_s},
	{"bf.s", op_bf_s},
	{"add.s", op_add_s},
	{"sub.s", op_sub_s},
	{"mul.s", op_mul_s},
	{"div.s", op_div_s},
	{"mov.s", op_mov_s},
	{"neg.s", op_neg_s},
	{"abs.s", op_abs_s},
	{"sqrt.s", op_sqrt_s},
	{"c.eq.s", op_c_eq_s},
	{"c.lt.s", op_c_lt_s},
	{"c.le.s", op_c_le_s},
	{"lw.s", op_lw_s},
	{"sw.s", op_sw_s},
	{"ftoi", op_ftoi},
	{"itof", op_itof},
	{"la", op_la},
	{"next", op_next},
	{"acc", op_acc},
	{"fork", op_fork},
	{"end", op_end}
}, cpcmp);


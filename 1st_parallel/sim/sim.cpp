#include"simprepare.h"
#include"parent_core.h"
#include<stdint.h>
#include<stdio.h>

#define CORE_NUM 4


typedef struct {
	int PC;
	uint32_t GPR[32];
	float FPR[32];
	char FPCC[9];
	uint32_t *DAT;
	int PJ;
} coreenv;

typedef struct passed_args_set {
	int32_t RD, RS, RT, C, FD, FS, FT, CC;
	void (*OP_EX) (coreenv& ce, struct passed_args_set&);
	void (*OP_PRNT) (struct passed_args_set&);
	int STOP;
	unsigned long long int COUNTS;
} arg;

coreenv CE[CORE_NUM];
int32_t AC, AD;

uint32_t *PTEX, *CTEX;
int PTEX_LAST, CTEX_LAST; // PTEX[PTEX_LAST-1] is the last accessible area


FILE *IFILE;
/*
unsigned long long OP_COUNT[OP_TOTAL] = {0};
int OP_TYPE;
unsigned long long COUNTS;
*/



int main(int argc, char *argv[]) {
	if (!simprepare(argc, argv)) {
		return 1;
	}
	invoke_parent_core(NULL);
	return 0;
}


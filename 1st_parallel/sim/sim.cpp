#include"sim.h"
#include"simprepare.h"
#include"parent_core.h"
#include<stdint.h>
#include<stdio.h>


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


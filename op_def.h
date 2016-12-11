#ifndef OP_DIF_H
#define OP_DIF_H

typedef struct {
	int (*is_op) (void);
	void (*op) (void);
} op_set;
extern op_set op_array[];

#endif

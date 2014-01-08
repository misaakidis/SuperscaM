#ifndef REGISTERFILE_H_
#define REGISTERFILE_H_

#include "definitions.h"

typedef struct {
	int_fast32_t value;
	//uint_fast8_t cycles;
	bool isFree;
} Rreg;

typedef struct {
	union{
		double single;	//let's have some fun! For a single unified double value
						//Assumes 64bit representation which is the default for most platforms
		struct {
			uint32_t high;
			uint32_t low;
		} split;
	} value;
	//uint_fast8_t cycles;
	bool isFree;
} Freg;

typedef union{
    Rreg rreg;
    Freg freg;
} Register;

extern Rreg* RFile;
extern Freg* FFile;

int index_to_rreg_value(int index);
double index_to_freg_value(int index);
void set_rreg_value(int index, int value);
void set_freg_value(int index, double value);


#endif /* REGISTERFILE_H_ */

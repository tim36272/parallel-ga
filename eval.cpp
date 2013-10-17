#ifndef EVAL_H
#define EVAL_H
#include "eval.h"
#include "individual.h"
#include <iostream>
#include "utility.h"
#include<boost/thread.hpp>

#define DIRTY -1

//define the protoype for eval, which
double eval(int *chromosome);

//the function to be threaded
void EvalParallel(individual* data) {
	//convert the bitset to int*
	int chromosome_binary[data->length_];
	bitset_to_binary(data->chromosome_, chromosome_binary);
	data->fitness_ = eval(chromosome_binary);
}
void eval(individual* data) {
	//the fitness pointer *must* remain allocated until the function returns
	//set the fitness to DIRTY
	data->fitness_ = DIRTY;
	//make a call to the evaluator. This function returns immediately, and the
	//fitness is not yet set
	boost::thread t(&EvalParallel,data);

	//return
	return;
}
#endif // EVAL_H

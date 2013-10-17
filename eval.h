#ifndef EVAL_H
#define EVAL_H
//a parallelizd evaluation method. Any call to evaluate() returns immediately,
//and once the fitness is set the eval is actually done
#include <iostream>
#include<boost/thread.hpp>
#include "utility.h"
#include "types.h"
//#include "individual.h"
class individual;

//define the protoype for eval, which
double eval(int *chromosome);

//the function to be threaded
void EvalParallel(individual* data);
void eval(individual* data);
#endif //EVAL_H

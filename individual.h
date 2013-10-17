#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H
#include <boost/dynamic_bitset.hpp>
#include <boost/shared_ptr.hpp>
#include "eval.h"

//individual in the population
class individual {
public:
	boost::dynamic_bitset<> chromosome_;
	int chromosome_decoded_; //in decimal
	int length_;

	double fitness_;
	void init(int length);

	friend std::ostream& operator <<(std::ostream& out, const individual& rhs);

	void crossover(boost::shared_ptr<individual> rhs);
	void CopyTo(individual* rhs);
};
#endif //INDIVIDUAL_H

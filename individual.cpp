#include "individual.h"
#include "eval.h"

void individual::init(int length) {
	length_ = length;
	chromosome_.resize(length);
	for(int i=0;i<length;i++) {
		chromosome_[i] = rand()%2;
	}
}

std::ostream& operator <<(std::ostream& out, const individual& rhs) {
	out<<rhs.chromosome_<<" Fit: "<<rhs.fitness_;
	return out;
}

void individual::crossover(boost::shared_ptr<individual> rhs) {
	//select a random spot in the chromosome and crossover the chromosomes in
	//that position
	int crossover_point = rand()%length_;
	boost::dynamic_bitset<> new_1(length_);
	boost::dynamic_bitset<> new_2(length_);

	for(int i=0;i<length_;i++) {
		if(i < crossover_point) {
			new_1[i] = this->chromosome_[i];
			new_2[i] = rhs->chromosome_[i];
		}
		else {
			new_1[i] = rhs->chromosome_[i];
			new_2[i] = this->chromosome_[i];
		}
	}
	rhs->chromosome_.swap(new_2);
	this->chromosome_.swap(new_1);
}

void individual::CopyTo(individual* rhs) {
	rhs->chromosome_ = chromosome_;
	rhs->chromosome_decoded_ = chromosome_decoded_;
	rhs->fitness_ = fitness_;
	rhs->length_ = length_;
}

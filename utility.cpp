#include "utility.h"
double bin_to_dec(int *chromosome, int l)
{
  int i;
  double product;

  product = 0.0;

  for(i = 0; i < l; i++){
	  product += (chromosome[i] == 0 ? 0.0 : pow((double)2.0, (double) i));
  }
  return product;
}

void dec_to_bin(int decimal_value, int *chromosome, int size)
{

	for(int i = 0; i < size; i++){
		chromosome[i] = decimal_value%2;
		decimal_value /=2;
	}
}


void bitset_to_binary(const boost::dynamic_bitset<>& chromosome, int* output) {
	for(int i=0;i<chromosome.size();i++) {
		output[i] = chromosome[i];
	}
}

void print_statistics_headers(std::ostream& out) {
	out<<std::setw(5)<<std::left<<"Gen";
	out<<std::setw(4)<<std::left<<"Max";
	out<<std::setw(4)<<std::left<<"Min";
	out<<std::setw(4)<<std::left<<"Avg";
	out<<std::setw(4)<<std::left<<"Max Ever";
	out<<std::setw(4)<<std::left<<"Max From"<<std::endl;
}

#ifndef UTILITY_H
#define UTILTIY_H

#include <cmath>
#include <fstream>
#include <boost/dynamic_bitset.hpp>
#include <iomanip>


double bin_to_dec(int *chromosome, int l);

//assumes *chromosome is already allocated size ints
void dec_to_bin(int decimal_value, int *chromosome, int size);

//assumes *output is already allocated
void bitset_to_binary(const boost::dynamic_bitset<>& chromosome, int* output);

//prints column headers
void print_statistics_headers(std::ostream& out);
#endif // UTILITY_H

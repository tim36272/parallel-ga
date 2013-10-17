#ifndef SPECIES_H
#define SPECIES_H

#include <fstream>
#include "individual.h"
#include <boost/smart_ptr.hpp>

//a population of the species
class species {
public:
	species() {assert(false);}
	species(const char in_file[]);
	std::vector<boost::shared_ptr<individual> > population_;
	int chromosome_length_;  /* chromosome length */
	int generation_;     /* current generation */

	double prob_crossover_;        /* probability of cross over */
	double prob_mutation_;          /* probability of Mutation */
	double random_seed_;

	int max_generations_; /* when to stop */
	int population_size_;/* population size */

	std::string ofile_;  /* output File name */

	void evolve();
	void print_statistics(std::ostream& out);
	void print_properties(std::ostream& out);
	friend std::ostream& operator <<(std::ostream& out,const species& rhs);
	double highest_ever_fitness_;
	int highest_ever_generation_;
private:
	double sum_fitness_; /* statistics parameters for selection and tracking*/

	int highest_ever_index_;

	int max_index_; /* index of best individual in current population*/
	int current_max_;
	int min_index_; /* index of worst individual in current population*/
	int current_min_;
	int64_t average_;

	void UpdateFitnessesAndBlock(bool initial);
};

#endif //SPECIES_H

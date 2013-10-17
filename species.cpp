#include "species.h"

#define BIG 5000

bool dbg1 = false;
bool elite_selection = true;

species::species(const char in_file[]) {
	char dummy[20];
	std::ifstream fin;
	fin.open(in_file);

	if(!fin.good()) {
		std::cout<<"could not open"<<in_file<<std::endl;
		assert(false);
	}
	fin>>chromosome_length_; /*get comment*/ fin.getline(dummy,20,'\n');
	fin>>population_size_; /*get comment*/ fin.getline(dummy,20,'\n');
	fin>>max_generations_; /*get comment*/ fin.getline(dummy,20,'\n');
	fin>>prob_crossover_; /*get comment*/ fin.getline(dummy,20,'\n');
	fin>>prob_mutation_; /*get comment*/ fin.getline(dummy,20,'\n');
	fin>>ofile_;

	highest_ever_fitness_ = -1;
	highest_ever_generation_=0;
	highest_ever_index_=0;
	max_index_=0;
	min_index_=0;
	current_max_=0;
	current_min_=1000;
	generation_ = 0;
	average_ = 0;
	sum_fitness_=0;
	if(!fin.good()) {
		std::cout<<in_file<<" was poorly formatted"<<std::endl;
		assert(false);
	}

	//initialize the population
	for(int i=0;i<population_size_;i++) {
		//intitialize each member of the population
		//note that the fitnesses will not be calculated yet
		boost::shared_ptr<individual> temp(new individual);
		temp->init(chromosome_length_);
		population_.push_back(temp);
	}
	//wait for all the fitnesses to be calculated
	UpdateFitnessesAndBlock(true);
	return;

}

void species::evolve() {
	std::vector<boost::shared_ptr<individual> > new_set;
	//selection
	//compute the sum of the fitnesses
	double fitness_sum=0;
	for(int i=0;i<population_size_;i++) {
		fitness_sum+= population_[i]->fitness_;
	}
	//get a number of successors
	for(int i=0;i<population_size_;i++) {
		//select a random number 0 < X < fitness_sum
		int target_value = rand()%int(fitness_sum+1);
		//search until that target is found
		int found_at=0;
		while(target_value >= 0 && found_at<population_size_) {
			if(dbg1) std::cout<<"subtraction target_value: "<<target_value<<" found at: "<<found_at<<std::endl;
			target_value -= population_[found_at++]->fitness_;
		}
		//add the selected element to the new set
		found_at--;
		boost::shared_ptr<individual> temp(new individual);
		population_[found_at]->CopyTo(&(*temp));
		temp->fitness_ = -1;
		new_set.push_back(temp);
	}
	//crossover
	//select prob_crossover*population_size successors and perform crossover
	//make a list of successors to crossover
	std::vector<int> successors_to_cross;
	for(int i=0;i<prob_crossover_*population_size_;i++) {
		successors_to_cross.push_back(rand()%population_size_);
	}
	if(successors_to_cross.size()<2) {
		std::cout<<"Warning: crossover rate too low"<<std::endl;
	}

	//select pairs from the list and cross them over
	while(successors_to_cross.size() > 1) {
		int successor_1=rand()%successors_to_cross.size();
		int successor_2=rand()%successors_to_cross.size();
		if(successor_1==successor_2) {
			//disallow asexual reproduction (duplicate parent)
			continue;
		}
		//do the crossover
		new_set[successor_1]->crossover(new_set[successor_2]);
		//remove the parents from the open list
		if(successor_1 > successor_2) {
			successors_to_cross.erase(successors_to_cross.begin()+successor_1);
			successors_to_cross.erase(successors_to_cross.begin()+successor_2);
		}
		else {
			successors_to_cross.erase(successors_to_cross.begin()+successor_2);
			successors_to_cross.erase(successors_to_cross.begin()+successor_1);
		}

	}
	//mutation
	//select length_*population_size*mutation_rate bits to flip
	for(int i=0;i<population_size_*chromosome_length_*prob_mutation_;i++) {
		//flip a random bit in a random individual
		int individual_to_flip = rand()%population_size_;
		int bit_to_flip = rand()%chromosome_length_;
		new_set[individual_to_flip]->chromosome_[bit_to_flip] = 1-new_set[individual_to_flip]->chromosome_[bit_to_flip];
	}
	if(elite_selection) {
		//copy the top into the first slot
		boost::shared_ptr<individual> temp(new individual);
			population_[max_index_]->CopyTo(&(*temp));
			temp->fitness_ = -1;
			new_set[0] = temp;
	}
	//swap the new list with the old list, causing the old list to be deallocated soon
	new_set.swap(population_);
	//calculate updated fitness
	UpdateFitnessesAndBlock(false);
}
void species::print_statistics(std::ostream& out) {
	out<<std::setw(5)<<std::left<<generation_<<",";
	out<<std::setw(4)<<std::left<<current_max_<<",";
	out<<std::setw(4)<<std::left<<current_min_<<",";
	out<<std::setw(4)<<std::left<<average_<<",";
	out<<std::setw(4)<<std::left<<highest_ever_fitness_<<",";
	out<<std::setw(4)<<std::left<<highest_ever_generation_<<std::endl;
}
void species::print_properties(std::ostream& out) {
	out<<"Chromosome length: "<<chromosome_length_<<std::endl;
	out<<"Pop size: "<<population_size_<<std::endl;
	out<<"Max gen: "<<max_generations_<<std::endl;
	out<<"prob cross: "<<prob_crossover_<<std::endl;
	out<<"prob mutation: "<<prob_mutation_<<std::endl;
	out<<"Seed: "<<random_seed_<<std::endl;

}
std::ostream& operator <<(std::ostream& out,const species& rhs) {
	for(int i=0;i<rhs.population_size_;i++) {
		out<<*(rhs.population_[i])<<std::endl;
	}
	return out;
}



void species::UpdateFitnessesAndBlock(bool initial) {
	//Evaluate the fitness. This function returns immediately,
	//finishing the initialization. The calling function must verify that the
	//initialization is complete before resuming by verifying fitness_!=DIRTY
	for(int i=0;i<population_size_;i++) {
		if(dbg1) std::cout<<"calcing fitness "<<i<<std::endl;
		eval(&(*population_[i]));
	}
	current_max_ = -1;
	current_min_ = BIG;
	//wait for all the fitnesses to be calculated
	//this could be done with a thread join, but this works, too
	sum_fitness_ = 0;
	for(int i=0;i<population_size_;i++) {
		//if the fitness is DIRTY wait a bit
		while(population_[i]->fitness_==-1) usleep(10);

		//then do statistics on it
		//test for max in generation
		if(population_[i]->fitness_ > current_max_) {
			current_max_ = population_[i]->fitness_;
			max_index_ = i;
		}
		//test for min in generation
		if(population_[i]->fitness_ < current_min_) {
			current_min_ = population_[i]->fitness_;
			min_index_ = i;
		}
		//test for max ever
		if(population_[i]->fitness_ > highest_ever_fitness_) {
			//this is the best yet
			highest_ever_fitness_ = population_[i]->fitness_;
			highest_ever_generation_ = generation_;
			highest_ever_index_ = i;
		}
		//update sum for averaging
		if(!initial) sum_fitness_+= population_[i]->fitness_;
	}
	//compute average
	average_ = sum_fitness_/(population_size_);
}

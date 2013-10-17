#include <fstream>
#include <iostream>
#include <sstream>
#include "species.h"
#include "utility.h"

bool dbg = false;

int main(int argc, char* argv[]) {

	//start timer
	struct timeval start,end;
	long mtime,seconds,useconds;
	gettimeofday(&start,NULL);

	//check for infile and seed parameter
	//open logging file
	std::ofstream fout;

	if(argc<3) {
		std::cout<<"Usage: ./ga infile(string) seed(int)";
		return 0;
	}

	int seed;
	//initialize random seed
	if(atoi(argv[2])==0) {
		//special case for true random seed
		seed = time(NULL)%1000000;
	}
	else {
		seed = atoi(argv[2]);
	}
	srand(seed);

	//initialize the colony
	species colony(argv[1]);
	colony.random_seed_ = seed;

	//modify infile name to make it file compatible
	if(argv[1][0]!='\0') {
		argv[1][0]='_';
		if(argv[1][1]!='\0') {
			argv[1][1]='_';
			if(argv[1][2]!='\0') {
				argv[1][2]='_';
			}
		}
	}

	std::stringstream name;
	name<<"p"<<colony.population_size_<<"_m"<<colony.max_generations_<<"_px"<<colony.prob_crossover_<<"_pm"<<
			colony.prob_mutation_<<"_s"<<colony.random_seed_<<"_if"<<argv[1];
	if(argc==4) {
		name<<"_"<<argv[3];
	}

//	fout = std::cout; //switch to output to screen
	fout.open(name.str().c_str());

	colony.print_properties(fout); fout<<std::endl;

	//print the initial colony
//	fout<<colony<<std::endl<<std::endl;


	print_statistics_headers(fout);

	//open log
	std::ofstream log_out;
	std::stringstream log_name;
	log_name<<"log_out_";//<<argv[3];
	log_out.open(log_name.str().c_str(),std::ios::app);


	//create life
	for(colony.generation_=0; colony.generation_<colony.max_generations_; colony.generation_++) {

		//do selection/crossover/mutation
		if(colony.highest_ever_fitness_!=64) colony.evolve();
		//print out all the individuals
//		fout<<colony;
		//print the max ever, etc.
//		colony.print_statistics(fout);
		colony.print_statistics(log_out);

	}
	//stop the clock
	gettimeofday(&end,NULL);
	seconds = end.tv_sec - start.tv_sec;
	useconds = end.tv_usec - start.tv_usec;
	mtime = ((seconds)*1000 + useconds/1000.)+0.5;
	std::cout<<"Max fitness: "<<colony.highest_ever_fitness_<<" from generation: "<<colony.highest_ever_generation_<<std::endl;

	//make another file with the result
	name<<"_r"<<colony.highest_ever_fitness_;
	std::ofstream fout2;
	fout2.open("log",std::ios::app);
	std::cout<<"Time: "<<mtime<<" milliseconds"<<std::endl;
	print_statistics_headers(fout2);
	colony.print_statistics(fout2);
	fout2<<mtime<<std::endl;
	fout2<<std::endl;
	fout2<<colony<<std::endl;
	colony.print_properties(fout2);
}

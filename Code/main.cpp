#include <iostream>
#include <fstream>
#include <cmath>
#include <omp.h>

#include "integrator_mh.h"
#include "integrator_wolff.h"
#include "square_lattice.h"
#include "triangular_lattice.h"
#include "honeycomb_lattice.h"
#include "ising.h"
#include "potts.h"
#include "config_file.h"
#include "config_file.tpp"

template<typename T>
int save(const std::vector<T>& data, const std::string& filename) {
	std::ofstream file(filename, std::ios::out | std::ios::binary);

	if (file.fail()) {
		std::cerr << "Cannot open the file " << filename << std::endl;
		return -1;
	}

	for (auto const& el : data) {
		file.write(reinterpret_cast<const char*>(&el), sizeof(el));
	}

	file.close();
	return 0.0;
}

template<typename T>
int save(const std::vector<std::vector<T>>& data, const std::string& filename) {
	std::ofstream file(filename, std::ios::out | std::ios::binary);

	if (file.fail()) {
		std::cerr << "Cannot open the file " << filename << std::endl;
		return -1;
	}

	for (auto const& row : data) {
		for (auto const& spin : row) {
			file.write(reinterpret_cast<const char*>(&spin), sizeof(spin));
		}
	}

	file.close();
	return 0.0;
}

int main(int argc, char** argv) {
	Config_file conf("config.in");
	for (int i=1; i<argc; ++i) {
		conf.insert(split_on(argv[i], '='));
	}

	conf.print();

	const size_t h = conf.get<size_t>("h");
	const size_t w = h;
	const double J = conf.get<double>("J");
	const bool save_lattice = conf.get<bool>("save_L");
	const bool save_EM = conf.get<bool>("save_EM");
	//const double beta_c = log(1.0 + sqrt(2.0))/2.0;

	const double beta_low  = conf.get<double>("beta_low");
	const double beta_up   = conf.get<double>("beta_up");
	const size_t nbr_beta  = conf.get<size_t>("nbr_step");
	const double beta_step = (beta_up - beta_low) / nbr_beta;

	const std::string lattice_type = conf.get<std::string>("lattice");
	const std::string model_type = conf.get<std::string>("model");
	const std::string integrator_type = conf.get<std::string>("integrator");

	const std::string path_save = conf.get<std::string>("path_save");

	omp_set_num_threads(conf.get<size_t>("threads"));
	#pragma omp parallel for
	for (size_t i=0; i<nbr_beta; ++i) {
		std::vector<int> M;
		std::vector<double> E;
		//const double beta = log(1+sqrt(2))/2.0;
		const double beta = beta_low + i * beta_step;
		std::cout << i << ": " << beta << std::endl;

		Lattice* lattice = nullptr;
		if (lattice_type == "honeycomb")
			lattice = new Honeycomb_lattice(h, w, J);
		else if (lattice_type == "square")
			lattice = new Square_lattice(h, w, J);
		else if (lattice_type == "triangular")
			lattice = new Triangular_lattice(h, w, J);
		else
			throw std::invalid_argument("unknown lattice type");


		Model* model = nullptr;
		if (model_type == "ising") {
			lattice -> random_init({-1, 1});
			model = new Ising(*lattice, beta, true);
		}
		else if (model_type == "potts") {
			std::vector<int> qs;
			for (size_t i=0; i<conf.get<size_t>("q"); ++i)
				qs.push_back(i+1);
			lattice -> random_init(qs);
			model = new Potts(*lattice, beta, false, conf.get<size_t>("q"));
		}
		else
			throw std::invalid_argument("unknown model type");
	
		model -> compute_energy();
		model -> compute_M();

		//std::cout << "init: " << model -> get_E() << std::endl;

		if (save_lattice) {
			save(lattice -> get_lattice(), path_save +
						    std::string("lattice0_") +
						    std::string(std::to_string(beta)) +
						    std::string("_.dat"));
		}
		
		bool mh = false;
		Integrator* integrator = nullptr;
		if (integrator_type == "mh") {
			integrator  = new Integrator_mh();
			mh = true;
		}
		else if (integrator_type == "wolff") {
			if (model_type == "ising")
				integrator  = new Integrator_wolff(1-exp(-2*J*beta));
			else
				integrator  = new Integrator_wolff(1-exp(-J*beta));
		}
		else
			throw std::invalid_argument("unknown integrator type");

		std::random_device rd;
		integrator -> set_random_generator(rd, *lattice);
		
		M.push_back(model -> get_M());
		E.push_back(model -> get_E());

		size_t j = 0;
		const size_t burning = conf.get<size_t>("burning"); //pow(2, 20);
		const size_t nbr_sample = conf.get<size_t>("nbr_data"); //pow(2, 16);

		while (E.size() < nbr_sample) {
			integrator -> step(*model, *lattice);

			if (j>burning) {
				if (mh && (j % 2*h*h == 0)) {
					M.push_back(model -> get_M());
					E.push_back(model -> get_E());
				}
				else {
					model -> compute_energy();
					M.push_back(model -> get_M());
					E.push_back(model -> get_E());
				}
			}
			++j;
		}
		//lattice.pretty_print();

		if (save_lattice) {
			save(lattice -> get_lattice(), path_save +
						    std::string("lattice_") +
						    std::string(std::to_string(beta)) +
						    std::string("_.dat"));
		}

		if (save_EM) {
			save(M, path_save +
				std::to_string(h) + std::string("/M_") +
				std::string(std::to_string(beta)) +
				std::string("_.dat"));

			save(E, path_save + 
				std::to_string(h) + std::string("/E_") +
				std::string(std::to_string(beta)) +
				std::string("_.dat"));
		}
	}

	return 0;
}

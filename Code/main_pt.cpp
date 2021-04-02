#include <iostream>
#include <fstream>
#include <cmath>
#include <omp.h>
#include <tuple>
#include <algorithm>
#include <pybind11/numpy.h>
#include <pybind11/embed.h>
#include <pybind11/stl.h>

#include "integrator_mh.h"
#include "integrator_wolff.h"
#include "square_lattice.h"
#include "ising.h"
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

enum State {UP, DOWN, MIDDLE};

namespace py = pybind11;

int main(int argc, char** argv) {

	py::scoped_interpreter guard{};
	
	Config_file conf(argv[1]);
	for (int i=2; i<argc; ++i) {
		conf.insert(split_on(argv[i], '='));
	}

	conf.print();

	const size_t h = conf.get<size_t>("h");
	const size_t w = h;
	const double J = conf.get<double>("J");
	//const double beta_c = log(1.0 + sqrt(2.0))/2.0;

	const double beta_low  = conf.get<double>("beta_low");
	const double beta_up   = conf.get<double>("beta_up");
	const size_t nbr_beta  = conf.get<size_t>("nbr_step");
	const double beta_step = (beta_up - beta_low) / nbr_beta;

	const std::string path_save = conf.get<std::string>("path_save");


	std::vector<double> Ts(nbr_beta);

	for (size_t i=0; i<nbr_beta; ++i) {
		Ts[i] = 1.0309 + 0.2989*i;
	}


	std::vector<double> betas(nbr_beta);
	for (size_t i=0; i<nbr_beta; ++i)
		betas[i] = 1.0/Ts[i];
		//betas[i] = beta_low + i*beta_step;

	std::reverse(betas.begin(), betas.end());

	std::vector<Square_lattice> lattices;
	for (size_t i=0; i<nbr_beta; ++i) {
		lattices.push_back(Square_lattice(h, w, J));
	}
	for (auto& lattice : lattices)
		lattice.random_init();

	size_t iteration = 0;
	while (iteration < conf.get<size_t>("nbr_iter")) {
		++iteration;
		std::vector<State> state(nbr_beta, MIDDLE);
		state[0] = DOWN;
		state.back() = UP;

		std::vector<size_t> n_down(nbr_beta, 0);
		std::vector<size_t> n_up(nbr_beta, 0);

		n_up.back() = 1;
		n_down[0] = 1;


		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<size_t> dis(1, nbr_beta-1);
		std::uniform_real_distribution<float> p(0.0, 1.0);

		omp_set_num_threads(conf.get<size_t>("threads"));
		for (size_t sw=0; sw<conf.get<size_t>("nbr_sweep"); ++sw) {
			if (sw % 1000 == 0)
				std::cout << sw << std::endl;

			std::vector<std::tuple<int, double, double>> summary(nbr_beta);

			#pragma omp parallel for
			for (size_t i=0; i<nbr_beta; ++i) {
				const double beta = betas[i];

				auto integrator  = Integrator_mh();
				//auto integrator  = Integrator_wolff(beta, J);
				auto model = Ising(lattices[i], beta, true);

				std::random_device rd;
				integrator.set_random_generator(rd, lattices[i]);
				
				size_t j = 0;

				while (j < conf.get<size_t>("nbr_sample")) {
					integrator.step(model, lattices[i]);
					++j;
				}

				summary[i] = std::make_tuple(i, beta, model.get_E());
			}

			/*	
			if (sw % 1000 == 0){
			std::cout << std::endl;
			for (size_t i=0; i<summary.size(); ++i) 
				std::cout << std::get<0>(summary[i]) << "," <<
					     std::get<1>(summary[i]) << "," << 
					     std::get<2>(summary[i]) << std::endl;
			}
			*/

			int nbr_swap_done = 0;
			for (size_t N=0; N<conf.get<size_t>("nbr_swap"); ++N) {
				const size_t candidate = dis(gen);
				const double dE = std::get<2>(summary[candidate]) - std::get<2>(summary[candidate-1]);
				const double dbeta = std::get<1>(summary[candidate]) - std::get<1>(summary[candidate-1]);

				if (p(gen) < exp(dbeta * dE)) {
					++nbr_swap_done;
					std::swap(lattices[candidate], lattices[candidate-1]);
					std::swap(state[candidate], state[candidate-1]);
					const double E_temp = std::get<2>(summary[candidate]);
					std::get<2>(summary[candidate]) = std::get<2>(summary[candidate-1]);
					std::get<2>(summary[candidate-1]) = E_temp;
					//std::cout << candidate << " <--> " << candidate-1 << std::endl;
				}
				if (state[0] != DOWN)
					state[0] = DOWN;
				if (state.back() != UP)
					state.back() = UP;

				for (size_t jj=0; jj<nbr_beta; ++jj) {
					if (state[jj] == UP)
						++n_up[jj];
					if (state[jj] == DOWN)
						++n_down[jj];
				}
			}

			/*
			if (sw % 1000 == 0) {
				std::cout << "Nbr swap done: " << nbr_swap_done << std::endl;

				for (auto const& el : state)
					std::cout << el << std::endl;
			}
			*/
		}	

		for (auto const& el : state)
			std::cout << el << std::endl;

		for (size_t i=0; i<nbr_beta; ++i) {
			std::cout << betas[i] << ", " << n_up[i] << ", " << n_down[i] << std::endl;
		}

		py::module myModule = py::module::import("plot_eta_module");
		auto process = myModule.attr("process");
		py::array_t<double> py_betas  = py::cast(betas);
		py::array_t<double> py_n_up   = py::cast(n_up);
		py::array_t<double> py_n_down = py::cast(n_down);

		betas = (process(py_betas, py_n_up, py_n_down)).cast<std::vector<double>>();
	
	}
	return 0;
}

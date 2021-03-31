#include "integrator_mh.h"
#include <iostream>
#include <cmath>
#include <algorithm>

std::pair<int, std::vector<int>> Integrator_mh::step(Model& model, Lattice& lattice)
{
	const size_t i = height(gen);
	const size_t j = width(gen);

	const std::vector<int> spins = lattice.get_spins(i, j);
	const int proposition = model.spin_proposition(spins.back());

	const int key = model.sum_spins(spins);
	const double dE = model.delta_energy(i, j, proposition);
	const double exponent = model.exponent(dE);

	//std::cout << model.get_E() << ":: ";
	if (dE <= 0) {
		lattice.change_spin(i, j, proposition); 
		model.update_energy(dE);
		model.update_M(spins.back(), proposition);
	}
	else {
		double A = 0.0;
		if (model.get_prob().empty()) {
			A = exp(exponent);
		}
		else {
			A = model.get_prob().at(key);
		}

		if (unif(gen) < A) {
			lattice.change_spin(i, j, proposition);  
			model.update_energy(dE);
			model.update_M(spins.back(), proposition);
		}
		else {
			//std::cout << std::endl;
			return std::make_pair(-1, std::vector<int>());
		}
	}

	//std::cout << dE << ", " << model.get_E() << ", " ;
	//model.compute_energy();
	//std::cout << " model " << model.get_E() << std::endl;

	return std::make_pair(i + j*(lattice.get_width()), std::vector<int>());
}

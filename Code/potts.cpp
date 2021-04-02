#include "potts.h"
#include <vector>
#include <algorithm>
#include <cmath>

double Potts::delta_energy(const size_t& i, const size_t& j, const double& proposition) const {
	std::vector<int> spins = lattice.get_spins(i, j);

	const double H_0 = local_energy(spins);
	spins.back() = proposition;

	const double H_new = local_energy(spins);

	return H_new - H_0;
}

double Potts::local_energy(const std::vector<int>& spins) const {
	return -lattice.get_J() * sum_spins(spins);
}

void Potts::create_memoise() {
	for (size_t i=0; i<lattice.get_coordination()+1; ++i)
		memoise.insert({i, exp(-beta * lattice.get_J() * i)});
}

int Potts::sum_spins(const size_t&i, const size_t& j) const {
	std::vector<int> spins = lattice.get_spins(i, j);
	
	int sum = 0;

	for (auto el=spins.begin(); el!=spins.end()-1; ++el)
		sum += kronecker(*el, spins.back());

	return sum;
}

int Potts::sum_spins(const std::vector<int>& spins) const {
	int sum = 0;

	for (auto el=spins.begin(); el!=spins.end()-1; ++el)
		sum += kronecker(*el, spins.back());

	return sum;
}

int Potts::spin_proposition(const int& current_spin) {
	std::vector<size_t> possibilities;
	std::copy_if(qs.begin(), qs.end(), std::back_inserter(possibilities), 
				[current_spin](auto t) { return t != current_spin; });

	const size_t idx = choose_q(gen);
	//for (auto const& el : possibilities)
	//	std::cout << el << ", ";
	//std::cout << " :: ";
	//std::cout << idx << std::endl;

	return possibilities[idx];
}

void Potts::update_M(const int& previous, const int& next) {
	--N[previous-1];
	++N[next-1];
}

void Potts::update_M(const int& previous, const int& next, const size_t& size) {
	N[(previous-1)] -= size;
	N[(next-1)]     += size;
}

double Potts::get_M() const {
	double sum_cos = 0.0;
	double sum_sin = 0.0;
	for (size_t q=0; q<qs.size(); ++q) {
		sum_cos += N[q]*cos(theta_i[q]);
		sum_sin += N[q]*sin(theta_i[q]);
	}

	return sqrt(pow(sum_cos, 2) + pow(sum_sin, 2));
}

void Potts::compute_M() {
	for (auto& el : N)
		el = 0;
	for (size_t i=0; i<lattice.get_width(); ++i) {
		for (size_t j=0; j<lattice.get_height(); ++j) {
			const size_t spin = lattice.get_spin(i, j);
			++N[spin-1];
		}
	}
}

size_t Potts::get_colour(const int& spin) const {
	return spin;
}

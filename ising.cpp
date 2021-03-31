#include "ising.h"
#include <numeric>
#include <map>
#include <cmath>

int Ising::sum_spins(const size_t& i, const size_t& j) const {
	const std::vector<int> spins = lattice.get_spins(i, j);
	//const int res = (spins[0]+spins[1]+spins[2]+spins[3])*spins[4];
	//return res;
	return std::accumulate(spins.cbegin(), spins.cend()-1, 0.0) * spins.back();
}

int Ising::sum_spins(const std::vector<int>& spins) const {
	return std::accumulate(spins.cbegin(), spins.cend()-1, 0.0) * spins.back();
}

double Ising::delta_energy(const size_t& i, const size_t& j, [[maybe_unused]] const double& proposition) const {
	return 2 * lattice.get_J() * sum_spins(i, j);
}

double Ising::local_energy(const std::vector<int>& spins) const {
	return -lattice.get_J() * sum_spins(spins);
}

double Ising::exponent(const double& dE) const {
	return -beta * dE;
}

void Ising::create_memoise() {
	for (auto const& el: lattice.get_comb()) {
		memoise.insert({el, exp(-2 * beta * lattice.get_J() * el)});		
	}
}

int Ising::spin_proposition(const int& current_spin) {
	return -current_spin;
}

void Ising::update_M(const int& previous, const int& next) {
	--N[(previous+1)/2];
	++N[(next+1)/2];
}

void Ising::update_M(const int& previous, const int& next, const size_t& size) {
	N[(previous+1)/2] -= size;
	N[(next+1)/2]     += size;
}
	
double Ising::get_M() const {
	return -N[0] + N[1];
}
		
void Ising::compute_M() {
	N = {0, 0};
	for (size_t i=0; i<lattice.get_width(); ++i) {
		for (size_t j=0; j<lattice.get_height(); ++j) {
			const int spin = lattice.get_spin(i, j);
			++N[(spin+1)/2];
		}
	}
}

size_t Ising::get_colour(const int& spin) const {
	return (spin + 1)/2;
}

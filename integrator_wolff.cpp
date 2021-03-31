#include "integrator_wolff.h"
#include <utility>
#include <set>
#include <vector>

typedef std::set<std::pair<size_t, size_t>> cluster;

std::pair<int, std::vector<int>> Integrator_wolff::step(Model& model, Lattice& lattice) {
	const size_t i = height(gen);
	const size_t j = width(gen);

	cluster C = {std::make_pair(i, j)};
	cluster F_old = {std::make_pair(i, j)};

	while (!F_old.empty()) {
		cluster F_new;
		for (auto const& el : F_old) {
			std::vector<int> neighbours = lattice.get_spins(el.first, el.second);
			list_index neighbours_idx = lattice.get_spins_idx(el.first, el.second);
			for (size_t k=0; k<neighbours.size()-1; ++k) {
				if (neighbours[k] == neighbours.back()) {
					if (unif(gen) < p) {
						if (C.insert(neighbours_idx[k]).second)
							F_new.insert(neighbours_idx[k]);
					}
				}
			}
		}
		F_old = F_new;
	}
	
	const int previous = lattice.get_spin(C.begin()->first, C.begin()->second);
	const int proposition = model.spin_proposition(previous);
	for (auto const& el : C) {
		lattice.change_spin(el.first, el.second, proposition); 
	}	

	model.update_M(previous, proposition, C.size());
	//model.compute_energy();

	std::vector<int> res;
	for (auto const& el : C) {
		res.push_back(el.first + lattice.get_width()*el.second);
	}

	return std::make_pair(-1, res);
}

bool in(const list_index& pairs, const std::pair<size_t, size_t>& pair) {
	for (auto const& el : pairs) {
		if (el == pair) {
			return true;
		}
	}
	return false;
}

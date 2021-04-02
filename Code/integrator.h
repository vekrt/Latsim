#ifndef INTEGRATOR_CLASS_H
#define INTEGRATOR_CLASS_H

#include "lattice.h"
#include "model.h"
#include <memory>
#include <utility>
#include <vector>

class Integrator {
	public:
		Integrator(): gen(rd()), 
			      unif(std::uniform_real_distribution<>(0.0, 1.0)), 
			      height(std::uniform_int_distribution<>(0,1)), 
			      width(std::uniform_int_distribution<>(0,1)) {}
		virtual std::pair<int, std::vector<int>> step(Model&, Lattice&) = 0;
		void set_random_generator(std::random_device& seed, const Lattice& lattice) {
			gen.seed(seed());
			height = std::uniform_int_distribution<>(0, lattice.get_height()-1);
			width = std::uniform_int_distribution<>(0, lattice.get_width()-1);
		}
		virtual ~Integrator() {}
	protected:
		std::random_device rd;
		std::mt19937 gen;
		std::uniform_real_distribution<> unif;
		std::uniform_int_distribution<> height;
		std::uniform_int_distribution<> width;
};

#endif //INTEGRATOR_CLASS_H

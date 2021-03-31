#ifndef INTEGRATOR_MH_CLASS_H
#define INTEGRATOR_MH_CLASS_H

#include "integrator.h"
#include <random>
#include <memory>

class Integrator_mh : public Integrator {
	public:
		Integrator_mh() : Integrator() {}
		virtual std::pair<int, std::vector<int>> step(Model& model, Lattice& lattice) override;
	private:

};

#endif //INTEGRATOR_MH_CLASS_H

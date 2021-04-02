#ifndef INTEGRATOR_WOLFF_CLASS_H
#define INTEGRATOR_WOLFF_CLASS_H

#include "integrator.h"

typedef std::vector<std::pair<size_t, size_t>> list_index;

class Integrator_wolff : public Integrator {
	public:
		Integrator_wolff(const double& p_): 
							Integrator(),
							p(p_) {}
		virtual std::pair<int, std::vector<int>> step(Model& model, Lattice& lattice) override;
	private:
		double p;
};

bool in(const list_index& pairs, const std::pair<size_t, size_t>& pair);

#endif //INTEGRATOR_WOLFF_CLASS_H

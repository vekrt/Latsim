#ifndef ISING_CLASS_H
#define ISING_CLASS_H

#include "model.h"

class Ising: public Model {
	public:
		Ising(Lattice& lattice, const double& beta_, const bool& memoise): 
			Model(lattice, beta_) {
			N = {0, 0};
			qs = {-1, 1};
			if (memoise)
				create_memoise();
			compute_energy();
			compute_M();
		}

		virtual double delta_energy(const size_t& i, const size_t& j, [[maybe_unused]] const double& proposition) const override;
		virtual double local_energy(const std::vector<int>& spins) const override;
		virtual double exponent(const double& dE) const override;
		virtual void create_memoise() override;
		virtual int sum_spins(const size_t&i, const size_t& j) const override;
		virtual int sum_spins(const std::vector<int>& spins) const override;
		virtual int spin_proposition(const int& current_spin) override;
		virtual void update_M(const int& previous, const int& next) override;
		virtual void update_M(const int& previous, const int& next, const size_t& size) override;
		virtual double get_M() const override;
		virtual void compute_M() override;
		virtual size_t get_colour(const int& spin) const override;
	private:
};

#endif //ISING_CLASS_H

#ifndef POTTS_CLASS_H
#define POTTS_CLASS_H

#include "model.h"
#include <random>

#define _2PI 2*M_PI

class Potts: public Model {
	public:
		Potts(Lattice& lattice, const double& beta_, const bool& memoise, const size_t& q_): 
			Model(lattice, beta_), 
			gen(rd()),
			choose_q(std::uniform_int_distribution<>(0,1)) {
				N = std::vector<int>(q_, 0);
				for (size_t q=1; q<q_+1; ++q) {
					qs.push_back(q);
					theta_i.push_back(2*M_PI*(q-1)/q_);
				}
				choose_q = std::uniform_int_distribution<>(0, q_-2);
			
				if (memoise)
					create_memoise();
				compute_energy();
				compute_M();
		}

		virtual double delta_energy(const size_t& i, const size_t& j, const double& proposition) const override;
		virtual double local_energy(const std::vector<int>& spins) const override;
		virtual void create_memoise() override;
		virtual int sum_spins(const size_t&i, const size_t& j) const override;
		virtual int sum_spins(const std::vector<int>& spins) const override;
		virtual int spin_proposition(const int& current_spin);
		virtual void update_M(const int& previous, const int& next) override;
		virtual void update_M(const int& previous, const int& next, const size_t& size) override;
		virtual double get_M() const override;
		virtual void compute_M() override;
		virtual size_t get_colour(const int& spin) const override;
	private:
		std::vector<double> theta_i;
		std::random_device rd;
		std::mt19937 gen;
		std::uniform_int_distribution<> choose_q;
};

inline int kronecker(const int& a, const int& b) {
	return (a == b);
}
 
#endif //POTTS_CLASS_H

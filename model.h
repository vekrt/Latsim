#ifndef MODEL_CLASS_H
#define MODEL_CLASS_H

#include <vector>
#include <map>
#include "lattice.h"
#include <memory>

typedef std::map<int, double> comb;

class Model {
	public:
		Model(Lattice& lattice_, const double& beta_):
					lattice(lattice_),
					beta(beta_), E(0.0) {}
 		virtual ~Model() {}
		virtual double delta_energy(const size_t& i, const size_t& j, const double& proposition) const = 0;
		virtual double local_energy(const std::vector<int>& spins) const = 0;
		virtual double exponent(const double& dE) const {return -beta * dE;};
		virtual void create_memoise() = 0;
		comb get_prob() const {return memoise;}
		virtual int sum_spins(const size_t& i, const size_t& j) const = 0;
		virtual int sum_spins(const std::vector<int>& spins) const = 0;
		virtual void compute_energy() {
			E = 0;
			for (size_t i=0; i<lattice.get_width(); ++i) {
				for (size_t j=0; j<lattice.get_height(); ++j) {
					E += local_energy(lattice.get_spins(i, j));
				}
			}
			E /= 2.0;
		}
		void update_energy(const double& dE) {E += dE;}
		double get_E() const {return E;}
		virtual int spin_proposition(const int& current_spin) = 0;
		virtual void update_M(const int& previous, const int& next) = 0;
		virtual void update_M(const int& previous, const int& next, const size_t& size) = 0;
		virtual double get_M() const = 0;
		virtual void compute_M() = 0;

		std::vector<int> get_N() const {return N;}

		std::vector<int> get_qs() const {return qs;}

		virtual size_t get_colour(const int& spin) const = 0;
	protected:
		Lattice& lattice;
		const double beta;
		comb memoise;
		double E;
		std::vector<int> N;
		std::vector<int> qs;
};

#endif //MODEL_CLASS_H

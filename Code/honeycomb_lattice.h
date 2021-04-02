#ifndef HONEYCOMB_LATTICE_CLASS_H
#define HONEYCOMB_LATTICE_CLASS_H

#include "lattice.h"
#include "renderable.h"

class Honeycomb_lattice : public Lattice {
	public:
		Honeycomb_lattice(const size_t& h, const size_t& w, const double& J_): 
				Lattice(h, w, J_) {coordination = 3;}
		virtual std::vector<int> get_spins(const size_t&, const size_t&) const override;
		virtual list_index  get_spins_idx(const size_t& i, const size_t& j) const override;
		virtual std::vector<int> get_comb() const override;
		virtual std::pair<size_t, size_t> draw(Renderer& renderer) const override;
		virtual void update(Renderer& renderer) const override;

		~Honeycomb_lattice() {}
	private:
};

inline int parity(const size_t& i) {
	return 1 - 2*(i%2);
}

#endif // HONEYCOMB_LATTICE_CLASS_H

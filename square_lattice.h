#ifndef SQUARE_LATTICE_CLASS_H
#define SQUARE_LATTICE_CLASS_H

#include "lattice.h"

class Square_lattice : public Lattice {
	public:
		Square_lattice(const size_t& h, const size_t& w, const double& J_): 
				Lattice(h, w, J_) {coordination = 4;}
		virtual std::vector<int> get_spins(const size_t&, const size_t&) const override;
		virtual list_index  get_spins_idx(const size_t& i, const size_t& j) const override;
		virtual std::vector<int> get_comb() const override;
		virtual std::pair<size_t, size_t> draw(Renderer& renderer) const override;
		virtual void update(Renderer& renderer) const override;

		~Square_lattice() {}
	private:
};

#endif // SQUARE_LATTICE_CLASS_H

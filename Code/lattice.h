#ifndef LATTICE_CLASS_H
#define LATTICE_CLASS_H

#include <vector>
#include <random>
#include <iostream>
#include <iomanip>
#include <utility>
#include "renderable.h"

typedef std::vector<std::vector<int>> lattice_data;
typedef std::vector<std::pair<size_t, size_t>> list_index;

class Lattice : public Renderable {
	public:
		Lattice(const size_t& h, const size_t& w, const double& J_): 
			height(h), 
			width(w),
			lattice(std::vector(h, std::vector<int>(w))),
			J(J_)
			{}
		virtual std::vector<int> get_spins(const size_t& i, const size_t& j) const = 0;
		virtual list_index get_spins_idx(const size_t& i, const size_t& j) const = 0;
		virtual std::vector<int> get_comb() const = 0;
		void random_init(const std::vector<int>& qs) {
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<> idx(0, qs.size()-1);
			for (auto& row : lattice) {
				for (auto& spin : row) {
					spin = qs[idx(gen)];
				}
			}
		};
		void fixed_init(const int& value) {
			for (auto& row : lattice) {
				for (auto& spin : row) {
					spin = value;
				}
			}
		}
		virtual void change_spin(const size_t& i, const size_t& j, const int& value) {
			lattice[i][j] = value;
		}
		size_t get_height() const {return height;}
		size_t get_width()  const {return width; }
		double get_J() const {return J;}
		void pretty_print() const {
			for (auto const& row : lattice) {
				for (auto const& spin : row) {
					std::cout << spin << ", " << std::setw(2);
				}
			std::cout << std::endl;
			}
		}
		std::ostream& show(std::ostream& out) const {
			for (auto const& row : lattice) {
				for (auto const& spin : row) {
					out << spin;
				}
			}
			
			return out;
		}
		lattice_data get_lattice() const {return lattice;}
		int get_spin(const size_t& i, const size_t& j) const {return lattice[i][j];}
		size_t get_coordination() const {return coordination;}

		virtual std::pair<size_t, size_t> draw(Renderer& renderer) const = 0;
		virtual void update(Renderer& renderer) const = 0;

		virtual ~Lattice() {}
	protected:
		size_t height;
		size_t width;
		lattice_data lattice;
		double J;

		size_t coordination;
};

inline std::ostream& operator<<(std::ostream& out, const Lattice& lattice) {
	lattice.show(out);
	
	return out;
}

inline size_t index(const int& i, const size_t& range) {
	if (i < 0)
		return i + range;
	else
		return i % range;
}

#endif //LATTICE_CLASS_H

#include "square_lattice.h"
#include <iostream>
#include <iomanip>

std::vector<int> Square_lattice::get_spins(const size_t& i, const size_t& j) const {
	return {
			lattice[(i+1) % width][j] , lattice[index(i-1, width)][j], 
			lattice[i][(j+1) % height], lattice[i][index(j-1, height)],
			lattice[i][j]
		};
}

list_index Square_lattice::get_spins_idx(const size_t& i, const size_t& j) const {
	return {
			std::make_pair(((i+1) % width), j), std::make_pair(index(i-1, width), j), 
			std::make_pair(i,((j+1) % height)), std::make_pair(i, index(j-1, height)),
			std::make_pair(i, j)
		};
}

std::vector<int> Square_lattice::get_comb() const {
	const std::vector<int> res = {-4, -2, 0, 2, 4};
	return res;
}

std::pair<size_t, size_t> Square_lattice::draw(Renderer& renderer) const {
	renderer.dim = width;
	renderer.size = float(renderer.height/(float)renderer.dim);
	
	renderer.positions = std::vector<r2::vec2<float>>(4*renderer.dim*renderer.dim);
	renderer.colours   = std::vector<r2::vec3<float>>(4*renderer.dim*renderer.dim);
	size_t i = 0;
	while (i < renderer.dim) {
		size_t j = 0;
		while (j < renderer.dim) {
			
			r2::Quad<float> new_quad = r2::Quad<float>(r2::Vertex<float>(renderer.origin + r2::vec2<float>(j*renderer.size, i*renderer.size)), renderer.size);
			renderer.positions[4*(j+i*renderer.dim)]   = new_quad.a.position;
			renderer.positions[4*(j+i*renderer.dim)+1] = new_quad.b.position;
			renderer.positions[4*(j+i*renderer.dim)+2] = new_quad.c.position;
			renderer.positions[4*(j+i*renderer.dim)+3] = new_quad.d.position;

			renderer.colours[4*(j+i*renderer.dim)]   = r2::vec3<float>(lattice[i][j], 0.0f, 0.0f);
			renderer.colours[4*(j+i*renderer.dim)+1] = r2::vec3<float>(lattice[i][j], 0.0f, 0.0f);
			renderer.colours[4*(j+i*renderer.dim)+2] = r2::vec3<float>(lattice[i][j], 0.0f, 0.0f);
			renderer.colours[4*(j+i*renderer.dim)+3] = r2::vec3<float>(lattice[i][j], 0.0f, 0.0f);
			++j;
		}
		++i;
	}

	renderer.indices = std::vector<unsigned int>(renderer.dim*renderer.dim*6);
	size_t k = 0;
	for (size_t i=0; i<renderer.dim*renderer.dim*6; i+=6) {
		renderer.indices[i]   = k;
		renderer.indices[i+1] = k+1;
		renderer.indices[i+2] = k+2;
		renderer.indices[i+3] = k+1;
		renderer.indices[i+4] = k+2;
		renderer.indices[i+5] = k+3;
		k += 4;
	}


	return {4 * renderer.dim * renderer.dim, 6 * renderer.dim * renderer.dim};
}

void Square_lattice::update(Renderer& renderer) const {
	size_t i = 0;
	while (i < renderer.dim) {
		size_t j = 0;
		while (j < renderer.dim) {
			renderer.colours[4*(j+i*renderer.dim)]   = r2::vec3<float>(lattice[i][j], 0.0f, 0.0f);
			renderer.colours[4*(j+i*renderer.dim)+1] = r2::vec3<float>(lattice[i][j], 0.0f, 0.0f);
			renderer.colours[4*(j+i*renderer.dim)+2] = r2::vec3<float>(lattice[i][j], 0.0f, 0.0f);
			renderer.colours[4*(j+i*renderer.dim)+3] = r2::vec3<float>(lattice[i][j], 0.0f, 0.0f);
			++j;
		}
		++i;
	}
}

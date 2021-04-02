#include "honeycomb_lattice.h"

std::vector<int> Honeycomb_lattice::get_spins(const size_t& i, const size_t& j) const {
	const int par_i = parity(i);
	const int par_j = parity(j);

	return {
			lattice[i][index(j-1, width)], lattice[i][(j+1) % width],

			lattice[index(i+par_i*par_j, height)][j],

			lattice[i][j]
		};
}

list_index  Honeycomb_lattice::get_spins_idx(const size_t& i, const size_t& j) const {
	const int par_i = parity(i);
	const int par_j = parity(j);

	return {
			std::make_pair(i, index(j-1, width)), std::make_pair(i, (j+1) % width),

			std::make_pair(index(i+par_i*par_j, height), j),

			std::make_pair(i, j)
		};
}

std::vector<int> Honeycomb_lattice::get_comb() const {
	return {-3, -1, 1, 3};
}

std::pair<size_t, size_t> Honeycomb_lattice::draw(Renderer& renderer) const {
	renderer.dim = width;
	renderer.size = 2*float(renderer.height/(float)renderer.dim);
	
	const float triangle_height = renderer.size/2.0f; //renderer.size * sqrtf(3.0f)/2;
	const float shift_bottom = 0.0f;//(renderer.size - triangle_height)/2.0f;//(2.0f - sqrtf(3.0))/4.0f;
	
	renderer.positions = std::vector<r2::vec2<float>>(3*renderer.dim*renderer.dim + 3*renderer.dim);
	renderer.colours   = std::vector<r2::vec3<float>>(3*renderer.dim*renderer.dim + 3*renderer.dim);
	
	size_t j = 0;
	while (j < renderer.dim) {
		size_t i = 0;
		while (i < renderer.dim) {
			const r2::vec2<float> position_triangle = renderer.origin + r2::vec2<float>(i*renderer.size/2.0f, triangle_height*j + shift_bottom);

			if (i == renderer.dim - 1) {
				r2::Vertex<float> vert_a(position_triangle);
				r2::Vertex<float> vert_b(position_triangle + r2::vec2<float>(renderer.size/2.0f, 0.0f));
				r2::Vertex<float> vert_c(position_triangle + r2::vec2<float>(renderer.size/2.0f, triangle_height));

				if (j % 2 == 1)
					vert_a = vert_a.shift_y(triangle_height);

				r2::Triangle<float> tri(vert_a, vert_b, vert_c);

				renderer.positions[3*(i + j*(renderer.dim+1))    ] = tri.a.position;
				renderer.positions[3*(i + j*(renderer.dim+1)) + 1] = tri.b.position;
				renderer.positions[3*(i + j*(renderer.dim+1)) + 2] = tri.c.position;

				renderer.colours[3*(i + j*(renderer.dim+1))    ] = r2::vec3<float>(lattice[i][j], 0.0f, 0.0f);
				renderer.colours[3*(i + j*(renderer.dim+1)) + 1] = r2::vec3<float>(lattice[i][j], 0.0f, 0.0f);
				renderer.colours[3*(i + j*(renderer.dim+1)) + 2] = r2::vec3<float>(lattice[i][j], 0.0f, 0.0f);

				vert_a = r2::Vertex<float>(renderer.origin + r2::vec2<float>(0.0f, triangle_height*j+shift_bottom));
				vert_b = r2::Vertex<float>(renderer.origin + r2::vec2<float>(renderer.size/2.0f, triangle_height*j+shift_bottom));
				vert_c = r2::Vertex<float>(renderer.origin + r2::vec2<float>(0.0f, triangle_height*(j+1) + shift_bottom));
				
				if (j % 2 == 1)
					vert_b = vert_b.shift_y(triangle_height);

				tri = r2::Triangle<float>(vert_a, vert_b, vert_c);

				renderer.positions[3*(i+1 + j*(renderer.dim+1))    ] = tri.a.position;
				renderer.positions[3*(i+1 + j*(renderer.dim+1)) + 1] = tri.b.position;
				renderer.positions[3*(i+1 + j*(renderer.dim+1)) + 2] = tri.c.position;
				
				renderer.colours[3*(i+1 + j*(renderer.dim+1))    ] = r2::vec3<float>(lattice[i][j], 0.0f, 0.0f);
				renderer.colours[3*(i+1 + j*(renderer.dim+1)) + 1] = r2::vec3<float>(lattice[i][j], 0.0f, 0.0f);
				renderer.colours[3*(i+1 + j*(renderer.dim+1)) + 2] = r2::vec3<float>(lattice[i][j], 0.0f, 0.0f);
			}

			else  {
				r2::Vertex<float> vert_a(position_triangle);
				r2::Vertex<float> vert_b(position_triangle + r2::vec2<float>(renderer.size, 0.0f));
				r2::Vertex<float> vert_c(position_triangle + r2::vec2<float>(renderer.size/2.0f, triangle_height));

				r2::Triangle<float> tri(vert_a, vert_b, vert_c);

				//r2::Etri<float> etri(r2::Vertex<float>(position_triangle, {0.0f, 0.0f, 0.0f}), renderer.size, lattice_value);
				if (parity(i)*parity(j) == 1) {
					tri.c.position.y -= 2*triangle_height;
					tri.translate(r2::vec2<float>(0.0f, triangle_height));
					//etri.flip_ud();
					//etri.translate(r2::vec2<float>(0.0f, triangle_height));
				}

				renderer.positions[3*(i + j*(renderer.dim+1))    ] = tri.a.position;
				renderer.positions[3*(i + j*(renderer.dim+1)) + 1] = tri.b.position;
				renderer.positions[3*(i + j*(renderer.dim+1)) + 2] = tri.c.position;

				renderer.colours[3*(i + j*(renderer.dim+1))    ] = r2::vec3<float>(lattice[i][j], 0.0f, 0.0f);
				renderer.colours[3*(i + j*(renderer.dim+1)) + 1] = r2::vec3<float>(lattice[i][j], 0.0f, 0.0f);
				renderer.colours[3*(i + j*(renderer.dim+1)) + 2] = r2::vec3<float>(lattice[i][j], 0.0f, 0.0f);
			}
			++i;
		}
	++j;
	}

	renderer.indices = std::vector<unsigned int>(renderer.dim*renderer.dim*6);

	for (size_t i=0; i<3*(renderer.dim + 1)*renderer.dim; i+=3) {
		renderer.indices[i]   = i;
		renderer.indices[i+1] = i+1;
		renderer.indices[i+2] = i+2;
	}

	return {3* renderer.dim * (renderer.dim + 1), 3 * renderer.dim * (renderer.dim + 1)};
}

void Honeycomb_lattice::update(Renderer& renderer) const {
	size_t j = 0;
	while (j < renderer.dim) {
		size_t i = 0;
		while (i < renderer.dim) {
			if (i == renderer.dim - 1) {
				renderer.colours[3*(i + j*(renderer.dim+1))    ] = r2::vec3<float>(lattice[i][j], 0.0f, 0.0f);
				renderer.colours[3*(i + j*(renderer.dim+1)) + 1] = r2::vec3<float>(lattice[i][j], 0.0f, 0.0f);
				renderer.colours[3*(i + j*(renderer.dim+1)) + 2] = r2::vec3<float>(lattice[i][j], 0.0f, 0.0f);

				renderer.colours[3*(i+1 + j*(renderer.dim+1))    ] = r2::vec3<float>(lattice[i][j], 0.0f, 0.0f);
				renderer.colours[3*(i+1 + j*(renderer.dim+1)) + 1] = r2::vec3<float>(lattice[i][j], 0.0f, 0.0f);
				renderer.colours[3*(i+1 + j*(renderer.dim+1)) + 2] = r2::vec3<float>(lattice[i][j], 0.0f, 0.0f);
			}

			else  {
				renderer.colours[3*(i + j*(renderer.dim+1))    ] = r2::vec3<float>(lattice[i][j], 0.0f, 0.0f);
				renderer.colours[3*(i + j*(renderer.dim+1)) + 1] = r2::vec3<float>(lattice[i][j], 0.0f, 0.0f);
				renderer.colours[3*(i + j*(renderer.dim+1)) + 2] = r2::vec3<float>(lattice[i][j], 0.0f, 0.0f);
			}
			++i;
		}
	++j;
	}
}

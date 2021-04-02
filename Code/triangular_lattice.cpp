#include "triangular_lattice.h"

std::vector<int> Triangular_lattice::get_spins(const size_t& i, const size_t& j) const {
	return {
			lattice[i][index(j-1, width)], 
			lattice[i][(j+1) % width], 
			
			lattice[(i+1) % height][index(j-1, width)], 
			lattice[(i+1) % height][j],
			
			lattice[index(i-1, height)][j],
			lattice[index(i-1, height)][(j+1) % width],
			
			lattice[i][j]	
		};
}

list_index  Triangular_lattice::get_spins_idx(const size_t& i, const size_t& j) const {
	return {
			std::make_pair(i, index(j-1, width)), 
			std::make_pair(i, (j+1) % width), 
			
			std::make_pair((i+1) % height, index(j-1, width)), 
			std::make_pair((i+1) % height, j),
			
			std::make_pair(index(i-1, height), j),
			std::make_pair(index(i-1, height), (j+1) % width),
			
			std::make_pair(i, j)	
		};
}

std::vector<int> Triangular_lattice::get_comb() const {
	return {-6, -4, -2, 0, 2, 4, 6};
}

std::pair<size_t, size_t> Triangular_lattice::draw(Renderer& renderer) const {
	renderer.dim = width;
	renderer.size = float(renderer.height/(float)renderer.dim);
	const float hex_height = float(renderer.height - 0.5f*renderer.size*(renderer.dim-1))/float(renderer.dim+1);
	const float shift_bottom = hex_height;//0.5f*(hex_height - renderer.size*0.5f);
	
	renderer.positions = std::vector<r2::vec2<float>>(6*renderer.dim*(renderer.dim+1) + 3*(renderer.dim+1));
	renderer.colours   = std::vector<r2::vec3<float>>(6*renderer.dim*(renderer.dim+1) + 3*(renderer.dim+1));
	
	size_t j = 0;
	size_t oddity = 0;
	r2::vec2<float> flatten(0.0f, 0.0f);
	while (j < renderer.dim) {
		size_t i = 0;
		float column_shift = 0.0f;
		if (j % 2 == 1) 
			column_shift = renderer.size*0.5f;

		if (j == renderer.dim -1)
			flatten.y = -hex_height;

		while (i < renderer.dim) {
			const r2::vec2<float> position_hex = renderer.origin + r2::vec2<float>(i*renderer.size + column_shift, j*(renderer.size*0.5f + hex_height) + shift_bottom);
			r2::Hex hex = r2::Hex<float>(r2::Vertex<float>(position_hex), renderer.size, hex_height);
			
			if (i != renderer.dim-1 || j % 2 == 0) {
				renderer.positions[6*(i + j*renderer.dim) + 2*oddity    ] = hex.a.position;
				renderer.positions[6*(i + j*renderer.dim) + 2*oddity + 1] = hex.b.position;
				renderer.positions[6*(i + j*renderer.dim) + 2*oddity + 2] = hex.c.position + flatten;
				renderer.positions[6*(i + j*renderer.dim) + 2*oddity + 3] = hex.d.position;
				renderer.positions[6*(i + j*renderer.dim) + 2*oddity + 4] = hex.e.position;
				renderer.positions[6*(i + j*renderer.dim) + 2*oddity + 5] = hex.f.position;

				renderer.colours[6*(i + j*renderer.dim) + 2*oddity    ] = r2::vec3<float>(lattice[i][j], 0.0f, 0.0f);
				renderer.colours[6*(i + j*renderer.dim) + 2*oddity + 1] = r2::vec3<float>(lattice[i][j], 0.0f, 0.0f);
				renderer.colours[6*(i + j*renderer.dim) + 2*oddity + 2] = r2::vec3<float>(lattice[i][j], 0.0f, 0.0f);
				renderer.colours[6*(i + j*renderer.dim) + 2*oddity + 3] = r2::vec3<float>(lattice[i][j], 0.0f, 0.0f);
				renderer.colours[6*(i + j*renderer.dim) + 2*oddity + 4] = r2::vec3<float>(lattice[i][j], 0.0f, 0.0f);
				renderer.colours[6*(i + j*renderer.dim) + 2*oddity + 5] = r2::vec3<float>(lattice[i][j], 0.0f, 0.0f);
			}
			else {
				r2::Hex_half<float> hex_half_r = r2::Hex_half<float>(r2::Vertex<float>(position_hex), renderer.size, hex_height);

				renderer.positions[6*(i + j*renderer.dim) + 2*oddity    ] = hex_half_r.a.position;
				renderer.positions[6*(i + j*renderer.dim) + 2*oddity + 1] = hex_half_r.b.position;
				renderer.positions[6*(i + j*renderer.dim) + 2*oddity + 2] = hex_half_r.c.position + flatten;
				renderer.positions[6*(i + j*renderer.dim) + 2*oddity + 3] = hex_half_r.d.position;

				renderer.colours[6*(i + j*renderer.dim) + 2*oddity    ] = r2::vec3<float>(lattice[i][j], 0.0f, 0.0f);
				renderer.colours[6*(i + j*renderer.dim) + 2*oddity + 1] = r2::vec3<float>(lattice[i][j], 0.0f, 0.0f);
				renderer.colours[6*(i + j*renderer.dim) + 2*oddity + 2] = r2::vec3<float>(lattice[i][j], 0.0f, 0.0f);
				renderer.colours[6*(i + j*renderer.dim) + 2*oddity + 3] = r2::vec3<float>(lattice[i][j], 0.0f, 0.0f);

				const r2::vec2<float> position_hex_half_l = renderer.origin + r2::vec2<float>(renderer.size*0.5f, j*(renderer.size*0.5f + hex_height) + shift_bottom);
				r2::Hex_half<float> hex_half_l = r2::Hex_half<float>(r2::Vertex<float>(position_hex_half_l), renderer.size, hex_height);
				hex_half_l = hex_half_l.flip_lr();

				renderer.positions[6*(i + j*renderer.dim) + 2*oddity + 4] = hex_half_l.a.position;
				renderer.positions[6*(i + j*renderer.dim) + 2*oddity + 5] = hex_half_l.b.position;
				renderer.positions[6*(i + j*renderer.dim) + 2*oddity + 6] = hex_half_l.c.position + flatten;
				renderer.positions[6*(i + j*renderer.dim) + 2*oddity + 7] = hex_half_l.d.position;

				renderer.colours[6*(i + j*renderer.dim) + 2*oddity + 4] = r2::vec3<float>(lattice[i][j], 0.0f, 0.0f);
				renderer.colours[6*(i + j*renderer.dim) + 2*oddity + 5] = r2::vec3<float>(lattice[i][j], 0.0f, 0.0f);
				renderer.colours[6*(i + j*renderer.dim) + 2*oddity + 6] = r2::vec3<float>(lattice[i][j], 0.0f, 0.0f);
				renderer.colours[6*(i + j*renderer.dim) + 2*oddity + 7] = r2::vec3<float>(lattice[i][j], 0.0f, 0.0f);

				++oddity;
			}
			++i;
		}
		++j;
	}

	r2::Vertex<float> vert_a(renderer.origin);
	r2::Vertex<float> vert_b(renderer.origin + r2::vec2<float>(renderer.size/2.0f, 0.0f));
	r2::Vertex<float> vert_c(renderer.origin + r2::vec2<float>(0.0f, hex_height));

	renderer.positions[6*(0 + renderer.dim*renderer.dim) + 2*oddity    ] = vert_a.position;
	renderer.positions[6*(0 + renderer.dim*renderer.dim) + 2*oddity + 1] = vert_b.position;
	renderer.positions[6*(0 + renderer.dim*renderer.dim) + 2*oddity + 2] = vert_c.position;

	renderer.colours[6*(0 + renderer.dim*renderer.dim) + 2*oddity    ] = r2::vec3<float>(lattice[renderer.dim-1][renderer.dim-1], 0.0f, 0.0f);
	renderer.colours[6*(0 + renderer.dim*renderer.dim) + 2*oddity + 1] = r2::vec3<float>(lattice[renderer.dim-1][renderer.dim-1], 0.0f, 0.0f);
	renderer.colours[6*(0 + renderer.dim*renderer.dim) + 2*oddity + 2] = r2::vec3<float>(lattice[renderer.dim-1][renderer.dim-1], 0.0f, 0.0f);

	float shift_triangle = 0.0f;
	int   shift_lattice = 0;
	for (size_t i=1; i<renderer.dim+1; ++i) {
		if (i == renderer.dim) {
			shift_triangle = -0.5f * renderer.size;
			shift_lattice = -1;
		}
		const r2::vec2<float> triangle_position = renderer.origin + r2::vec2<float>(renderer.size*0.5f + renderer.size*(i-1), 0.0f);
		r2::Vertex<float> vert_a(triangle_position);
		r2::Vertex<float> vert_b(triangle_position + r2::vec2<float>(renderer.size + shift_triangle, 0.0f));
		r2::Vertex<float> vert_c(triangle_position + r2::vec2<float>(renderer.size*0.5f, hex_height));

		renderer.positions[6*(renderer.dim*renderer.dim) + 2*oddity + 3*i     ] = vert_a.position;
		renderer.positions[6*(renderer.dim*renderer.dim) + 2*oddity + 3*i  + 1] = vert_b.position;
		renderer.positions[6*(renderer.dim*renderer.dim) + 2*oddity + 3*i  + 2] = vert_c.position;

		renderer.colours[6*(renderer.dim*renderer.dim) + 2*oddity + 3*i    ] = r2::vec3<float>(lattice[renderer.dim-1][i + shift_lattice], 0.0f, 0.0f);
		renderer.colours[6*(renderer.dim*renderer.dim) + 2*oddity + 3*i + 1] = r2::vec3<float>(lattice[renderer.dim-1][i + shift_lattice], 0.0f, 0.0f);
		renderer.colours[6*(renderer.dim*renderer.dim) + 2*oddity + 3*i + 2] = r2::vec3<float>(lattice[renderer.dim-1][i + shift_lattice], 0.0f, 0.0f);
	}

	renderer.indices = std::vector<unsigned int>(renderer.dim*renderer.dim*12 + renderer.dim + 3*(renderer.dim+1));

	size_t k = 0;
	for (size_t j=0; j < renderer.dim; ++j) {

		size_t shift = 0;
		if (j % 2 == 1) {
			shift = 1;	
		}

		for (size_t i=0; i<12*(renderer.dim-shift); i+=12) {
			renderer.indices[i + 12*j*renderer.dim]    = k;
			renderer.indices[i + 12*j*renderer.dim+1]  = k+1;
			renderer.indices[i + 12*j*renderer.dim+2]  = k+5;

			renderer.indices[i + 12*j*renderer.dim+3]  = k+1;
			renderer.indices[i + 12*j*renderer.dim+4]  = k+2;
			renderer.indices[i + 12*j*renderer.dim+5]  = k+3;

			renderer.indices[i + 12*j*renderer.dim+6]  = k+3;
			renderer.indices[i + 12*j*renderer.dim+7]  = k+4;
			renderer.indices[i + 12*j*renderer.dim+8]  = k+5;

			renderer.indices[i + 12*j*renderer.dim+9]  = k+1;
			renderer.indices[i + 12*j*renderer.dim+10] = k+3;
			renderer.indices[i + 12*j*renderer.dim+11] = k+5;

			k += 6;
		}

		if (j % 2 == 1) {
			int i = 12 * (renderer.dim-1);
			renderer.indices[i + 12*j*renderer.dim    ] = k;
			renderer.indices[i + 12*j*renderer.dim + 1] = k + 1;
			renderer.indices[i + 12*j*renderer.dim + 2] = k + 2;
			renderer.indices[i + 12*j*renderer.dim + 3] = k;
			renderer.indices[i + 12*j*renderer.dim + 4] = k + 2;
			renderer.indices[i + 12*j*renderer.dim + 5] = k + 3;
			k += 4;

			renderer.indices[i + 12*j*renderer.dim + 6 ] = k;
			renderer.indices[i + 12*j*renderer.dim + 7 ] = k + 1;
			renderer.indices[i + 12*j*renderer.dim + 8 ] = k + 2;
			renderer.indices[i + 12*j*renderer.dim + 9 ] = k;
			renderer.indices[i + 12*j*renderer.dim + 10] = k + 2;
			renderer.indices[i + 12*j*renderer.dim + 11] = k + 3;
			k += 4;
		}
	}
	
	for (size_t i=0; i<3*(renderer.dim+1); i+=3) {
		renderer.indices[i + 12*renderer.dim*renderer.dim    ] = k;
		renderer.indices[i + 12*renderer.dim*renderer.dim + 1] = k + 1;
		renderer.indices[i + 12*renderer.dim*renderer.dim + 2] = k + 2;
		k += 3;
	}


	const size_t grid_size = renderer.dim * renderer.dim;
	return {6 * grid_size + renderer.dim + 3*(renderer.dim+1), 12 * grid_size + 3*(renderer.dim+1)};
}

void Triangular_lattice::update(Renderer& renderer) const {
	size_t j = 0;
	size_t oddity = 0;
	while (j < renderer.dim) {
		size_t i = 0;
		while (i < renderer.dim) {
			if (i != renderer.dim-1 || j % 2 == 0) {
				renderer.colours[6*(i + j*renderer.dim) + 2*oddity    ] = r2::vec3<float>(lattice[i][j], 0.0f, 0.0f);
				renderer.colours[6*(i + j*renderer.dim) + 2*oddity + 1] = r2::vec3<float>(lattice[i][j], 0.0f, 0.0f);
				renderer.colours[6*(i + j*renderer.dim) + 2*oddity + 2] = r2::vec3<float>(lattice[i][j], 0.0f, 0.0f);
				renderer.colours[6*(i + j*renderer.dim) + 2*oddity + 3] = r2::vec3<float>(lattice[i][j], 0.0f, 0.0f);
				renderer.colours[6*(i + j*renderer.dim) + 2*oddity + 4] = r2::vec3<float>(lattice[i][j], 0.0f, 0.0f);
				renderer.colours[6*(i + j*renderer.dim) + 2*oddity + 5] = r2::vec3<float>(lattice[i][j], 0.0f, 0.0f);
			}
			else {
				renderer.colours[6*(i + j*renderer.dim) + 2*oddity    ] = r2::vec3<float>(lattice[i][j], 0.0f, 0.0f);
				renderer.colours[6*(i + j*renderer.dim) + 2*oddity + 1] = r2::vec3<float>(lattice[i][j], 0.0f, 0.0f);
				renderer.colours[6*(i + j*renderer.dim) + 2*oddity + 2] = r2::vec3<float>(lattice[i][j], 0.0f, 0.0f);
				renderer.colours[6*(i + j*renderer.dim) + 2*oddity + 3] = r2::vec3<float>(lattice[i][j], 0.0f, 0.0f);

				renderer.colours[6*(i + j*renderer.dim) + 2*oddity + 4] = r2::vec3<float>(lattice[i][j], 0.0f, 0.0f);
				renderer.colours[6*(i + j*renderer.dim) + 2*oddity + 5] = r2::vec3<float>(lattice[i][j], 0.0f, 0.0f);
				renderer.colours[6*(i + j*renderer.dim) + 2*oddity + 6] = r2::vec3<float>(lattice[i][j], 0.0f, 0.0f);
				renderer.colours[6*(i + j*renderer.dim) + 2*oddity + 7] = r2::vec3<float>(lattice[i][j], 0.0f, 0.0f);

				++oddity;
			}
			++i;
		}
		++j;
	}
	renderer.colours[6*(0 + renderer.dim*renderer.dim) + 2*oddity    ] = r2::vec3<float>(lattice[renderer.dim-1][renderer.dim-1], 0.0f, 0.0f);
	renderer.colours[6*(0 + renderer.dim*renderer.dim) + 2*oddity + 1] = r2::vec3<float>(lattice[renderer.dim-1][renderer.dim-1], 0.0f, 0.0f);
	renderer.colours[6*(0 + renderer.dim*renderer.dim) + 2*oddity + 2] = r2::vec3<float>(lattice[renderer.dim-1][renderer.dim-1], 0.0f, 0.0f);

	int   shift_lattice = 0;
	for (size_t i=1; i<renderer.dim+1; ++i) {
		if (i == renderer.dim) {
			shift_lattice = -1;
		}

		renderer.colours[6*(renderer.dim*renderer.dim) + 2*oddity + 3*i    ] = r2::vec3<float>(lattice[i + shift_lattice][renderer.dim-1], 0.0f, 0.0f);
		renderer.colours[6*(renderer.dim*renderer.dim) + 2*oddity + 3*i + 1] = r2::vec3<float>(lattice[i + shift_lattice][renderer.dim-1], 0.0f, 0.0f);
		renderer.colours[6*(renderer.dim*renderer.dim) + 2*oddity + 3*i + 2] = r2::vec3<float>(lattice[i + shift_lattice][renderer.dim-1], 0.0f, 0.0f);
	}
}

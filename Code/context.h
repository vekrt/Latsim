#ifndef CONTEXT_CLASS_H
#define CONTEXT_CLASS_H

#include "renderer.h"
#include "lattice.h"
#include "integrator.h"
#include <typeinfo>



const size_t height = 800;
const size_t width  = 800;

float off[] = {
	0.1f, 0.1f, 0.1f,   0.1f, 0.1f, 0.1f,
	0.1f, 0.1f, 0.1f,   0.1f, 0.1f, 0.1f
};

float on[] = {
	0.0f, 0.5f, 0.5f,   0.0f, 0.5f, 0.5f,
	0.0f, 0.5f, 0.5f,   0.0f, 0.5f, 0.5f
};

float delta_time = 0.0f;	// time between current frame and last frame
float last_frame = 0.0f;


const std::map<size_t, r2::vec3<float>> palette = {
						{0, r2::vec3<float>(0.1f,  0.1f,  0.1f)},
						{1, r2::vec3<float>(0.0f,  0.4f,  0.4f)},
						{2, r2::vec3<float>(1.0f,  0.79f, 0.0f)},
						{3, r2::vec3<float>(0.84f, 0.36f, 0.0f)},
						{4, r2::vec3<float>(0.06f, 0.63f,  0.63f)},
					};

class Context {
	public:
		void init(Lattice* lattice, Model* model) {
			model = model;
			lattice = lattice;
			renderer.init(height, width);
			renderer.load_shader("rect.vert", "rect.frag");
			renderer.camera.init();
			std::pair<size_t, size_t> vertices_indices = lattice -> draw(renderer);
			for (auto& el : renderer.colours) {
				el = palette.at(model -> get_colour(el.x));
			}

			nbr_vertices = vertices_indices.first;
			nbr_indices  = vertices_indices.second;

			renderer.send(nbr_vertices, nbr_indices);

			//Texture Tex1, Tex2;
			//Tex1.init(off, 0);
			//Tex2.init(on, 1);

			renderer.shader.use();
			//glUniform1i(glGetUniformLocation(renderer.shader.ID, "texture1"), 0);
			//glUniform1i(glGetUniformLocation(renderer.shader.ID, "texture2"), 1);
		}

		void launch(Integrator* integrator, Model* model, Lattice* lattice) {
			size_t k = 0;
			while (renderer.close()) {
				float current_frame = glfwGetTime();
				delta_time = current_frame - last_frame;
				last_frame = current_frame;
				renderer.process_input(delta_time);
				
				if (k % 1000 == 0)
					std::cout << k << std::endl;

				float zoom = renderer.camera.zoom;
				const r2::vec2<float> pos = renderer.camera.ortho_position;
				renderer.shader.setMat4("view", renderer.camera.get_ortho(
								(0.0f-pos.x), (width-pos.x), 
								(0.0f-pos.y), (height-pos.y),
								 -1.0f, 1.0f)
								 );

				r2::vec2 center((width-renderer.size)/2, (height-renderer.size)/2);

				renderer.shader.setMat4("projection", r2::zoom<float>(center.x, center.y, zoom));
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glClearColor(0.5f, 0.1f, 0.1f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT);
				
				for (size_t i=0; i<renderer.number_of_updates; ++i) {
					//const std::pair<int, std::vector<int>> idx = 
					if (!renderer.pause)
						integrator -> step(*model, *lattice);
				}
				lattice -> update(renderer);
				for (auto& el : renderer.colours) {
					el = palette.at(model -> get_colour(el.x));
				}

				renderer.update(nbr_vertices);
				
				renderer.draw(nbr_vertices);

				renderer.swap_buffer();

				glfwPollEvents();
				
				++k;
			}
		}

		void terminate() {
			glfwTerminate();
			renderer.cleanup();
		}
	protected:
		Renderer renderer;
		size_t nbr_vertices;
		size_t nbr_indices;
		
};

#endif //CONTEXT_CLASS_H

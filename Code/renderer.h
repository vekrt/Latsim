#ifndef RENDERER_CLASS_H
#define RENDERER_CLASS_H

#include <vector>
#include "glad.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include "shader.h"
#include "camera.h"
#include <map>

#include "r2.h"

const size_t SCREEN_HEIGHT = 800;
const size_t SCREEN_WIDTH  = 800;

class Renderer {
	public:
		Renderer() = default;
		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;
		void cleanup() {
			glDeleteBuffers(1, &EBO);
			glDeleteBuffers(2, VBO);
			glDeleteVertexArrays(1, &VAO);			
		}

		int init(const float&, const float&);
		void send(const size_t& nbr_vertices, const size_t& nbr_indices);
		void draw(const size_t& nbr_triangles) const;
		void update(const size_t& nbr_vertices);
		void change_quad(const size_t& idx);
		Shader load_shader(const char*, const char*);
		bool close() const;
		void process_input(const float&);
		void swap_buffer();
		void scroll_callback(GLFWwindow*, double, double);
		void save();

		Shader shader;
		Camera camera;
		GLFWwindow* window;
		unsigned int VAO, VBO[2], EBO;
		bool pause;
		std::vector<float> pixels;
		size_t save_counter;
		
		std::vector<r2::vec2<float>> positions;
		std::vector<r2::vec3<float>> colours;
		std::vector<unsigned int> indices;

		float height;
		float width;
		float size;
		unsigned int dim;

		size_t number_of_updates;
		r2::vec2<float> origin;
};

void framebuffer_size_callback(GLFWwindow*, int, int);

#endif //RENDERER_CLASS_H

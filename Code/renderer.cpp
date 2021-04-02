#include "renderer.h"
#include <cstring>

int Renderer::init(const float& height_, const float& width_) {
	glfwInit();
	glfwWindowHint(GLFW_VERSION_MAJOR, 4);	
	glfwWindowHint(GLFW_VERSION_MINOR, 6);	

	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Simulation", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
    	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//glfwSetScrollCallback(window, static_cast<Renderer*>(window)->scroll_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialise GLAD" << std::endl;
		return -1;
	}

	height = height_;
	width = width_;

	number_of_updates = 1;

	origin = {1.0f, 1.0f};

	pause = false;

	pixels = std::vector<float>(height*width*3);
	save_counter = 0;

	return 0;
}

Shader Renderer::load_shader(const char* vertex, const char* fragment) {
	shader.init(vertex, fragment);

	return shader;
}

void framebuffer_size_callback([[maybe_unused]] GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

bool Renderer::close() const {
	return !glfwWindowShouldClose(window);
}

void Renderer::process_input(const float& delta_time) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.camera_input(FORWARD, delta_time);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.camera_input(BACKWARD, delta_time);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.camera_input(LEFT, delta_time);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.camera_input(RIGHT, delta_time);
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
		camera.reset();
	}
	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
		camera.camera_input(IN, delta_time);
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
		camera.camera_input(OUT, delta_time);
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		number_of_updates += 10;
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
		if (number_of_updates > 10)
			number_of_updates -= 10;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		pause = !pause;
	}
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		save();
		++save_counter;
	}
}

void Renderer::swap_buffer() {
	glfwSwapBuffers(this -> window);
}

void Renderer::send(const size_t& nbr_vertices, const size_t& nbr_indices) {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(2, VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, nbr_indices*sizeof(unsigned int), static_cast<void*>(indices.data()), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, nbr_vertices*sizeof(r2::vec2<float>), static_cast<void*>(positions.data()), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(r2::vec2<float>), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, nbr_vertices*sizeof(r2::vec3<float>), static_cast<void*>(colours.data()), GL_STREAM_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(r2::vec3<float>), (void*)0);
	glEnableVertexAttribArray(1);


	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(r2::Vertex<float>), (void*)offsetof(r2::Vertex<float>, colour));
	//glEnableVertexAttribArray(1);

	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(r2::Vertex<float>), (void*)offsetof(r2::Vertex<float>, texture));
	//glEnableVertexAttribArray(2);
	//
	//glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(r2::Vertex<float>), (void*)offsetof(r2::Vertex<float>, tex_idx));
	//glEnableVertexAttribArray(3);
}

void Renderer::draw(const size_t& nbr_triangles) const {
	//glBindVertexArray(VAO);
	//glBindBuffer(GL_ARRAY_BUFFER, EBO);
	glDrawElements(GL_TRIANGLES, 2*nbr_triangles, GL_UNSIGNED_INT, 0);
}

void Renderer::update(const size_t& nbr_vertices) {
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	void *ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	
	memcpy(ptr, static_cast<void*>(colours.data()), nbr_vertices*sizeof(r2::vec3<float>));
	glUnmapBuffer(GL_ARRAY_BUFFER);
}

void Renderer::save() {
	glReadBuffer(GL_FRONT);
	glReadPixels(0, 0, width, height, GL_RGB, GL_FLOAT, pixels.data());

	std::string filename = std::string("Image/") + std::to_string(save_counter) + std::string(".dat");
	std::ofstream file(filename, std::ios::out | std::ios::binary);

	if (file.fail()) {
		std::cerr << "Cannot open the file " << filename << std::endl;
	}

	for (auto const& el : pixels) {
		file.write(reinterpret_cast<const char*>(&el), sizeof(el));
	}

	file.close();
}

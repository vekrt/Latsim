#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include "glad.h"
#include "r2.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
	public:
		unsigned int ID;

		Shader() = default;
		void init(const char* vert_path, const char* frag_path) {
			std::string vert_code;
			std::string frag_code;
			std::ifstream vert_shader_file;
			std::ifstream frag_shader_file;

			vert_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			frag_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

			try {
				vert_shader_file.open(vert_path);
				frag_shader_file.open(frag_path);

				std::stringstream vert_shader_stream, frag_shader_stream;
				vert_shader_stream << vert_shader_file.rdbuf();
				frag_shader_stream << frag_shader_file.rdbuf();

				vert_shader_file.close();
				frag_shader_file.close();

				vert_code = vert_shader_stream.str();
				frag_code = frag_shader_stream.str();
			}
			catch (const std::ifstream::failure& e) {
				std::cout << "ERROR::SHADER::CANNOT_READ_FILE" << std::endl;
			}

			const char* vert_shader_code = vert_code.c_str();
			const char* frag_shader_code = frag_code.c_str();

			unsigned int vert, frag;

			vert = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vert, 1, &vert_shader_code, NULL);
			glCompileShader(vert);
			check_compile(vert, "VERTEX");

			
			frag = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(frag, 1, &frag_shader_code, NULL);
			glCompileShader(frag);
			check_compile(frag, "FRAGMENT");



			ID = glCreateProgram();
			glAttachShader(ID, vert);
			glAttachShader(ID, frag);
			glLinkProgram(ID);
			check_compile(ID, "PROGRAM");


			glDeleteShader(vert);
			glDeleteShader(frag);

		}
		void use() {
			glUseProgram(ID);
		}


		void check_compile(unsigned int target, std::string type) const {
			const size_t size = 1024;
			int success;
			char info_log[size];
			if (type == "PROGRAM") {
				glGetProgramiv(target, GL_LINK_STATUS, &success);
				if (!success) {
					glGetProgramInfoLog(target, size, nullptr, info_log);
					std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << std::endl;
			}
			}
			else {
				if (type == "VERTEX") {
					glGetShaderiv(target, GL_COMPILE_STATUS, &success);
					if (!success) {
						glGetShaderInfoLog(target, size, NULL, info_log);
						std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << std::endl;
					}
				}
				else {
					glGetShaderiv(target, GL_COMPILE_STATUS, &success);
					if (!success) {
						glGetShaderInfoLog(target, size, nullptr, info_log);
						std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << info_log << std::endl;
					}
				}
			}
		}
		void setInt(const std::string &name, int value) const {
			glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
		}

	    	void setMat4(const std::string &name, const r2::mat4<float>& mat) const
	    	{
			glUniformMatrix4fv(
				glGetUniformLocation(ID, name.c_str()), 
				1, 
				GL_FALSE, &mat[0][0]);
	    	}

	private:
};

#endif //SHADER_CLASS_H

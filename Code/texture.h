#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include "glad.h"

class Texture {
	public:
		unsigned int ID;
		void init(float* texture, const size_t& idx) {
			glGenTextures(1, &ID);
			glActiveTexture(GL_TEXTURE0 + idx);
			glBindTexture(GL_TEXTURE_2D, ID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_FLOAT, texture);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
	private:
};

#endif //TEXTURE_CLASS_H

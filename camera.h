#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include <ostream>
#include "r2.h"

enum Camera_movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	IN,
	OUT
};

class Camera {
	public:
		Camera() = default;
		void init();
		void camera_input(const Camera_movement&, const float&);
		void reset();
		r2::mat4<float> get_look_at();
		r2::mat4<float> get_ortho(const float&, const float&, const float&,
				   	  const float&, const float&, const float&);
		r2::mat4<float> get_zoom();

		void ProcessMouseScroll(float);

		r2::vec3<float> position;
		r2::vec3<float> front;
		r2::vec3<float> up;
		r2::vec3<float> direction;
		r2::vec3<float> right;

		r2::vec2<float> ortho_position;

		float zoom;

		r2::mat4<float> look_at;
		r2::mat4<float> ortho;
		r2::mat4<float> scale;
};

#endif //CAMERA_CLASS_H

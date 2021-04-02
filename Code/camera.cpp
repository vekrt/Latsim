#include "camera.h"
#include <iostream>

void Camera::init() {
	position  = r2::vec3<float>(0.0f, 0.0f,  3.0f);
	front     = r2::vec3<float>(0.0f, 0.0f, -1.0f);
	up        = r2::vec3<float>(0.0f, 1.0f,  0.0f);
	direction = r2::normalise(position);
	right     = r2::normalise(cross(up, direction));
	up        = r2::normalise(cross(direction, right));

	look_at = get_look_at();

	zoom = 1.0f;
	ortho_position = r2::vec2<float>(0.0f, 0.0f);
}

r2::mat4<float> Camera::get_look_at() {
	const double r_p = -r2::dot(right, position);
	const double u_p = -r2::dot(up, position);
	const double d_p = -r2::dot(direction, position);

	look_at.c1 = {right.x, up.x, direction.x, 0.0f};
	look_at.c2 = {right.y, up.y, direction.y, 0.0f};
	look_at.c3 = {right.z, up.z, direction.z, 0.0f};
	look_at.c4 = {(float)r_p, (float)u_p, (float)d_p, 1.0f};

	//look_at.c1 = {right.x, right.y, right.z, (float)r_p};
	//look_at.c2 = {up.x, up.y, up.z, (float)u_p};
	//look_at.c3 = {direction.x, direction.y, direction.z, (float)d_p};
	//look_at.c4 = {0.0f, 0.0f, 0.0f, 1.0f};

	return (*this).look_at;
}

r2::mat4<float> Camera::get_ortho(const float& left, const float& right, const float& bottom,
			          const float& top, const float& near, const float& far) 
{
	const float top_bottom = top - bottom;
	const float right_left = right - left;
	const float far_near = far - near;

	ortho.c1 = {2.0f/right_left, 0.0f, 0.0f, 0.0f};
	ortho.c2 = {0.0f, 2.0f/top_bottom, 0.0f, 0.0f};
	ortho.c3 = {0.0f, 0, 2.0f/far_near, 0.0f};
	ortho.c4 = {-(right+left)/right_left, -(top+bottom)/top_bottom, -(far+near)/far_near, 1.0f};

	//ortho.c1 = {2.0f/right_left, 0.0f, 0.0f, -(right+left)/right_left};
	//ortho.c2 = {0.0f, 2.0f/top_bottom, 0.0f, -(top+bottom)/top_bottom};
	//ortho.c3 = {0.0f, 0.0f, 2.0f/far_near, -(far+near)/far_near};
	//ortho.c4 = {0.0f, 0.0f, 0.0f, 1.0f};

	return ortho;
}

void Camera::reset() {
	position  = r2::vec3<float>(0.0f, 0.0f,  3.0f);
	front     = r2::vec3<float>(0.0f, 0.0f, -1.0f);
	up        = r2::vec3<float>(0.0f, 1.0f,  0.0f);
	direction = r2::normalise(position);
	right     = r2::normalise(cross(up, direction));
	up        = r2::normalise(cross(direction, right));

	look_at = get_look_at();
	zoom = 1.0f;
	ortho_position = r2::vec2<float>(0.0f, 0.0f);
}

void Camera::camera_input(const Camera_movement& move, const float& delta_time) {
	float reactivity = 500.0f * delta_time;

	if (move == FORWARD) {
		position -= up * reactivity;
		ortho_position.y -= reactivity;
	}
	if (move == BACKWARD) {
		position += up * reactivity;
		ortho_position.y +=reactivity;
	}
	if (move == LEFT) {
		position += right * reactivity;
		ortho_position.x +=reactivity;
	}
	if (move == RIGHT) {
		position -= right * reactivity;
		ortho_position.x -= reactivity;
	}
	if (move == IN) {
		if (zoom > 0.11f)
			zoom -= 0.1f;
	}
	if (move == OUT) {
		zoom += 0.1f;
	}
}


void Camera::ProcessMouseScroll(float yoffset) {
	zoom -= (float)yoffset;
	if (zoom < 1.0f)
		zoom = 1.0f;
	if (zoom > 45.0f)
		zoom = 45.0f; 
	}

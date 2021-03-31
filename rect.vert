#version 460 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aCol;
//layout (location = 2) in vec2 aTexCoord;
//layout (location = 3) in float atex_idx;

out vec3 colour;
//out vec2 TexCoord;
//out float tex_idx;
#define RATIO (600.0 / 800.0)

uniform mat4 view;
uniform mat4 projection;

void main() {
	gl_Position = view * projection * vec4(aPos, 0.0f, 1.0f);
	colour = aCol;
	//TexCoord = aTexCoord;
	//tex_idx = atex_idx;
}

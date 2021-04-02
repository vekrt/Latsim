#version 460 core

out vec4 FragColor;
in vec3 colour;
//in vec2 TexCoord;
//in float tex_idx;
//
//uniform sampler2D texture1;
//uniform sampler2D texture2;

void main()
{
	//FragColor = texture(ourTexture, TexCoord) * vec4(colour, 1.0f);
	//FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), tex_idx);
	FragColor = vec4(colour, 1.0f);
};


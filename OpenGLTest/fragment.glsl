#version 460 core

in vec3 vertexColour;
in vec2 TexCoord;

out vec4 fragColour;

layout (binding = 0) uniform sampler2D texture1;
layout (binding = 1) uniform sampler2D texture2;
uniform float ratio;

void main() {
	fragColour = mix(texture(texture1, TexCoord), texture(texture2, vec2(TexCoord.x, TexCoord.y)), ratio);// * vec4(vertexColour, 1.0f);
	//fragColour = vec4(vertexColour, 1.0f);
};
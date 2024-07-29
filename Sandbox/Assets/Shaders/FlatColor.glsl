#type Vertex

#version 410 core
layout(location = 0) in vec4 position;

uniform mat4 u_VP;
uniform mat4 u_Transform;

void main() {
	gl_Position = u_VP * u_Transform * position;
};

#type Fragment

#version 410 core

layout(location = 0) out vec4 color;
uniform vec3 u_Color;

void main() {
	color = vec4(u_Color, 1.0f);
};
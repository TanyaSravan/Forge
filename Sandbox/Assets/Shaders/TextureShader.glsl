#type Vertex
#version 410 core
layout(location = 0) in vec4 position;
layout(location = 1) in vec2 TexCoord;

uniform mat4 u_VP;
uniform mat4 u_Transform;

uniform float u_NumTiles;

out vec2 v_TexCoord;

void main() {
	v_TexCoord = TexCoord * u_NumTiles;
	gl_Position = u_VP * u_Transform * position;
};

#type Fragment
#version 410 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;
uniform vec4 u_Color;

void main() {
	color = texture(u_Texture, v_TexCoord) * u_Color;
}
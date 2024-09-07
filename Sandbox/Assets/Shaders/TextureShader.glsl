#type Vertex
#version 410 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec4 Color;
layout(location = 2) in vec2 TexCoord;
layout(location = 3) in float TexIndex;

uniform mat4 u_VP;
uniform float u_NumTiles;

out vec2 v_TexCoord;
out vec4 v_Color;
out float v_TexIndex;

void main() {
	v_TexIndex = TexIndex;
	v_Color = Color;
	v_TexCoord = TexCoord * 10;
	gl_Position = u_VP  * vec4(position,1.0f);
};

#type Fragment
#version 410 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec4 v_Color;
in float v_TexIndex;

uniform sampler2D u_Texture[32];

void main() {
	color = texture(u_Texture[int(v_TexIndex)], v_TexCoord) * v_Color;
}
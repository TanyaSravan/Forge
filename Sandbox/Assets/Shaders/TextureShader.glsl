#type Vertex
#version 410 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec4 Color;
layout(location = 2) in vec2 TexCoord;

uniform mat4 u_VP;
uniform float u_NumTiles;

out vec2 v_TexCoord;
out vec4 v_Color;

void main() {
	v_Color = Color;
	v_TexCoord = TexCoord * u_NumTiles;
	gl_Position = u_VP  * vec4(position,1.0f);
};

#type Fragment
#version 410 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec4 v_Color;

uniform sampler2D u_Texture;
uniform vec4 u_Color;

void main() {
	//color = texture(u_Texture, v_TexCoord) * u_Color;
	color = v_Color;
}
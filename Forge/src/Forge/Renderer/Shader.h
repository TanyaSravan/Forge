#pragma once
#include <string>

namespace Forge{

	class Shader {
	public:
		Shader(const std::string& vertexShaderSrc, const std::string& fragmentShaderSrc);
		~Shader();

		void Bind();
		void Unbind();
	private:
		uint32_t m_RendererId;
	};
}

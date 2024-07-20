#pragma once
#include <string>

namespace Forge{

	class Shader {
	public:
		virtual ~Shader() {}

		virtual void Bind() const = 0;
		virtual  void Unbind() const = 0;
		static Shader* Create(const std::string& vertexShaderSrc, const std::string& fragmentShaderSrc);
	};
}
 #pragma once
#include <string>
#include "glm/glm.hpp"
#include "Forge/Core.h"

namespace Forge{

	class Shader {
	public:
		virtual ~Shader() {}

		virtual void Bind() const = 0;
		virtual  void Unbind() const = 0;

		static Ref<Shader> Create(const std::string& vertexShaderSrc, const std::string& fragmentShaderSrc);
		static Ref<Shader> Create(const std::string& filepath);
	};
}

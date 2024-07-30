 #pragma once
#include <string>
#include "glm/glm.hpp"
#include "Forge/Core/Core.h"

namespace Forge{

	class Shader {
	public:
		virtual ~Shader() {}

		virtual void Bind() const = 0;
		virtual  void Unbind() const = 0;

		virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) = 0;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) = 0;
		virtual void SetInt(const std::string& name, const int& value) = 0;

		virtual std::string GetName() const = 0;

		static Ref<Shader> Create(const std::string& name,const std::string& vertexShaderSrc, const std::string& fragmentShaderSrc);
		static Ref<Shader> Create(const std::string& filepath);
	};

	class ShaderLibrary {
	public:
		Ref<Shader>Get(const std::string& name);

		bool Exists(const std::string& name) const;

		void Add(const std::string& name, const Ref<Shader>& shader);
		void Add(const Ref<Shader>& shader);

		Ref<Shader> Load(const std::string& filepath);
		Ref<Shader> Load(const std::string& name,const std::string& filepath);
	private:
		std::unordered_map<std::string, Ref<Shader>> m_shaders;
	};
}

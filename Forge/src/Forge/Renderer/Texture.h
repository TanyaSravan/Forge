#pragma once
#include <string>
#include "Forge/Core/Core.h"

namespace Forge {

	class Texture {
	public:
		
		virtual ~Texture() = default;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual uint32_t GetRendererID() const = 0;

		virtual void SetData(const void* data, const int& size) = 0;

		virtual void Bind(unsigned int slot = 0) const = 0;

		virtual bool operator== (const Texture& other) const = 0;
	};

	class Texture2D : public Texture {
	public:
		static Ref<Texture2D> Create(const std::string& filepath);
		static Ref<Texture2D> Create(const int& width, const int& height);
	};
}



#pragma once

#include "Forge/Renderer/Texture.h"

namespace Forge {
	class OpenGlTexture2D : public Texture2D {
	public:

		OpenGlTexture2D(const std::string& filepath);
		~OpenGlTexture2D();

		virtual unsigned int GetWidth() const override { return m_Width; }
		virtual unsigned int GetHeight() const override { return m_Height; }

		virtual void Bind(unsigned int slot = 0) const override;

	private:
		std::string m_filepath;
		unsigned int m_Width, m_Height;

		uint32_t m_RendererId;

	};
}
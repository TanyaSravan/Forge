#pragma once

#include "Forge/Renderer/Texture.h"
#include "glad/glad.h"

namespace Forge {
	class OpenGlTexture2D : public Texture2D {
	public:

		OpenGlTexture2D(const std::string& filepath);
		OpenGlTexture2D(const int& width, const int& height);
		~OpenGlTexture2D();

		virtual unsigned int GetWidth() const override { return m_Width; }
		virtual unsigned int GetHeight() const override { return m_Height; }
		virtual uint32_t GetRendererID() const override { return m_RendererId; }

		virtual void SetData(const void* data, const int& size) override;

		virtual void Bind(unsigned int slot = 0) const override;

		virtual bool operator == (const Texture& other) const override {
			return (m_RendererId == ((OpenGlTexture2D&)other).m_RendererId);
		}

	private:
		std::string m_filepath;
		unsigned int m_Width, m_Height;

		GLenum m_InternalFormat, m_DataFormat;

		uint32_t m_RendererId;

	};
}
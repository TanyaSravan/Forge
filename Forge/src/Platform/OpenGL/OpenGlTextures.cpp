#include "FGpch.h"
#include "OpenGlTextures.h"

#include "stb_image.h"
#include "glad/glad.h"

namespace Forge {

	OpenGlTexture2D::OpenGlTexture2D(const std::string& filepath )
		:m_filepath(filepath)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);

		m_Width = width;
		m_Height = height;

		int filter;
		if (channels == 3)
			filter = GL_RGB;
		else if (channels == 4)
			filter = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererId);
		glTextureStorage2D(m_RendererId, 1, GL_RGB8, m_Width, m_Height);

		glTextureParameteri(m_RendererId, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(m_RendererId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureSubImage2D(m_RendererId, 0, 0, 0, m_Width, m_Width, filter, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGlTexture2D::~OpenGlTexture2D() {
		glDeleteTextures(1, &m_RendererId);
	}

	void OpenGlTexture2D::Bind(unsigned int slot) const {
		glBindTextureUnit(0, m_RendererId);
	}
}
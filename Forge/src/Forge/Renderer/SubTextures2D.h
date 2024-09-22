#pragma once
#include "Forge/Renderer/Texture.h"
#include "glm/glm.hpp"

namespace Forge {

	class SubTextures2D {
	public:
		SubTextures2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);

		glm::vec2* GetTexCoord() { return m_TexCoord; }
		Ref<Texture2D> GetTexture() { return m_Texture; }

		static Ref<SubTextures2D> CreateSubTexture(const Ref<Texture2D>& texture, const glm::vec2& spriteIndex, const glm::vec2& cellSize, const glm::vec2& spriteSize = { 1,1 });
	private:
		Ref<Texture2D> m_Texture;
		glm::vec2 m_TexCoord[4];
	};
}

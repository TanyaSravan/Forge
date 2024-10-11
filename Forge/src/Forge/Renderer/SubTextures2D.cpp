#include "FGpch.h"
#include "SubTextures2D.h"

namespace Forge {
	SubTextures2D::SubTextures2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max)
		:m_Texture(texture)
	{
		m_TexCoord[0] = { min.x,min.y };
		m_TexCoord[1] = { max.x,min.y };
		m_TexCoord[2] = { max.x,max.y };
		m_TexCoord[3] = { min.x,max.y };
	}


	Ref<SubTextures2D> SubTextures2D::CreateSubTexture(const Ref<Texture2D>& texture, const glm::vec2& spriteIndex, const glm::vec2& cellSize, const glm::vec2& spriteSize)
	{
		glm::vec2 min = { (spriteIndex.x * cellSize.x) / texture->GetWidth(), (spriteIndex.y * cellSize.y) / texture->GetHeight() };
		glm::vec2 max = { ((spriteIndex.x + spriteSize.x) * cellSize.x) / texture->GetWidth(), ((spriteIndex.y + spriteSize.y) * cellSize.y) / texture->GetHeight() };
		return std::make_shared<SubTextures2D>(texture, min, max);
		
		
	}
}
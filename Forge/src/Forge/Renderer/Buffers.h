#pragma once
#include <cstdint> 

namespace Forge {

	enum class ShaderDataType
	{
		None = 0,
		Float, Float2, Float3, Float4,
		Int, Int2, Int3, Int4,
		Mat3, Mat4,
		Bool
	};

	static uint32_t GetSizeOfShaderDataType(const ShaderDataType& type) {
		switch (type)
		{
			case Forge::ShaderDataType::None:			FG_CORE_ASSERT(false, "Does notsupport none ShaderType"); return 0;
			case Forge::ShaderDataType::Float:			return sizeof(float);
			case Forge::ShaderDataType::Float2:			return sizeof(float) * 2;
			case Forge::ShaderDataType::Float3:			return sizeof(float) * 3;
			case Forge::ShaderDataType::Float4:			return sizeof(float) * 4;
			case Forge::ShaderDataType::Int:			return sizeof(int);
			case Forge::ShaderDataType::Int2:			return sizeof(int) * 2;
			case Forge::ShaderDataType::Int3:			return sizeof(int) * 3;
			case Forge::ShaderDataType::Int4:			return sizeof(int) * 4;
			case Forge::ShaderDataType::Mat3:			return sizeof(float) * 3 * 3;
			case Forge::ShaderDataType::Mat4:			return sizeof(float) * 4 * 4;
			case Forge::ShaderDataType::Bool:			return sizeof(bool);
		}

		FG_CORE_ASSERT(false, "Unknown Shader DataType");
		return 0;
	}

	struct BufferElement {
		std::string Name;
		ShaderDataType Type;
		uint32_t Offset;
		uint32_t Size;
		uint32_t Normalised;

		BufferElement(const ShaderDataType& type, const std::string name, bool normalised = false)
			:Name(name), Type(type), Offset(0), Size(GetSizeOfShaderDataType(type)),Normalised(normalised) {}

		BufferElement() {}

		uint32_t GetElementCount(const ShaderDataType& type) const {
			switch (type)
			{
				case Forge::ShaderDataType::None:			FG_CORE_ASSERT(false, "Does notsupport none ShaderType"); return 0;
				case Forge::ShaderDataType::Float:			return 1;
				case Forge::ShaderDataType::Float2:			return 2;
				case Forge::ShaderDataType::Float3:			return 3;
				case Forge::ShaderDataType::Float4:			return 4;
				case Forge::ShaderDataType::Int:			return 1;
				case Forge::ShaderDataType::Int2:			return 2;
				case Forge::ShaderDataType::Int3:			return 3;
				case Forge::ShaderDataType::Int4:			return 4;
				case Forge::ShaderDataType::Mat3:			return 3 * 3;
				case Forge::ShaderDataType::Mat4:			return 4 * 4;
				case Forge::ShaderDataType::Bool:			return 1;
			}

			FG_CORE_ASSERT(false, "Unknown Shader DataType");
			return 0;
		}

	};

	class BufferLayout {
	public:

		BufferLayout(const std::initializer_list<BufferElement>&  elements) 
			:m_Elements(elements)
		{
			SetElementOffsetandStride();
		}
		 
		BufferLayout() {}

		inline const std::vector<BufferElement> GetBufferElements() const { return m_Elements; }

		inline uint32_t GetStride() const { return m_Stride; }

		std::vector<BufferElement> ::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement> ::iterator end() { return m_Elements.end(); }

		std::vector<BufferElement> ::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement> ::const_iterator end() const { return m_Elements.end(); }
	private:
		void SetElementOffsetandStride() {
			uint32_t offset = 0;
			for (auto& elem : m_Elements) {
				elem.Offset = offset;
				offset += elem.Size;
				m_Stride += elem.Size;
			}
		}
	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;
	};

	class VertexBuffer {
	public:

		virtual ~VertexBuffer() {}
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void SetLayout(BufferLayout layout) = 0;
		virtual const BufferLayout GetLayout() const = 0;

		static Ref<VertexBuffer> Create(float* vertices, uint32_t size);
	};

	class IndexBuffer {
	public:
		virtual ~IndexBuffer() {}
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount() const = 0;

		static Ref<IndexBuffer> Create(uint32_t* indices, uint32_t size);
	};
}

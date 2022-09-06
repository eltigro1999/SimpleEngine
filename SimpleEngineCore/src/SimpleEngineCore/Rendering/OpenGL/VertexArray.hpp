#pragma once
#include "VertexBuffer.hpp"

namespace SimpleEngine {
	class VertexArray {
	public:
		VertexArray();
		~VertexArray();

		VertexArray(const VertexArray&) = delete;
		VertexArray& operator=(const VertexArray&)=delete;
		VertexArray& operator=(VertexArray&&)noexcept;
		VertexArray(VertexArray&&)noexcept;

		void addBuffer(const VertexBuffer&);
		void bind() const;
		static void unbind();

	private:
		unsigned int m_id=0;
		unsigned int m_elements_count = 0;
	};
}
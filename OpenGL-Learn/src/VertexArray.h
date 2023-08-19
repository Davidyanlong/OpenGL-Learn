#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray {

private:
	unsigned int m_RendererID;
public:
	VertexArray();
	~VertexArray();

	void addBuffer(const VertexBuffer& vb, const VertexBufferLaoyout& layout);

	void Bind();
	void Unbind();
};
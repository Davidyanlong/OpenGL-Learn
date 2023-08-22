#pragma once

#include "VertexBuffer.h"

class VertexBufferLaoyout;

class VertexArray {

private:
	unsigned int m_RendererID;
public:
	VertexArray();
	~VertexArray();

	void addBuffer(const VertexBuffer& vb, const VertexBufferLaoyout& layout);

	void Bind() const;
	void Unbind() const;
};
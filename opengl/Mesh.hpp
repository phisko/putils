#pragma once

#include "opengl/RAII.hpp"

namespace putils::gl {
	struct Mesh {
		putils::gl::VertexArray vertexArrayObject;
		putils::gl::Buffer vertexBuffer;
		putils::gl::Buffer indexBuffer;
		size_t nbIndices = 0;
		GLenum indexType = GL_UNSIGNED_INT;
	};

	void draw(const Mesh & mesh);
}
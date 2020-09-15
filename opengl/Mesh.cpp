#include "Mesh.hpp"

namespace putils::gl {
	void draw(const Mesh & mesh) {
		glBindVertexArray(mesh.vertexArrayObject);
		glBindBuffer(GL_ARRAY_BUFFER, mesh.vertexBuffer);
		glDrawElements(GL_TRIANGLES, (GLsizei)mesh.nbIndices, mesh.indexType, nullptr);
	}
}
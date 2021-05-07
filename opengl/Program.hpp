#pragma once

#ifndef PUTILS_GLM
static_assert(false, "putils::gl::Program requires GLM");
#else
#include "opengl/Uniform.hpp"

#include "glm/glm.hpp"
#include "Rect.hpp"
#include "vector.hpp"
#include "string.hpp"

#ifndef PUTILS_UNIFORM_NAME_MAX_LENGTH
# define PUTILS_UNIFORM_NAME_MAX_LENGTH 256
#endif

namespace putils::gl {
	void setViewPort(const Rect2i & viewport) noexcept;

	template<typename VertexType, typename Member>
	void setAttrib(size_t location, size_t length, size_t offset) noexcept;

	template<typename T>
	void setVertexType() noexcept;
	template<typename VertexType>
	void setPolyvoxVertexType() noexcept;

	class Program {
	public:
		Program(bool usesGBuffer = false, const char * name = "") noexcept;
		virtual ~Program() noexcept;

		virtual void init(size_t firstTextureID) noexcept = 0;

		struct Parameters {
			glm::mat4 view;
			glm::mat4 proj;
			glm::vec3 camPos;
			float camFOV;
			putils::Rect2i viewport;
			float nearPlane;
			float farPlane;
			size_t viewportID;
		};
		virtual void run(const Parameters & params) noexcept = 0;

		void use() noexcept;

		auto getHandle() const noexcept { return _handle; }

		struct ShaderDescription {
			const char * src;
			GLenum type;
		};

		template<typename CRTP, size_t N>
		void initWithShaders(const putils::vector<ShaderDescription, N> & shaders) noexcept;

		void addGBufferTexture(const char * name, int texture) noexcept;

	private:
		void setGBufferAttrib(const char * name, int texture) noexcept;
		GLint loadShader(const char * src, GLenum type) noexcept;

	public:
		bool usesGBuffer() const noexcept { return _usesGBuffer; }

#ifndef PUTILS_NDEBUG
		const auto & getName() const noexcept { return _name; }
#endif

	protected:
		GLuint _handle = -1;
		bool _usesGBuffer;

#ifndef PUTILS_NDEBUG
		putils::string<64> _name;
#endif
	};
}

#include "Program.inl"

#endif
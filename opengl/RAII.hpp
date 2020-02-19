#pragma once

#include "GL/glew.h"
#include "GL/GL.h"

namespace putils::gl {
	struct FrameBuffer;
	struct Texture;
	struct Buffer;
	struct VertexArray;

	// Impl
	struct UIntSwapper {
		GLuint res = (GLuint)-1;

		operator GLuint() const { return res; }

		GLuint & get() { return res; }
		const GLuint get() const { return res; }

		UIntSwapper(UIntSwapper && rhs) noexcept
			: res(rhs.res)
		{
			rhs.res = (GLuint)-1;
		}

		UIntSwapper & operator=(UIntSwapper && rhs) noexcept {
			std::swap(res, rhs.res);
			return *this;
		}

		UIntSwapper() = default;
		UIntSwapper(const UIntSwapper &) = delete;
		UIntSwapper & operator=(const UIntSwapper &) = delete;
	};

	struct FrameBuffer : UIntSwapper {
		~FrameBuffer() {
			glDeleteFramebuffers(1, &res);
		}

		void generate() { glGenFramebuffers(1, &res); }

		FrameBuffer() = default;
		FrameBuffer(FrameBuffer &&) = default;
		FrameBuffer & operator=(FrameBuffer &&) = default;
	};

	struct Texture : UIntSwapper {
		~Texture() {
			glDeleteTextures(1, &res);
		}

		void generate() { glGenTextures(1, &res); }

		Texture() = default;
		Texture(Texture &&) = default;
		Texture & operator=(Texture &&) = default;
	};

	struct Buffer : UIntSwapper {
		~Buffer() {
			glDeleteBuffers(1, &res);
		}

		void generate() { glGenBuffers(1, &res); }

		Buffer() = default;
		Buffer(Buffer &&) = default;
		Buffer & operator=(Buffer &&) = default;
	};

	struct VertexArray : UIntSwapper {
		~VertexArray() {
			glDeleteVertexArrays(1, &res);
		}

		void generate() { glGenVertexArrays(1, &res); }

		VertexArray() = default;
		VertexArray(VertexArray &&) = default;
		VertexArray & operator=(VertexArray &&) = default;
	};
}
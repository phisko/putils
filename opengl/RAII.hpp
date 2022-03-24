#pragma once

#include <utility>
#include "GL/glew.h"
#include "GL/gl.h"

namespace putils::gl {
	struct FrameBuffer;
	struct Texture;
	struct Buffer;
	struct VertexArray;

	// Impl
	struct UIntSwapper {
		GLuint res = (GLuint)-1;

		operator GLuint() const noexcept { return res; }

		GLuint & get() noexcept { return res; }
		const GLuint get() const noexcept { return res; }

		UIntSwapper(UIntSwapper && rhs) noexcept {
			std::swap(res, rhs.res);
		}

		UIntSwapper & operator=(UIntSwapper && rhs) noexcept {
			std::swap(res, rhs.res);
			return *this;
		}

		UIntSwapper() noexcept = default;
		UIntSwapper(const UIntSwapper &) = delete;
		UIntSwapper & operator=(const UIntSwapper &) = delete;
	};

	struct FrameBuffer : UIntSwapper {
		~FrameBuffer() noexcept {
			glDeleteFramebuffers(1, &res);
		}

		void generate() noexcept { glGenFramebuffers(1, &res); }

		FrameBuffer() noexcept = default;
		FrameBuffer(FrameBuffer &&) noexcept = default;
		FrameBuffer & operator=(FrameBuffer &&) noexcept = default;
	};

	struct Texture : UIntSwapper {
		~Texture() noexcept {
			glDeleteTextures(1, &res);
		}

		void generate() noexcept { glGenTextures(1, &res); }

		Texture() noexcept = default;
		Texture(Texture &&) noexcept = default;
		Texture & operator=(Texture &&) noexcept = default;
	};

	struct Buffer : UIntSwapper {
		~Buffer() noexcept {
			glDeleteBuffers(1, &res);
		}

		void generate() noexcept { glGenBuffers(1, &res); }

		Buffer() noexcept = default;
		Buffer(Buffer &&) noexcept = default;
		Buffer & operator=(Buffer &&) noexcept = default;
	};

	struct VertexArray : UIntSwapper {
		~VertexArray() noexcept {
			glDeleteVertexArrays(1, &res);
		}

		void generate() noexcept { glGenVertexArrays(1, &res); }

		VertexArray() noexcept = default;
		VertexArray(VertexArray &&) noexcept = default;
		VertexArray & operator=(VertexArray &&) noexcept = default;
	};
}
#ifdef PUTILS_GLM

#include "Program.hpp"
#include <termcolor/termcolor.hpp>
#include "reflection.hpp"

namespace putils::gl {
	void setViewPort(const putils::Rect2i & viewport) noexcept {
		glViewport(viewport.position.x, viewport.position.y, viewport.size.x, viewport.size.y);
	}

	Program::Program(bool usesGBuffer, const char * name) noexcept
		: _usesGBuffer(usesGBuffer)
#ifndef PUTILS_NDEBUG
		, _name(name)
#endif
	{}

	Program::~Program() noexcept {
		if (_handle != -1)
			glDeleteProgram(_handle);
	}

	void Program::use() noexcept {
		glUseProgram(_handle);
	}

	void Program::addGBufferTexture(const char * name, int texture) noexcept {
		if (!usesGBuffer())
			return;

		assert(_handle != -1);
		use();
		setGBufferAttrib(name, texture);
	}

	void Program::setGBufferAttrib(const char * name, int texture) noexcept {
		const putils::string<64> gName("g%s", name);
		const auto attrib = glGetUniformLocation(_handle, gName.data());
		if (attrib != -1)
			glUniform1i(attrib, texture);
#if !defined(PUTILS_NDEBUG) && !defined(PUTILS_NO_SHADER_DEBUG)
		else {
			std::cerr << termcolor::yellow
				<< "Failed to get location for `" << termcolor::cyan << gName << termcolor::yellow << "` uniform" << std::endl
				<< "\tNote: When building [" << termcolor::cyan << _name << termcolor::yellow << "] program" << std::endl
				<< termcolor::reset;
		}
#endif
	}

	GLint Program::loadShader(const char * src, GLenum type) noexcept {
		const auto shader = glCreateShader(type);
		glShaderSource(shader, 1, &src, nullptr);
		glCompileShader(shader);

#if !defined(PUTILS_NDEBUG) && !defined(PUTILS_NO_SHADER_DEBUG)
		{
			char buffer[512];
			glGetShaderInfoLog(shader, sizeof(buffer), nullptr, buffer);
			if (strlen(buffer) != 0) {
				std::cerr << termcolor::red <<
					"Error compiling program [" << termcolor::cyan << _name << termcolor::red << "]:" << std::endl
					<< '\t' << termcolor::white << buffer << std::endl
					<< termcolor::reset;
				assert(false);
			}

			GLint status;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
			assert(status == GL_TRUE);
		}
#endif
		return shader;
	}


}

#endif
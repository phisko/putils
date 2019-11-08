#pragma once

#include "GL/glew.h"
#include "GL/GL.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Point.hpp"
#include "Color.hpp"

namespace putils::gl {
	inline void setUniform(GLint location, bool val) { glUniform1i(location, val); }
	inline void setUniform(GLint location, int val) { glUniform1i(location, val); }
	inline void setUniform(GLint location, GLuint val) { glUniform1i(location, val); }
	inline void setUniform(GLint location, size_t val) { glUniform1i(location, (GLint)val); }
	inline void setUniform(GLint location, float val) { glUniform1f(location, val); }

	inline void setUniform(GLint location, const float(&val)[3]) { glUniform3f(location, val[0], val[1], val[2]); }
	inline void setUniform(GLint location, const glm::vec3 & val) { glUniform3f(location, val.x, val.y, val.z); }
	inline void setUniform(GLint location, const Point3f & val) { glUniform3f(location, val.x, val.y, val.z); }

	inline void setUniform(GLint location, const float(&val)[4]) { glUniform4f(location, val[0], val[1], val[2], val[3]); }
	inline void setUniform(GLint location, const NormalizedColor & val) { glUniform4f(location, val.r, val.g, val.b, val.a); }

	inline void setUniform(GLint location, const float(&val)[2]) { glUniform2f(location, val[0], val[1]); }
	inline void setUniform(GLint location, const glm::vec2 & val) { glUniform2f(location, val.x, val.y); }
	inline void setUniform(GLint location, const Point2f & val) { glUniform2f(location, val.x, val.y); }

	inline void setUniform(GLint location, const glm::mat4 & val) { glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(val)); }

	template<typename T>
	struct Uniform {
		void operator=(const T & val) const {
			setUniform(location, val);
		}

		GLint location = -1;
	};
}
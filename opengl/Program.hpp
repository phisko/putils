#pragma once

#include "GL/glew.h"
#include "GL/GL.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <functional>
#include <fstream>
#include "Point.hpp"
#include "Color.hpp"

#include "reflection/Reflectible.hpp"
#include "lengthof.hpp"
#include "members.hpp"
#include "read_stream.hpp"
#include "vector.hpp"
#include "string.hpp"

namespace putils {
	namespace gl {
		inline void setUniform(GLint location, int val) { glUniform1i(location, val); }
		inline void setUniform(GLint location, GLuint val) { glUniform1i(location, val); }
		inline void setUniform(GLint location, size_t val) { glUniform1i(location, (GLint)val); }
		inline void setUniform(GLint location, float val) { glUniform1f(location, val); }

		inline void setUniform(GLint location, const float (&val)[3]) { glUniform3f(location, val[0], val[1], val[2]); }
		inline void setUniform(GLint location, const glm::vec3 & val) { glUniform3f(location, val.x, val.y, val.z); }
		inline void setUniform(GLint location, const putils::Point3f & val) { glUniform3f(location, val.x, val.y, val.z); }

		inline void setUniform(GLint location, const float(&val)[4]) { glUniform4f(location, val[0], val[1], val[2], val[3]); }
		inline void setUniform(GLint location, const NormalizedColor & val) { glUniform4f(location, val.r, val.g, val.b, val.a); }

		inline void setUniform(GLint location, const float (&val)[2]) { glUniform2f(location, val[0], val[1]); }
		inline void setUniform(GLint location, const glm::vec2 & val) { glUniform2f(location, val.x, val.y); }
		inline void setUniform(GLint location, const putils::Point2f & val) { glUniform2f(location, val.x, val.y); }

		inline void setUniform(GLint location, const glm::mat4 & val) { glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(val)); }

		template<typename VertexType, typename Member>
		void setAttrib(size_t location, size_t length, size_t offset) {
			glEnableVertexAttribArray((GLuint)location);
			using ArraySubType = std::remove_all_extents_t<Member>;

			static_assert(
				std::is_same_v<ArraySubType, float> ||
				std::is_same_v<ArraySubType, int> ||
				std::is_same_v<ArraySubType, unsigned int>
			);

			if constexpr (std::is_same_v<ArraySubType, float>)
				glVertexAttribPointer((GLuint)location, (GLint)length, GL_FLOAT, GL_FALSE, sizeof(VertexType), (void *)offset);
			else if constexpr (std::is_same_v<ArraySubType, int>)
				glVertexAttribIPointer((GLuint)location, (GLint)length, GL_INT, sizeof(VertexType), (void *)offset);
			else if constexpr (std::is_same_v<ArraySubType, unsigned int>)
				glVertexAttribIPointer((GLuint)location, (GLint)length, GL_UNSIGNED_INT, sizeof(VertexType), (void *)offset);
		}

		template<typename T>
		void setVertexType() {
			size_t location = 0;
			putils::for_each_attribute(T::get_attributes(), [&](const char *, auto member) {
				using Ptr = decltype(member);
				using Member = std::remove_reference_t<decltype(std::declval<T>().*(std::declval<Ptr>()))>;

				constexpr auto length = lengthof(std::declval<Member>());
				setAttrib<T, Member>(location++, length, putils::member_offset(member));
			});
		}

		template<typename VertexType>
		void setPolyvoxVertexType() {
			size_t location = 0;

			setAttrib<VertexType, float[3]>(location++, 3, offsetof(VertexType, position)); // position
			setAttrib<VertexType, float[3]>(location++, 3, offsetof(VertexType, normal)); // position

			using Data = typename VertexType::DataType;
			if constexpr (putils::has_member_get_attributes<Data>()) {
				const auto dataOffset = offsetof(VertexType, data);

				putils::for_each_attribute(Data::get_attributes(), [&](const char *, auto member) {
					using Ptr = decltype(member);
					using Member = std::remove_reference_t<decltype(std::declval<Data>().*(std::declval<Ptr>()))>;

					size_t length = 1;
					if constexpr (std::is_array_v<Member>)
						length = lengthof(std::declval<Member>());

					setAttrib<VertexType, Member>(location++, length, dataOffset + putils::member_offset(member));
				});
			}
		}


		class Program {
		public:
			Program(bool usesGBuffer = false, const char * name = "") : _usesGBuffer(usesGBuffer)
#ifndef NDEBUG
				, _name(name)
#endif
			{}

			~Program() {
				if (_handle != -1)
					glDeleteProgram(_handle);
			}

			virtual void init(size_t firstTextureID, size_t screenWidth, size_t screenHeight, GLuint gBufferFBO) = 0;

			struct Parameters {
				glm::mat4 view;
				glm::mat4 proj;
				glm::vec3 camPos;
				glm::vec2 screenSize;
				float nearPlane;
				float farPlane;
			};
			virtual void run(const Parameters & params) = 0;

			void use() { glUseProgram(_handle); }

			struct Texture {
				putils::string<64> name;
				GLuint id = (GLuint)-1;
			};
			putils::vector<Texture, 4> texturesToDebug;

			struct ShaderDescription {
				const char * src;
				GLenum type;
			};

			auto getHandle() const { return _handle; }

			template<typename CRTP, size_t N>
			void initWithShaders(const putils::vector<ShaderDescription, N> & shaders) {
				assert(_handle == -1);
				// if (_handle != -1)
				// 	glDeleteProgram(_handle);
				_handle = glCreateProgram();

				for (const auto & shader : shaders)
					glAttachShader(_handle, loadShader(shader.src, shader.type));

				// glBindFragDataLocation(_handle, 0, "outColor"); // Not necessary right now since fragment has only one output
				glLinkProgram(_handle);
#if !defined(NDEBUG) && !defined(PUTILS_NO_SHADER_DEBUG)
				char buffer[512];
				glGetProgramInfoLog(_handle, lengthof(buffer), nullptr, buffer);
				if (strlen(buffer) != 0) {
					std::cerr << "Error linking shaders:\n\t" << buffer << '\n';
					std::cerr << "\tNote: When building [" << _name << "] program\n";
					assert(false);
				}

				GLint success;
				glGetProgramiv(_handle, GL_LINK_STATUS, &success);
				assert(success == GL_TRUE);
#endif
				glUseProgram(_handle);

				putils::for_each_attribute(CRTP::get_attributes(), [&](const char * name, auto member) {
					auto & crtp = static_cast<CRTP &>(*this);
					auto & uniformLocation = crtp.*member;
					uniformLocation = glGetUniformLocation(_handle, name);
#if !defined(NDEBUG) && !defined(PUTILS_NO_SHADER_DEBUG)
					if (uniformLocation == -1) {
						std::cerr << "Failed to get location for `" << name << "` uniform\n";
						std::cerr << "\tNote: When building [" << _name << "] program\n";
					}
#endif
				});
			}

			void addGBufferTexture(const char * name, int texture) {
				if (!usesGBuffer())
					return;

				assert(_handle != -1);
				use();
				setGBufferAttrib(name, texture);
			}

		private:
			void setGBufferAttrib(const char * name, int texture) {
				const putils::string<64> gName("g%s", name);
				const auto attrib = glGetUniformLocation(_handle, gName.data());
				if (attrib != -1)
					glUniform1i(attrib, texture);
#if !defined(NDEBUG) && !defined(PUTILS_NO_SHADER_DEBUG)
				else {
					std::cerr << "Failed to get location for `" << gName << "` uniform\n";
					std::cerr << "\tNote: When building [" << _name << "] program\n";
				}
#endif
			}

			static GLint loadShader(const char * src, GLenum type) {
				const auto shader = glCreateShader(type);
				glShaderSource(shader, 1, &src, nullptr);
				glCompileShader(shader);

#if !defined(NDEBUG) && !defined(PUTILS_NO_SHADER_DEBUG)
				{
					char buffer[512];
					glGetShaderInfoLog(shader, sizeof(buffer), nullptr, buffer);
					if (strlen(buffer) != 0) {
						std::cerr << "Error compiling program:\n\t" << buffer << '\n';
						assert(false);
					}

					GLint status;
					glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
					assert(status == GL_TRUE);
				}
#endif
				return shader;
			}

		public:
			bool usesGBuffer() const { return _usesGBuffer; }

#ifndef NDEBUG
			const auto & getName() const { return _name; }
#endif

		protected:
			GLuint _handle = -1;
			bool _usesGBuffer;

#ifndef NDEBUG
			putils::string<64> _name;
#endif
		};
	}
}
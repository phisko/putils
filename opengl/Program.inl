#include "Program.hpp"
#include "termcolor.hpp"
#include "lengthof.hpp"

namespace putils::gl {
	template<typename VertexType, typename Member>
	void setAttrib(size_t location, size_t length, size_t offset) noexcept {
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
	void setVertexType() noexcept {
		size_t location = 0;
		putils::reflection::for_each_attribute<T>([&](const char *, auto member) noexcept {
			using Member = putils::MemberType<putils_typeof(member)>;

			constexpr auto length = putils::lengthof<Member>();
			setAttrib<T, Member>(location++, length, putils::member_offset(member));
		});
	}

	template<typename VertexType>
	void setPolyvoxVertexType() noexcept {
		size_t location = 0;

		setAttrib<VertexType, float[3]>(location++, 3, offsetof(VertexType, position)); // position
		setAttrib<VertexType, float[3]>(location++, 3, offsetof(VertexType, normal)); // position

		using Data = typename VertexType::DataType;
		if constexpr (putils::reflection::has_attributes<Data>()) {
			const auto dataOffset = offsetof(VertexType, data);

			putils::reflection::for_each_attribute<Data>([&](const char *, auto member) noexcept {
				using Ptr = decltype(member);
				using Member = std::remove_reference_t<decltype(std::declval<Data>().*(std::declval<Ptr>()))>;

				size_t length = 1;
				if constexpr (std::is_array_v<Member>)
					length = lengthof<Member>();

				setAttrib<VertexType, Member>(location++, length, dataOffset + putils::member_offset(member));
				});
		}
	}

	template<typename CRTP, size_t N>
	void Program::initWithShaders(const putils::vector<ShaderDescription, N> & shaders) noexcept {
		if (_handle != -1)
			glDeleteProgram(_handle);
		_handle = glCreateProgram();

		for (const auto & shader : shaders)
			glAttachShader(_handle, loadShader(shader.src, shader.type));

		glLinkProgram(_handle);
#if !defined(PUTILS_NDEBUG) && !defined(PUTILS_NO_SHADER_DEBUG)
		char buffer[512];
		glGetProgramInfoLog(_handle, (GLsizei)lengthof(buffer), nullptr, buffer);
		if (strlen(buffer) != 0) {
			std::cerr << putils::termcolor::red << "Error linking shaders:\n\t" << buffer << '\n';
			std::cerr << "\tNote: When building [" << putils::termcolor::cyan << _name << putils::termcolor::red << "] program\n" << putils::termcolor::reset;
			assert(false);
		}

		GLint success;
		glGetProgramiv(_handle, GL_LINK_STATUS, &success);
		assert(success == GL_TRUE);
#endif
		glUseProgram(_handle);

		putils::reflection::for_each_attribute(static_cast<CRTP &>(*this), [&](const char * name, auto && uniformLocation) {
			static const auto setLocation = [this](GLint & loc, const char * name) noexcept {
				loc = glGetUniformLocation(_handle, name);
#if !defined(PUTILS_NDEBUG) && !defined(PUTILS_NO_SHADER_DEBUG)
				if (loc == -1) {
					std::cerr << putils::termcolor::yellow <<
						"Failed to get location for `" << putils::termcolor::cyan << name << putils::termcolor::yellow << "` uniform\n"
						"\tNote: When building [" << putils::termcolor::cyan << _name << putils::termcolor::yellow << "] program\n"
						<< putils::termcolor::reset;
				}
#endif
			};

			if constexpr (std::is_array<std::remove_reference_t<decltype(uniformLocation)>>::value) {
				for (size_t i = 0; i < lengthof(uniformLocation); ++i) {
					using SubType = std::remove_reference_t<decltype(uniformLocation[i])>;
					if constexpr (std::is_same_v<SubType, GLint>)
						setLocation(uniformLocation[i], putils::string<PUTILS_UNIFORM_NAME_MAX_LENGTH>("%s[%d]", name, i));
					else // should be putils::gl::Uniform<T>
						setLocation(uniformLocation[i].location, putils::string<PUTILS_UNIFORM_NAME_MAX_LENGTH>("%s[%d]", name, i));
				}
			}
			else if constexpr (std::is_same_v<putils_typeof(uniformLocation), GLint>)
				setLocation(uniformLocation, name);
			else // should be putils::gl::Uniform<T>
				setLocation(uniformLocation.location, name);
			});
	}

}

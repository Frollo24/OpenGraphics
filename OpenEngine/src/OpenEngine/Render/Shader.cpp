#include "ogpch.h"
#include "Shader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace OpenGraphics
{
    static GLenum ShaderTypeToGLenum(const ShaderType type)
	{
		switch (type)
		{
			using enum OpenGraphics::ShaderType;
			case None:     return 0;
			case Vertex:   return GL_VERTEX_SHADER;
			case Fragment: return GL_FRAGMENT_SHADER;

			default:
				return 0;
		}
	}

	static std::string ReadFile(const std::string& filepath)
	{
		std::string result;
		std::ifstream in(filepath, std::ios::in); // ifstream closes itself due to RAII
		if (in)
		{
			in.seekg(0, std::ios::end);
			size_t size = in.tellg();
			if (size != -1)
			{
				result.resize(size);
				in.seekg(0, std::ios::beg);
				in.read(&result[0], size);
			}
			else
			{
				Logger::Error("Could not read from file '{0}'", filepath);
			}
		}
		else
		{
			Logger::Error("Could not open file '{0}'", filepath);
		}

		return result;
	}

	static GLuint Compile(const ShaderStringMap& shaderSources)
	{
		GLuint program = glCreateProgram();
		std::array<GLuint, (int)ShaderType::Count - 1> glShaderIDs{};  // ShaderType::None is also a shader type
		int shaderIndex = 0;
		for (const auto& [type, source] : shaderSources) {
			if (type == ShaderType::None) continue;

			GLenum shaderType = ShaderTypeToGLenum(type);

			GLuint shaderID = glCreateShader(shaderType);
			const char* c_source = source.c_str();
			glShaderSource(shaderID, 1, &c_source, 0);
			glCompileShader(shaderID);

			GLint isCompiled = 0;
			glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shaderID, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shaderID);

				Logger::Error("{0}", infoLog.data());
				// TODO: Add proper assertions
				assert(false && "Shader compilation failure!");
				break;
			}

			glAttachShader(program, shaderID);
			glShaderIDs[shaderIndex++] = shaderID;
		}

		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			for (const GLuint& id : glShaderIDs)
			{
				glDetachShader(program, id);
				glDeleteShader(id);
			}

			glDeleteProgram(program);
			program = 0;

			Logger::Error("{0}", infoLog.data());
			// TODO: Add proper assertions
			assert(false && "Shader linking failure!");
		}

		for (const GLuint& id : glShaderIDs)
		{
			glDetachShader(program, id);
			glDeleteShader(id);
		}

		return program;
	}

    Shader::Shader(const std::vector<ShaderSpecs> &shaderFiles)
    {
    	ShaderStringMap sources{};
    	sources.reserve(shaderFiles.size());
    	for (const auto& shader : shaderFiles)
    	{
    		std::string source = ReadFile(shader.Filepath);
    		sources[shader.Type] = source;
    	}
    	m_RendererID = Compile(sources);
    }

    Shader::~Shader()
	{
    	glDeleteProgram(m_RendererID);
    }

    GLint Shader::GetUniformLocation(const std::string& name) const
    {
    	if (auto locationCache = m_UniformLocationCache.find(name);
			locationCache != m_UniformLocationCache.end()) return locationCache->second;

    	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    	m_UniformLocationCache[name] = location;
    	return location;
    }

    void Shader::SetFloat4(const std::string& name, const Vector4D& value) const
	{
    	GLint location = GetUniformLocation(name);
    	glProgramUniform4f(m_RendererID, location, value.x, value.y, value.z, value.w);
    }

	void Shader::SetColor(const std::string& name, const Color& color) const
	{
    	GLint location = GetUniformLocation(name);
    	glProgramUniform4f(m_RendererID, location, color.r, color.g, color.b, color.a);
    }

    void Shader::SetMat4(const std::string& name, const Matrix4x4& matrix) const
	{
    	GLint location = GetUniformLocation(name);
    	glProgramUniformMatrix4fv(m_RendererID, location, 1, GL_FALSE, glm::value_ptr(matrix.toGlmMatrix()));
    }
}

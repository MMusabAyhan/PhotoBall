#include "Shader.h"
#include "Renderer2D.h"
#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string& filepath) 
	: m_filePath(filepath), m_RendererID(0)
{
	ShaderProgramSource source;
	source = ParseShader(filepath);
	m_RendererID = CreateShader(source.vertexSource, source.fragmentSource);
}

Shader::~Shader() 
{
	glDeleteProgram(m_RendererID);
}

void Shader::Bind() const 
{
	glUseProgram(m_RendererID);
}

void Shader::Unbind() const 
{
	glUseProgram(0);
}

unsigned int Shader::GetUniformLocation(const std::string& name)
{
	if (m_CachedLocations.find(name) != m_CachedLocations.end())
		return m_CachedLocations[name];

	int location = glGetUniformLocation(m_RendererID, name.c_str());

	if (location == -1)
		std::cout << "Warning : Uniform " << name << " doesn't exist!" << std::endl;

	m_CachedLocations.emplace(name, location);

	return location;
}

void Shader::SetUniform4f(const std::string& name, const float v0, const float v1, const float v2, const float v3 )
{
	int location = GetUniformLocation(name);
	glUniform4f(location, v0, v1, v2, v3);
}

void Shader::SetUniformMatrix4fv(const std::string& name, const float* value)
{
	int location = GetUniformLocation(name);
	glUniformMatrix4fv(location, 1, GL_FALSE, value);
}    

ShaderProgramSource Shader::ParseShader(const std::string& filePath)
{
    enum class shaderType
    {
        none = -1,
        vertex = 0,
        fragment = 1
    };

    shaderType shaderType;

    std::ifstream stream(filePath);

    std::string line;
    std::stringstream ss[2];

    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                shaderType = shaderType::vertex;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                shaderType = shaderType::fragment;
            }
        }

        else
        {
            ss[(int)shaderType] << line << "\n";
        }
    }

    return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        char* message = (char*)alloca(sizeof(char) * length);

        glGetShaderInfoLog(id, length, &length, message);

        std::cout << "Could not compile"
            << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment")
            << "Shader" << std::endl;

        std::cout << message;

        glDeleteShader(id);

        return 0;
    }

    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);
    glValidateProgram(program);

    glDetachShader(program, vs);
    glDetachShader(program, fs);
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}
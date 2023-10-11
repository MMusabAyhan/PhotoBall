#pragma once

#include <string>
#include <unordered_map>

struct ShaderProgramSource
{
    std::string vertexSource;
    std::string fragmentSource;
};

class Shader 
{
public:

	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

    void SetUniform4f(const std::string& name, const float v0, const float v1, const float v2, const float v3);
	void SetUniformMatrix4fv(const std::string& name, const float* value);

private:

    ShaderProgramSource ParseShader(const std::string& filePath);
    unsigned int CompileShader(unsigned int type, const std::string& source);
    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

    unsigned int GetUniformLocation(const std::string& name);

private:

	unsigned int m_RendererID;
    std::string m_filePath;
    std::unordered_map<std::string, int> m_CachedLocations;
};

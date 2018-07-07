//
// Created by strongheart on 7/7/18.
//

#ifndef PARTICLE_ATTRACTOR_OPENGL_SHADER_H
#define PARTICLE_ATTRACTOR_OPENGL_SHADER_H

#include <string>
#include <unordered_map>

class Shader {
private:
    std::string m_FilePath;
    unsigned int m_RendererID;
    std::unordered_map<std::string, int> m_UniformLocationCache; //caching for uniforms
public:
    Shader(const std::string& filePath);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    //Set Uniforms
    void SetUniform1i(const std::string& name, int value);
    void SetUniform1f(const std::string& name, float value);

private:
    struct ShaderProgramSource
    {
        std::string VertexSource;
        std::string FragmentSource;
    };

    ShaderProgramSource ParseShader(const std::string& filePath);
    unsigned int CompileShader(unsigned int type, const std::string& source);
    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
    int GetUniformLocation(const std::string& name);
};

#endif //PARTICLE_ATTRACTOR_OPENGL_SHADER_H

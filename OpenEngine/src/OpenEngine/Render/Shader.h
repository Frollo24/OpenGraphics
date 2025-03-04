#pragma once

#include "OpenEngine/Core/Base.h"
#include "OpenEngine/Math/Color.h"
#include "OpenEngine/Math/Vector4D.h"
#include "OpenEngine/Math/Matrix4x4.h"

typedef int GLint;

namespace OpenGraphics
{
    enum class ShaderType
    {
        None = 0,
        Vertex,
        Fragment,
        Count
    };

    struct ShaderSpecs
    {
        std::string Filepath = "";
        ShaderType Type = ShaderType::None;
    };

    using ShaderStringMap = std::unordered_map<ShaderType, std::string>;

    class OPEN_API Shader
    {
    public:
        Shader(const std::vector<ShaderSpecs>& shaderFiles);
        ~Shader();

        void SetFloat(const std::string& name, const float& value) const;
        void SetFloat3(const std::string& name, const Vector3D& value) const;
        void SetFloat4(const std::string& name, const Vector4D& value) const;
        void SetColor(const std::string& name, const Color& color) const;
        void SetMat4(const std::string& name, const Matrix4x4& matrix) const;

        [[nodiscard]] inline uint32_t GetRendererID() const { return m_RendererID; }

    private:
        GLint GetUniformLocation(const std::string& name) const;

    private:
        mutable std::unordered_map<std::string, GLint> m_UniformLocationCache;
        uint32_t m_RendererID = 0;
    };

}

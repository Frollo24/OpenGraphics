#pragma once

#include "OpenEngine/Core/Build.h"

#include <stdint.h>
#include <vector>
#include <string>
#include <iostream>

namespace OpenGraphics
{
    enum class ShaderDataType {
        None = 0,
        Bool, Int, Int2, Int3, Int4,
        Float, Float2, Float3, Float4,
        Mat3, Mat4
    };

    static uint32_t ShaderDataTypeSize(ShaderDataType type)
    {
        switch (type)
        {
            case ShaderDataType::Bool:     return 1;
            case ShaderDataType::Int:      return 4;
            case ShaderDataType::Int2:     return 4 * 2;
            case ShaderDataType::Int3:     return 4 * 3;
            case ShaderDataType::Int4:     return 4 * 4;
            case ShaderDataType::Float:    return 4;
            case ShaderDataType::Float2:   return 4 * 2;
            case ShaderDataType::Float3:   return 4 * 3;
            case ShaderDataType::Float4:   return 4 * 4;
            case ShaderDataType::Mat3:     return 4 * 3 * 3;
            case ShaderDataType::Mat4:     return 4 * 4 * 4;
            default:
                std::cerr << "Unknown ShaderDataType!" << std::endl;
            return 0;
        }
    }

    struct VertexAttribute
    {
        std::string Name;
        ShaderDataType Type = ShaderDataType::None;
        uint32_t Size = 0;
        uint32_t Offset = 0;
        bool Normalized = false;

        VertexAttribute() = default;
        VertexAttribute(const ShaderDataType type, const std::string& name, const bool normalized = false)
            : Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized) {}

        uint32_t GetComponentCount() const
        {
            switch (Type)
            {
                case ShaderDataType::Float:   return 1;
                case ShaderDataType::Float2:  return 2;
                case ShaderDataType::Float3:  return 3;
                case ShaderDataType::Float4:  return 4;
                case ShaderDataType::Mat3:    return 3; // 3 * float3
                case ShaderDataType::Mat4:    return 4; // 4 * float4
                case ShaderDataType::Int:     return 1;
                case ShaderDataType::Int2:    return 2;
                case ShaderDataType::Int3:    return 3;
                case ShaderDataType::Int4:    return 4;
                case ShaderDataType::Bool:    return 1;
                default:
                    return 0;
            }
        }
    };

    struct OPEN_API VertexAttribBinding
    {
    public:
        enum class InputRate { Vertex, Instance };

        VertexAttribBinding(const std::initializer_list<VertexAttribute>& attributes) : m_Attributes(attributes) { CalculateStrideAndOffset(); }
        VertexAttribBinding(const std::vector<VertexAttribute>& attributes) : m_Attributes(attributes) { CalculateStrideAndOffset(); }

        inline const std::vector<VertexAttribute>& GetAttributes() const { return m_Attributes; }
        inline InputRate GetInputRate() const { return m_InputRate; }
        inline uint32_t GetBinding() const { return m_Binding; }
        inline uint32_t GetStride() const { return m_Stride; }

        inline void SetInputRate(const InputRate& inputRate) { m_InputRate = inputRate; }

    private:
        void CalculateStrideAndOffset();
        friend class VertexArray;

    protected:
        std::vector<VertexAttribute> m_Attributes;
        InputRate m_InputRate = InputRate::Vertex;
        uint32_t m_Binding = 0;
        uint32_t m_Stride = 0;
    };

    class OPEN_API VertexArray
    {
    public:
        VertexArray(const std::vector<VertexAttribBinding>& bindings);
        ~VertexArray();

        inline const std::vector<VertexAttribBinding>& GetBindings() const { return m_Bindings; }
        inline uint32_t GetRendererID() const { return m_RendererID; }

    private:
        void HandleAttributes(const VertexAttribBinding& binding, const uint32_t& bindingIndex, uint32_t& currentAttribIndex) const;

    private:
        std::vector<VertexAttribBinding> m_Bindings;
        uint32_t m_RendererID = 0;
    };

} // OpenGraphics

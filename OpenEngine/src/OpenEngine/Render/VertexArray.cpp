#include "ogpch.h"
#include "VertexArray.h"

#include <glad/glad.h>

namespace OpenGraphics
{
    void VertexAttribBinding::CalculateStrideAndOffset()
    {
        uint32_t offset = 0;
        m_Stride = 0;
        for (auto& element : m_Attributes) {
            element.Offset = offset;
            offset += element.Size;
            m_Stride += element.Size;
        }
    }

    VertexArray::VertexArray(const std::vector<VertexAttribBinding>& bindings)
        : m_Bindings(bindings)
    {
        int i = 0;
        for (VertexAttribBinding& binding : m_Bindings)
        {
            binding.m_Binding = i;
            i++;
        }

        glCreateVertexArrays(1, &m_RendererID);

        GLuint attribIndex = 0;
        GLuint bindingIndex = 0;
        for (const auto& binding : m_Bindings)
        {
            HandleAttributes(binding, bindingIndex, attribIndex);
            bindingIndex++;
        }
    }

    VertexArray::~VertexArray()
    {
        glDeleteVertexArrays(1, &m_RendererID);
    }

    void VertexArray::HandleAttributes(const VertexAttribBinding& binding, const uint32_t& bindingIndex, uint32_t& currentAttribIndex) const
    {
        for (const auto& attribute : binding.GetAttributes())
        {
            switch (attribute.Type)
            {
                using enum OpenGraphics::ShaderDataType;
                case None:
                {
                    std::cerr << "ShaderDataType of Attribute must not be None!" << std::endl;
                    break;
                }
                case Float:
                case Float2:
                case Float3:
                case Float4:
                {
                    glEnableVertexArrayAttrib(m_RendererID, currentAttribIndex);
                    glVertexArrayAttribBinding(m_RendererID, currentAttribIndex, bindingIndex);
                    glVertexArrayAttribFormat(m_RendererID, currentAttribIndex, attribute.GetComponentCount(),
                        GL_FLOAT, attribute.Normalized ? GL_TRUE : GL_FALSE, attribute.Offset);
                    currentAttribIndex++;
                    break;
                }
                case Bool:
                case Int:
                case Int2:
                case Int3:
                case Int4:
                {
                    glEnableVertexArrayAttrib(m_RendererID, currentAttribIndex);
                    glVertexArrayAttribBinding(m_RendererID, currentAttribIndex, bindingIndex);
                    glVertexArrayAttribIFormat(m_RendererID, currentAttribIndex, attribute.GetComponentCount(),
                        GL_INT, attribute.Offset);
                    currentAttribIndex++;
                    break;
                }
                case Mat3:
                case Mat4:
                {
                    uint32_t count = attribute.GetComponentCount();
                    for (uint8_t i = 0; i < count; i++)
                    {
                        glEnableVertexArrayAttrib(m_RendererID, currentAttribIndex);
                        glVertexArrayAttribBinding(m_RendererID, currentAttribIndex, bindingIndex);
                        glVertexArrayAttribFormat(m_RendererID, currentAttribIndex, count, GL_FLOAT,
                            attribute.Normalized ? GL_TRUE : GL_FALSE, attribute.Offset + sizeof(float) * count * i);
                        currentAttribIndex++;
                    }
                    break;
                }
                default:
                {
                    std::cerr << "Unknown Attribute ShaderDataType!" << std::endl;
                    break;
                }
            }
        }

        using InputRate = VertexAttribBinding::InputRate;
        if (binding.GetInputRate() == InputRate::Instance)
            glVertexArrayBindingDivisor(m_RendererID, bindingIndex, 1);
    }
}

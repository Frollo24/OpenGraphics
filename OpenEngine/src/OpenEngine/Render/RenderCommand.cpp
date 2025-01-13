#include "ogpch.h"
#include "RenderCommand.h"

#include <glad/glad.h>

namespace OpenGraphics
{
    void RenderCommand::BeginFrame()
    {
        glClearColor(0.2f, 0.3f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void RenderCommand::EndFrame()
    {
        // Reset buffer writing to true for any buffer
        glDepthMask(GL_TRUE);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    }

    void RenderCommand::BindVertexArray(const VertexArray* vertexArray)
    {
        s_RenderState.VertexArray = const_cast<VertexArray*>(vertexArray);
        glBindVertexArray(vertexArray->GetRendererID());
    }

    void RenderCommand::SetVertexBuffer(const Buffer* vertexBuffer, const VertexAttribBinding& binding)
    {
        GLuint vertexArrayID = s_RenderState.VertexArray->GetRendererID();
        GLuint vertexBufferID = vertexBuffer->GetRendererID();
        glBindVertexArray(vertexArrayID);
        glVertexArrayVertexBuffer(vertexArrayID, binding.GetBinding(), vertexBufferID, 0, binding.GetStride());
    }

    void RenderCommand::SetIndexBuffer(const Buffer *indexBuffer)
    {
        GLuint vertexArrayID = s_RenderState.VertexArray->GetRendererID();
        GLuint indexBufferID = indexBuffer->GetRendererID();
        glBindVertexArray(vertexArrayID);
        glVertexArrayElementBuffer(vertexArrayID, indexBufferID);
    }

    void RenderCommand::UseShader(const Shader *shader)
    {
        glUseProgram(shader->GetRendererID());
    }

    void RenderCommand::Draw(uint32_t vertexCount)
    {
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    }

    void RenderCommand::DrawIndexed(uint32_t indexCount)
    {
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
    }
}

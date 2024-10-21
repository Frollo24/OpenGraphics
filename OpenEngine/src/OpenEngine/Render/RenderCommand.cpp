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
}

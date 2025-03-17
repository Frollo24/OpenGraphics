#include "ogpch.h"
#include "Texture.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace OpenGraphics
{
    static GLenum GetOpenGLTarget(const TextureDescription& desc)
    {
        if (static_cast<int>(desc.SampleCount) > 1)
        {
            switch (desc.ImageType)
            {
                case ImageType::Image2D: return GL_TEXTURE_2D_MULTISAMPLE;
                case ImageType::Image3D: return GL_TEXTURE_2D_MULTISAMPLE_ARRAY;
                default:
                    return GL_NONE;
            }
        }

        switch (desc.ImageType)
        {
            case ImageType::Image1D: return GL_TEXTURE_1D;
            case ImageType::Image2D: return GL_TEXTURE_2D;
            case ImageType::Image3D: return GL_TEXTURE_3D;
            case ImageType::Cubemap: return GL_TEXTURE_CUBE_MAP;
            default:
                return GL_NONE;
        }
    }

	static GLenum ImageFormatToOpenGLInternalFormat(const ImageFormat format)
	{
		switch (format)
		{
			case ImageFormat::None:    return GL_NONE;
			case ImageFormat::RGB8:    return GL_RGB8;
			case ImageFormat::RGBA8:   return GL_RGBA8;
			case ImageFormat::RGB9E5:  return GL_RGB9_E5;
			case ImageFormat::RG16F:   return GL_RG16F;
			case ImageFormat::RGB16F:  return GL_RGB16F;
			case ImageFormat::RGBA16F: return GL_RGBA16F;
			case ImageFormat::DEPTH32: return GL_DEPTH_COMPONENT32;
			default:
				return GL_NONE;
		}
	}

	static GLenum ImageFormatToOpenGLFormat(const ImageFormat format)
	{
		switch (format)
		{
			case ImageFormat::None:    return GL_NONE;
			case ImageFormat::RGB8:    return GL_RGB;
			case ImageFormat::RGBA8:   return GL_RGBA;
			case ImageFormat::RGB9E5:  return GL_RGB;
			case ImageFormat::RG16F:   return GL_RG;
			case ImageFormat::RGB16F:  return GL_RGB;
			case ImageFormat::RGBA16F: return GL_RGBA;
			case ImageFormat::DEPTH32: return GL_DEPTH_COMPONENT;
			default:
				return GL_NONE;
		}
	}

	static GLenum ImageFormatToOpenGLType(const ImageFormat format)
	{
		switch (format)
		{
			case ImageFormat::None:    return GL_NONE;
			case ImageFormat::RGB8:    return GL_UNSIGNED_BYTE;
			case ImageFormat::RGBA8:   return GL_UNSIGNED_BYTE;
			case ImageFormat::RGB9E5:  return GL_UNSIGNED_BYTE;
			case ImageFormat::RG16F:   return GL_FLOAT;
			case ImageFormat::RGB16F:  return GL_FLOAT;
			case ImageFormat::RGBA16F: return GL_FLOAT;
			case ImageFormat::DEPTH32: return GL_FLOAT;
			default:
				return GL_NONE;
		}
	}

	static GLint TextureFilterModeToOpenGLFilterMode(const TextureFilterMode filterMode, const TextureMipmapFilterMode mipmapMode)
	{
		switch (mipmapMode)
		{
			case TextureMipmapFilterMode::NoMipmap:
				return filterMode == TextureFilterMode::Linear ? GL_LINEAR : GL_NEAREST;
			case TextureMipmapFilterMode::NearestMipmap:
				return filterMode == TextureFilterMode::Linear ? GL_LINEAR_MIPMAP_NEAREST : GL_NEAREST_MIPMAP_NEAREST;
			case TextureMipmapFilterMode::LinearMipmap:
				return filterMode == TextureFilterMode::Linear ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_LINEAR;
			default:
				return GL_NONE;
		}
	}

	static GLint TextureMagFilterModeToOpenGLFilterMode(const TextureFilterMode filterMode)
	{
	    switch (filterMode)
	    {
	    	case TextureFilterMode::Linear:  return GL_LINEAR;
	    	case TextureFilterMode::Nearest: return GL_NEAREST;
	    	default:
	    		return GL_NONE;
	    }
    }

	static GLint TextureWrapModeToOpenGLWrapMode(const TextureWrapMode wrapMode)
	{
		switch (wrapMode)
		{
			case TextureWrapMode::Repeat:      return GL_REPEAT;
			case TextureWrapMode::ClampEdge:   return GL_CLAMP_TO_EDGE;
			case TextureWrapMode::ClampBorder: return GL_CLAMP_TO_BORDER;
			case TextureWrapMode::Mirror:      return GL_MIRRORED_REPEAT;
			case TextureWrapMode::MirrorOnce:  return GL_MIRROR_CLAMP_TO_EDGE;
			default:
				return GL_NONE;
		}
	}

	static GLsizei CalculateMipmapLevels(const ImageExtent& extent)
	{
		const double maxElement = std::max(std::max(extent.Width, extent.Height), extent.Depth);
		const GLsizei mipLevels = static_cast<GLsizei>(std::floor(std::log2(maxElement))) + 1;
		return mipLevels;
	}

    Texture::Texture(const TextureDescription& desc)
        : m_TextureDesc(desc)
    {
        const GLenum target = GetOpenGLTarget(desc);
        glCreateTextures(target, 1, &m_RendererID);

        bool isMultisampled = static_cast<int>(desc.SampleCount) > 1;
        if (!isMultisampled)
        {
	        const GLint filterMode = TextureFilterModeToOpenGLFilterMode(desc.FilterMode, desc.MipmapMode);
        	const GLint magFilterMode = TextureMagFilterModeToOpenGLFilterMode(desc.FilterMode);
            glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, filterMode);
            glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, magFilterMode);

	        const GLint wrapMode = TextureWrapModeToOpenGLWrapMode(desc.WrapMode);
            glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, wrapMode);
            glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, wrapMode);
            glTextureParameterfv(m_RendererID, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(desc.BorderColor.toGlmVector()));
        }

        GLsizei mipLevels = 1;
        if (desc.GenerateMipmaps)
            mipLevels = CalculateMipmapLevels(desc.ImageExtent);

        GLenum internalFormat = ImageFormatToOpenGLInternalFormat(desc.ImageFormat);
        const ImageExtent& extent = desc.ImageExtent;
    	const GLsizei width = static_cast<GLsizei>(desc.ImageExtent.Width);
    	const GLsizei height = static_cast<GLsizei>(desc.ImageExtent.Height);
    	const GLsizei depth = static_cast<GLsizei>(desc.ImageExtent.Depth);
    	const GLsizei sampleCount = static_cast<GLsizei>(desc.SampleCount);
        if (isMultisampled)
        {
            switch (desc.ImageType)
            {
                case ImageType::Image2D:
                    glTextureStorage2DMultisample(m_RendererID, sampleCount, internalFormat, width, height, GL_FALSE);
                break;
                case ImageType::Image3D:
                    glTextureStorage3DMultisample(m_RendererID, sampleCount, internalFormat, width, height, depth, GL_FALSE);
                break;
                default:
                    break;
            }
        }
        else
        {
            switch (desc.ImageType)
            {
                case ImageType::Image1D:
                    glTextureStorage1D(m_RendererID, mipLevels, internalFormat, width);
                break;
                case ImageType::Image2D:
                case ImageType::Cubemap:
                    glTextureStorage2D(m_RendererID, mipLevels, internalFormat, width, height);
                break;
                case ImageType::Image3D:
                    glTextureStorage3D(m_RendererID, mipLevels, internalFormat, width, height, depth);
                break;
                default:
                    break;
            }
        }
    }

    Texture::~Texture()
    {
        glDeleteTextures(1, &m_RendererID);
    }

    void Texture::SetData(const void* data)
    {
    	GLenum format = ImageFormatToOpenGLFormat(m_TextureDesc.ImageFormat);
    	GLenum type = ImageFormatToOpenGLType(m_TextureDesc.ImageFormat);

    	const GLsizei width = static_cast<GLsizei>(m_TextureDesc.ImageExtent.Width);
    	const GLsizei height = static_cast<GLsizei>(m_TextureDesc.ImageExtent.Height);
    	const GLsizei depth = static_cast<GLsizei>(m_TextureDesc.ImageExtent.Depth);

    	switch (m_TextureDesc.ImageType)
    	{
    		case ImageType::Image1D:
    			glTextureSubImage1D(m_RendererID, 0, 0, width, format, type, data);
    		break;
    		case ImageType::Image2D:
    			glTextureSubImage2D(m_RendererID, 0, 0, 0, width, height, format, type, data);
    		break;
    		case ImageType::Image3D:
    			glTextureSubImage3D(m_RendererID, 0, 0, 0, 0, width, height, depth,
					format, type, data);
    		break;
    		case ImageType::Cubemap:
    			for (int face = 0; face < 6; face++)
    			{
				    const auto dataPtr = (const size_t**)data;
    				const void* faceData = dataPtr[face];
    				glTextureSubImage3D(m_RendererID, 0, 0, 0, face, width, height, 1, format, type, faceData);
    			}
    		default:
    			break;
    	}

    	if (m_TextureDesc.GenerateMipmaps)
    		glGenerateTextureMipmap(m_RendererID);
    }

    void Texture::BindTextureUnit(uint32_t textureUnit)
    {
    	glBindTextureUnit(textureUnit, m_RendererID);
    }
}

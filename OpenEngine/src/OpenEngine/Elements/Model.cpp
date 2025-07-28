#include "ogpch.h"
#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace OpenGraphics
{
    Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices, const Material& material)
        : m_Vertices(vertices), m_Indices(indices), m_Material(material),
        m_RenderEntity(RenderEntity(vertices, indices))
    {
    }

    Mesh::Mesh(const Mesh &other)
        : m_Vertices(other.m_Vertices), m_Indices(other.m_Indices), m_Material(other.m_Material),
        m_RenderEntity(RenderEntity(other.m_Vertices, other.m_Indices))
    {
    }

    Mesh::~Mesh()
    {
        m_Vertices.clear();
        m_Indices.clear();
    }

    Model::Model(const std::string& path)
    {
        Assimp::Importer import;
        const aiScene* scene = import.ReadFile(path,
            aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace |
            aiProcess_RemoveRedundantMaterials);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            Logger::Error("ASSIMP: {}", import.GetErrorString());
            return;
        }

        LoadMaterials(scene);
        LoadMeshes(scene);
    }

    Model::~Model()
    {
        m_Meshes.clear();
    }

    void Model::LoadMeshes(const aiScene* scene)
    {
        m_Meshes.reserve(scene->mNumMeshes);
        for (size_t i = 0; i < scene->mNumMeshes; i++)
        {
            const aiMesh* mesh = scene->mMeshes[i];
            m_Meshes.push_back(ProcessMesh(mesh, scene));
        }
    }

    Mesh Model::ProcessMesh(const aiMesh* mesh, const aiScene* scene) const
    {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        for (int i = 0; i < mesh->mNumVertices; i++) {
            Vertex vertex;

            Vector3D position;
            position.x = mesh->mVertices[i].x;
            position.y = mesh->mVertices[i].y;
            position.z = mesh->mVertices[i].z;
            vertex.Position = position;

            Vector3D normal;
            normal.x = mesh->mNormals[i].x;
            normal.y = mesh->mNormals[i].y;
            normal.z = mesh->mNormals[i].z;
            vertex.Normal = normal;

            if (mesh->mTangents)
            {
                Vector3D tangent;
                tangent.x = mesh->mTangents[i].x;
                tangent.y = mesh->mTangents[i].y;
                tangent.z = mesh->mTangents[i].z;
                vertex.Tangent = tangent;

                Vector3D bitangent;
                bitangent.x = mesh->mBitangents[i].x;
                bitangent.y = mesh->mBitangents[i].y;
                bitangent.z = mesh->mBitangents[i].z;
                vertex.Bitangent = bitangent;
            }

            if (mesh->mTextureCoords[0])
            {
                Vector2D texCoord;
                texCoord.x = mesh->mTextureCoords[0][i].x;
                texCoord.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoord = texCoord;
            }

            vertices.push_back(vertex);
        }

        for (uint32_t i = 0; i < mesh->mNumFaces; i++)
        {
            const aiFace face = mesh->mFaces[i];
            for (uint32_t j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        const Material material = m_Materials[mesh->mMaterialIndex];
        return Mesh(vertices, indices, material);
    }

    void Model::LoadMaterials(const aiScene* scene)
    {
        m_Materials.reserve(scene->mNumMaterials);

        if (scene->mNumMaterials == 1)
        {
            const aiMaterial* material = scene->mMaterials[0];
            if (material->GetName() == aiString(AI_DEFAULT_MATERIAL_NAME))
            {
                m_Materials.push_back(ProcessDefaultMaterial(material, scene));
                return;
            }
        }

        for (size_t i = 0; i < scene->mNumMaterials; i++)
        {
            const aiMaterial* material = scene->mMaterials[i];
            if (material->GetName() == aiString(AI_DEFAULT_MATERIAL_NAME))
                continue;
            m_Materials.push_back(ProcessMaterial(material, scene));
        }
    }

    Material Model::ProcessMaterial(const aiMaterial* material, const aiScene* scene) const
    {
        Material modelMaterial{};
        modelMaterial.Name = material->GetName().C_Str();

        aiShadingMode shadingMode = aiShadingMode_Unlit;
        ai_int shadingModeInt = 0;
        material->Get(AI_MATKEY_SHADING_MODEL, shadingModeInt);
        shadingMode = static_cast<aiShadingMode>(shadingModeInt);

        switch (shadingMode)
        {
            case aiShadingMode_PBR_BRDF: {
                aiColor3D baseColor (0.f,0.f,0.f);
                float metallic, roughness;
                material->Get(AI_MATKEY_BASE_COLOR, baseColor);
                material->Get(AI_MATKEY_METALLIC_FACTOR, metallic);
                material->Get(AI_MATKEY_ROUGHNESS_FACTOR, roughness);
                modelMaterial.MainColor = Color(baseColor.r, baseColor.g, baseColor.b);
                modelMaterial.SetFloat("_Metallic", metallic);
                modelMaterial.SetFloat("_Roughness", roughness);
                break;
            }
            case aiShadingMode_Gouraud:
            case aiShadingMode_Phong:
            case aiShadingMode_Blinn: {
                aiColor3D diffuse{}, specular{}, emissive{};
                float shininess;
                material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
                material->Get(AI_MATKEY_COLOR_SPECULAR, specular);
                material->Get(AI_MATKEY_COLOR_EMISSIVE, emissive);
                material->Get(AI_MATKEY_SHININESS, shininess);
                modelMaterial.MainColor = Color(diffuse.r, diffuse.g, diffuse.b);
                modelMaterial.SetColor("_SpecularColor", Color(specular.r, specular.g, specular.b));
                modelMaterial.SetColor("_EmissiveColor", Color(emissive.r, emissive.g, emissive.b));
                modelMaterial.SetFloat("_Shininess", shininess);
                break;
            }
            case aiShadingMode_Unlit:
            default:
                aiColor3D matColor (0.f,0.f,0.f);
                material->Get(AI_MATKEY_COLOR_DIFFUSE, matColor);
                modelMaterial.MainColor = Color(matColor.r, matColor.g, matColor.b);
        }


        return modelMaterial;
    }

    Material Model::ProcessDefaultMaterial(const aiMaterial* material, const aiScene* scene) const
    {
        Material defaultMaterial{};
        defaultMaterial.Name = AI_DEFAULT_MATERIAL_NAME;
        aiShadingMode shadingMode = aiShadingMode_Unlit;
        material->Get(AI_MATKEY_SHADING_MODEL, shadingMode);

        switch (shadingMode)
        {
            case aiShadingMode_PBR_BRDF: {
                aiColor3D baseColor (0.f,0.f,0.f);
                material->Get(AI_MATKEY_BASE_COLOR, baseColor);
                defaultMaterial.MainColor = Color(baseColor.r, baseColor.g, baseColor.b);
                defaultMaterial.SetFloat("_Metallic", 0.0f);
                defaultMaterial.SetFloat("_Roughness", 0.5f);
                break;
            }
            case aiShadingMode_Gouraud:
            case aiShadingMode_Phong:
            case aiShadingMode_Blinn:
            case aiShadingMode_Unlit:
            default:
                aiColor3D diffuse (0.f,0.f,0.f);
                material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
                defaultMaterial.MainColor = Color(diffuse.r, diffuse.g, diffuse.b);
        }
        return defaultMaterial;
    }
}

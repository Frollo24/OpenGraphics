#include "ogpch.h"
#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace OpenGraphics
{
    Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices, const Material& material)
        : m_Vertices(vertices), m_Indices(indices), m_Material(material),
        m_RenderEntity(new RenderEntity(vertices, indices))
    {
    }

    Mesh::Mesh(const Mesh &other)
    {
        m_Vertices = other.m_Vertices;
        m_Indices = other.m_Indices;
        m_Material = other.m_Material;
        m_RenderEntity = new RenderEntity(m_Vertices, m_Indices);
    }

    Mesh::~Mesh()
    {
        delete m_RenderEntity;
        m_Vertices.clear();
        m_Indices.clear();
    }

    Model::Model(const std::string &path)
    {
        Assimp::Importer import;
        const aiScene* scene = import.ReadFile(path,
            aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);

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

    Mesh Model::ProcessMesh(const aiMesh *mesh, const aiScene *scene)
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

    void Model::LoadMaterials(const aiScene *scene)
    {
        m_Materials.reserve(scene->mNumMaterials);
        for (size_t i = 0; i < scene->mNumMaterials; i++)
        {
            const aiMaterial* material = scene->mMaterials[i];
            m_Materials.push_back(ProcessMaterial(material, scene));
        }
    }

    Material Model::ProcessMaterial(const aiMaterial *material, const aiScene *scene)
    {
        // TODO: load more material properties into the material
        aiColor3D diffuse (0.f,0.f,0.f);
        material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);

        Material modelMaterial{};
        modelMaterial.MainColor = Color(diffuse.r, diffuse.g, diffuse.b);

        return modelMaterial;
    }
}

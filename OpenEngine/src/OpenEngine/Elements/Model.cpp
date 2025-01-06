#include "ogpch.h"
#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace OpenGraphics
{
    Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices)
        : m_Vertices(vertices), m_Indices(indices), m_RenderEntity(new RenderEntity(vertices, indices))
    {
    }

    Mesh::Mesh(const Mesh &other)
    {
        m_Vertices = other.m_Vertices;
        m_Indices = other.m_Indices;
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
        const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            Logger::Error("ASSIMP: {}", import.GetErrorString());
            return;
        }

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
            aiMesh* mesh = scene->mMeshes[i];
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

            vertex.Color = Vector3D(0.6f, 0.6f, 0.6f);

            vertices.push_back(vertex);
        }

        for (uint32_t i = 0; i < mesh->mNumFaces; i++)
        {
            const aiFace face = mesh->mFaces[i];
            for (uint32_t j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        // TODO: load materials and convert them to engine materials
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        return Mesh(vertices, indices);
    }
}

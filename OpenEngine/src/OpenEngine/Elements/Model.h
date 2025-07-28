#pragma once

#include "OpenEngine/Core/Base.h"
#include "OpenEngine/Elements/Material.h"
#include "OpenEngine/Render/RenderEntity.h"

#include <assimp/scene.h>

namespace OpenGraphics
{
    class OPEN_API Mesh
    {
    public:
        Mesh(const std::vector<Vertex>& vertices, const std::vector<Index>& indices, const Material& material);
        Mesh(const Mesh& other);
        ~Mesh();

        [[nodiscard]] inline const std::vector<Vertex>& GetVertices() const { return m_Vertices; }
        [[nodiscard]] inline const std::vector<Index>& GetIndices() const { return m_Indices; }
        [[nodiscard]] inline const Material& GetMaterial() const { return m_Material; }
        [[nodiscard]] inline const RenderEntity& GetRenderEntity() const { return m_RenderEntity; }

        inline void Render() const { m_RenderEntity.Render(); }

    private:
        std::vector<Vertex> m_Vertices;
        std::vector<Index> m_Indices;
        Material m_Material;
        RenderEntity m_RenderEntity;
    };

    class OPEN_API Model
    {
    public:
        Model(const std::string& path);
        ~Model();

        [[nodiscard]] inline const std::vector<Mesh>& GetMeshes() const { return m_Meshes; }
        [[nodiscard]] inline const std::vector<Material>& GetMaterials() const { return m_Materials; }

    private:
        void LoadMeshes(const aiScene* scene);
        Mesh ProcessMesh(const aiMesh* mesh, const aiScene* scene) const;

        void LoadMaterials(const aiScene* scene);
        Material ProcessMaterial(const aiMaterial* material, const aiScene* scene) const;
        Material ProcessDefaultMaterial(const aiMaterial* material, const aiScene* scene) const;

    private:
        std::vector<Mesh> m_Meshes;
        std::vector<Material> m_Materials;
    };
}

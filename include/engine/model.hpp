#pragma once
#include <string>
#include <vector>
#include <memory>
// #include <iostream> // Removed for std::cout

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Logger.hpp>
#include <assimp/DefaultLogger.hpp>

#include <engine/ecs/entity.hpp>
#include <engine/mesh.hpp>

#include <engine/components/meshfilter.hpp>
#include <engine/components/meshrenderer.hpp>
#include <engine//components/material.hpp>

#include <engine/texture2D.hpp>
#include <engine/scene.hpp>

class Model
{
public:
    static Entity *Load(const std::string &_path, Scene *_scene)
    {
        // Create an instance of the Assimp logger
        Assimp::DefaultLogger::create("", Assimp::Logger::VERBOSE);

        Assimp::Importer importer;
        const aiScene *aiScene = importer.ReadFile(
            _path,
            aiProcess_Triangulate |
                aiProcess_GenNormals |
                aiProcess_CalcTangentSpace |
                aiProcess_JoinIdenticalVertices |
                aiProcess_ImproveCacheLocality);

        if (!aiScene || !aiScene->mRootNode)
        {
            throw std::runtime_error("Assimp Failed to load model: " + _path + " (" + importer.GetErrorString() + ")");
        }

        const std::string directory = _path.substr(0, _path.find_last_of("/\\"));
        auto root = ProcessNode(aiScene->mRootNode, aiScene, _scene, directory);

        // Kill the logger when we're done with it
        Assimp::DefaultLogger::kill();

        return root;
    }

private:
    static Entity *ProcessNode(aiNode *node, const aiScene *scene, Scene *_scene, const std::string &directory)
    {
        auto entity = _scene->CreateEntity(node->mName.C_Str());

        ApplyTransform(node->mTransformation, entity);

        // Process all meshes of this node
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh *aiMesh = scene->mMeshes[node->mMeshes[i]];

            // std::cout << "Processing Mesh: " << aiMesh->mName.C_Str() << std::endl; // Debug log
            // std::cout << "  Num Vertices: " << aiMesh->mNumVertices << std::endl;   // Debug log
            // std::cout << "  Num Faces: " << aiMesh->mNumFaces << std::endl;         // Debug log

            auto m_filter = entity->AddComponent<MeshFilter>();
            m_filter->mesh = ProcessMesh(aiMesh, scene, directory);
            entity->AddComponent<Material>();

            // std::cout << "  Num Indices after processing: " << m_filter->mesh->GetIndices().size() << std::endl; // Debug log

            entity->AddComponent<MeshRenderer>();
        }

        // Recursively process children
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            auto childEntity = ProcessNode(node->mChildren[i], scene, _scene, directory);
            childEntity->transform.parent = entity->GetID();
            entity->transform.children.push_back(childEntity->GetID());
        }

        return entity;
    }

    static void ApplyTransform(const aiMatrix4x4 &m, Entity *entity)
    {
        aiVector3D pos, scale;
        aiQuaternion rot;
        m.Decompose(scale, rot, pos);

        entity->transform.position = {pos.x, pos.y, pos.z};
        entity->transform.scale = {scale.x, scale.y, scale.z};
        entity->transform.rotation = quat(rot.x, rot.y, rot.z, rot.w);
    }

    static std::unique_ptr<Mesh> ProcessMesh(aiMesh *mesh, const aiScene *scene, const std::string &directory)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<std::shared_ptr<Texture2D>> textures;

        // Vertices
        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex v{};
            v.position = {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};

            if (mesh->HasNormals())
                v.normal = {mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z};

            if (mesh->mTextureCoords[0])
                v.uv = {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y};
            else
                v.uv = {0.0f, 0.0f};

            vertices.push_back(v);
        }

        // Indices
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            const aiFace &face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        // Materials
        if (mesh->mMaterialIndex >= 0)
        {
            aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

            AppendMaterialTextures(textures, material, aiTextureType_DIFFUSE, Type::DIFFUSE, directory);
            AppendMaterialTextures(textures, material, aiTextureType_SPECULAR, Type::SPECULAR, directory);
        }

        return std::make_unique<Mesh>(vertices, indices, textures);
    }

    static void AppendMaterialTextures(
        std::vector<std::shared_ptr<Texture2D>> &out,
        aiMaterial *material,
        aiTextureType type,
        Type engineType,
        const std::string &directory)
    {
        for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
        {
            aiString str;
            material->GetTexture(type, i, &str);

            out.push_back(std::make_shared<Texture2D>(directory + "/" + str.C_Str(), engineType));
        }
    }
};

#include <assimp/material.h>
#include <assimp/mesh.h>
#include <iostream>
#include "mesh.h"
#include "glad/glad.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


//TODO: Optimize to avoid loading the same texture multiple times.
static void process_node(Model *model, aiNode *node, const aiScene *scene, std::vector<Mesh>& meshes);
static Mesh process_mesh(Model *model, aiMesh *mesh, const aiScene *scene);
static std::vector<Texture> load_material_textures(aiMaterial *material,
                                                   aiTextureType type,
                                                   const char *type_name);

void init_mesh(Mesh *mesh,
               std::vector<Vertex> && vertices,
               std::vector<unsigned int> && indices,
               std::vector<Texture> && textures){


    assert(mesh != NULL);
    glGenVertexArrays(1, &mesh->vao);
    glGenBuffers(1,      &mesh->vbo);
    glGenBuffers(1,      &mesh->ebo);

    glBindVertexArray(mesh->vao);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);  

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);	
    // vertex normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);	
    // vertex texture coords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv_coords));
    glEnableVertexAttribArray(2);	

    glBindVertexArray(0);

    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR){
        std::cerr << "Gl error: " << err << "\n";
    }

    mesh->vertices = std::move(vertices);
    mesh->indices = std::move(indices);
    mesh->textures = std::move(textures);
}

static void mesh_draw(Mesh *mesh, const Shader& shader){
    assert(mesh != NULL);
    uint32_t diffuse_n = 1;
    uint32_t specular_n = 1;

    for(uint32_t i = 0; i < mesh->textures.size(); i++){
        glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
        std::string type, number;
        type = mesh->textures[i].type;
        if(type == "texture_diffuse"){
            number = std::to_string(diffuse_n++);
        }else if(type == "texture_specular"){
            number = std::to_string(specular_n++);
        }

        auto sampler_id = "material." + type + number;
        shader.set_int(sampler_id, i);
        glBindTexture(GL_TEXTURE_2D, mesh->textures[i].id);
    };
    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(mesh->vao);
    //glDrawArrays(GL_TRIANGLES, 0, mesh->vertices.size());
    glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR){
        std::cerr << "Gl error: " << err << "\n";
    }
};

void model_draw(Model *model, const Shader& shader){
    assert(model != NULL);
    for(size_t i = 0; i < model->meshes.size(); i++){
        mesh_draw(&model->meshes[i], shader);
    }
};

bool load_model(Model* model, const char* filename){
    assert(model != NULL);
    Assimp::Importer importer;
    uint32_t importer_flags = aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_FlipUVs;
    const aiScene *scene = importer.ReadFile(filename, importer_flags);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
        std::cerr << "Error: " << importer.GetErrorString() << "\n";
        return false;
    }

    int end = strlen(filename) - 1;
    while(end >= 0 && filename[end] != '/') end--;

    model->directory = end < 0 ? "." : std::string(filename, filename + end);
    process_node(model, scene->mRootNode, scene, model->meshes);

    return true;
};

static void process_node(Model *model, aiNode *node, const aiScene *scene, std::vector<Mesh>& meshes){
    assert(node != NULL);
    assert(scene != NULL);

    aiMesh *curr_mesh;

    for(size_t i = 0; i < node->mNumMeshes; i++){
        uint32_t mesh_index = node->mMeshes[i];
        curr_mesh = scene->mMeshes[mesh_index];
        meshes.push_back(process_mesh(model, curr_mesh, scene));
    }

    for(size_t i = 0; i < node->mNumChildren; i++){
        process_node(model, node->mChildren[i], scene, meshes);
    }
};

static Mesh process_mesh(Model *model, aiMesh *mesh, const aiScene *scene){
    assert(model != NULL);
    std::vector<Vertex> model_vertices;
    std::vector<uint32_t> model_indices;
    std::vector<Texture> model_textures;
    Mesh _mesh;
    model_vertices.resize(mesh->mNumVertices);

    for(uint32_t i = 0; i < mesh->mNumVertices; i++){
        Vertex& vertex = model_vertices[i];
        vertex.uv_coords = glm::vec2(0.0);
        vertex.normal = glm::vec3(0.0);

        vertex.position.x = mesh->mVertices[i].x;
        vertex.position.y = mesh->mVertices[i].y;
        vertex.position.z = mesh->mVertices[i].z;

        if(mesh->mNormals){
            vertex.normal.x = mesh->mNormals[i].x;
            vertex.normal.y = mesh->mNormals[i].y;
            vertex.normal.z = mesh->mNormals[i].z;
        }

        // assimp has up to eight different texture coordinates channel
        // we only care about the first one
        if(mesh->mTextureCoords[0]){
            vertex.uv_coords.x = mesh->mTextureCoords[0][i].x;
            vertex.uv_coords.y = mesh->mTextureCoords[0][i].y;
        }                
    };

    for(uint32_t i = 0; i < mesh->mNumFaces; i++){
        aiFace& face = mesh->mFaces[i];
        for(int j = 0; j < face.mNumIndices; j++){
            model_indices.push_back(face.mIndices[j]);
        }
    }

    if(mesh->mMaterialIndex >= 0){
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        auto diffuse_maps = load_material_textures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        auto specular_maps = load_material_textures(material, aiTextureType_DIFFUSE, "texture_specular");
        model_textures.insert(model_textures.end(), diffuse_maps.begin(), diffuse_maps.end());
        model_textures.insert(model_textures.end(), specular_maps.begin(), specular_maps.end());
    }

    init_mesh(&_mesh, std::move(model_vertices), std::move(model_indices), std::move(model_textures));
    return _mesh;
};

static std::vector<Texture> load_material_textures(aiMaterial *material,
                                                   aiTextureType type,
                                                   const char *type_name){
    size_t textures_num = material->GetTextureCount(type);

    
    std::vector<Texture> textures;
    for(size_t i = 0; i < textures_num; i++){
        Texture texture;
        aiString path;

        material->GetTexture(type, i, &path);
        if(!load_texture_make_id(path.C_Str(), &texture.id)){
            std::cerr << "Failed to load texture " << path.C_Str() << "\n";
            continue;
        }

        texture.type = type_name;
        texture.path = path.C_Str();

        textures.push_back(texture);
    }
    return textures;
}

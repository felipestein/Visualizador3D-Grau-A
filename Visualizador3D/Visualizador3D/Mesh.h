#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"

#include <string>
#include <vector>
using namespace std;

#define MAX_BONE_INFLUENCE 4

// armazenando os atributos do vértice 
struct Vertex {
    glm::vec3 Position; // position
    glm::vec3 Normal; // normal
    glm::vec2 TexCoords; // texCoords
    glm::vec3 Tangent;// tangent 
    glm::vec3 Bitangent; // bitangent 

    int m_BoneIDs[MAX_BONE_INFLUENCE];
    float m_Weights[MAX_BONE_INFLUENCE];
};

// organizando os dados da textura
struct Texture {
    unsigned int id; // id da textura
    string type; // difusa ou especular
    string path; // caminho
};

class Mesh {
public:
    // dados da malha
    vector<Vertex>       vertices;
    vector<unsigned int> indices;
    vector<Texture>      textures;
    unsigned int VAO;

    // constructor // dados necessários para iniciar a malha
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;

        // com os dados necessários, definir os buffers de vértice e seus ponteiros de atributo.
        setupMesh(); // inicializamos os buffers
    }

    // renderizando a malha
    void Draw(Shader& shader) // desenhar a malha
    {
        // vinculando texturas apropriadas
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr = 1;
        unsigned int heightNr = 1;
        for (unsigned int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); // ativando a textura antes de vincular

            // recuperando o número da textura // number retrieve 
            string number;
            string name = textures[i].type;
            if (name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if (name == "texture_specular")
                number = std::to_string(specularNr++); // transferindo unsigned int para string
            else if (name == "texture_normal")
                number = std::to_string(normalNr++); // transferindo unsigned int para string
            else if (name == "texture_height")
                number = std::to_string(heightNr++); // transferindo unsigned int para string

            // definindo a textura correta
            glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);

            // vincular a textura
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }

        // desenhando malha
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glActiveTexture(GL_TEXTURE0);
    }

private:
    // dados do render
    unsigned int VBO, EBO;

    // inicia todos os buffers objetos/arrays
    void setupMesh()
    {
        // criando buffers/arrays
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        // carregando dados no vertex buffers
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        // passando um ponteiro para o struct e ele transfere perfeitamente para um array
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // definindo os ponteiros de atributo de vértice
        // vertex Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        
        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        
        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
        
        // vertex tangent
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
        
        // vertex bitangent
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
        
        // ids
        glEnableVertexAttribArray(5);
        glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));

        // weights
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));
        glBindVertexArray(0);
    }
};
#endif


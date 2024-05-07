#include "Toroide.h"
//Toroide correspondiente a la alumna: Lourdes Victoria Genis Cruz
Toroide::Toroide(float majorRadius, float minorRadius, int majorSlices, int minorSlices) :
    majorRadius(majorRadius), minorRadius(minorRadius), majorSlices(majorSlices), minorSlices(minorSlices),
    VAO(0), VBO(0), EBO(0) {
}

Toroide::~Toroide() {
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &VBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDeleteBuffers(1, &EBO);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VAO);
}

void Toroide::init() {
    vertices.resize(((majorSlices + 1) * (minorSlices + 1)));
    indices.resize((majorSlices * minorSlices + majorSlices) * 6);
    for (int i = 0; i <= minorSlices; ++i) {
        float V = i / (float)minorSlices;
        float phi = V * 2.0f * M_PI;

        for (int j = 0; j <= majorSlices; ++j) {
            float U = j / (float)majorSlices;
            float theta = U * 2.0f * M_PI;

            float X = (majorRadius + minorRadius * cos(phi)) * cos(theta);
            float Y = (majorRadius + minorRadius * cos(phi)) * sin(theta);
            float Z = minorRadius * sin(phi);
            vertices[i * (majorSlices + 1) + j].position = glm::vec3(X, Y, Z);
            vertices[i * (majorSlices + 1) + j].color = glm::sphericalRand(1.0);
        }
    }

    //calculateNormals(); // Calcular normales
    calcAverageNormals(indices.data(), indices.size(), reinterpret_cast<GLfloat*>(vertices.data()), vertices.size(),
        sizeof(VertexColor) / sizeof(GLfloat), offsetof(VertexColor, normal));

    for (int i = 0; i < majorSlices * minorSlices + majorSlices; ++i) {
        indices[i * 6] = i;
        indices[i * 6 + 1] = i + majorSlices + 1;
        indices[i * 6 + 2] = i + majorSlices;
        indices[i * 6 + 3] = i + majorSlices + 1;
        indices[i * 6 + 4] = i;
        indices[i * 6 + 5] = i + 1;
    }
}


void Toroide::load() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    size_t stride = sizeof(VertexColor);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * stride,
        vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
        indices.data(), GL_STATIC_DRAW);

    // First attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offsetof(VertexColor, position));
    glEnableVertexAttribArray(0);
    // Second attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offsetof(VertexColor, color));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0); // Unbind VAO
}

void Toroide::render() {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Toroide::calculateNormals() {
    for (int i = 0; i <= minorSlices; ++i) {
        for (int j = 0; j <= majorSlices; ++j) {
            glm::vec3 normal(0.0f); // Inicializar la normal a cero

            // Obtener la posición del vértice actual
            glm::vec3 position = vertices[i * (majorSlices + 1) + j].position;

            // Calcular la dirección del vector normal para un toroide
            glm::vec3 normalDirection = glm::normalize(position);

            // Calcular las componentes X, Y y Z de la normal
            float normalX = normalDirection.x;
            float normalY = normalDirection.y;
            float normalZ = normalDirection.z;

            // Asignar la normal al vértice
            vertices[i * (majorSlices + 1) + j].normal = glm::vec3(normalX, normalY, normalZ);
        }
    }
}


void Toroide::calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
    unsigned int vLength, unsigned int normalOffset)
{
    for (size_t i = 0; i < indiceCount; i += 3)
    {
        unsigned int in0 = indices[i] * vLength;
        unsigned int in1 = indices[i + 1] * vLength;
        unsigned int in2 = indices[i + 2] * vLength;
        glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
        glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
        glm::vec3 normal = glm::cross(v1, v2);
        normal = glm::normalize(normal);

        in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
        vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
        vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
        vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
    }

    for (size_t i = 0; i < verticeCount / vLength; i++)
    {
        unsigned int nOffset = i * vLength + normalOffset;
        glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
        vec = glm::normalize(vec);
        vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
    }
}

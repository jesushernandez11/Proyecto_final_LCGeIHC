#pragma once
#ifndef TOROIDE_H_
#define TOROIDE_H_
//Toroide correspondiente a la alumna: Lourdes Victoria Genis Cruz
#include <vector>

#define _USE_MATH_DEFINES
#include <math.h>
#include "definition.h"

class Toroide {
public:
    Toroide(float majorRadius, float minorRadius, int majorSlices, int minorSlices);
    void init();
    void load();
    void render();
    void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
        unsigned int vLength, unsigned int normalOffset);
    void calculateNormals();
    virtual ~Toroide();
private:
    std::vector<VertexColor> vertices;
    std::vector<GLuint> indices;
    float majorRadius;
    float minorRadius;
    int majorSlices;
    int minorSlices;

    GLuint VAO, VBO, EBO;
};

#endif /* TOROIDE_H_ */

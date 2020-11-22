/**
 * ------------------------------------
 * @author: Weipeng Kong
 * @date: 2020/11/20
 * @email: yjxkwp@foxmail.com
 * @site: https://x86.design
 * @description: 
 * ------------------------------------
**/

#ifndef CADEULEROPERATION_MODELPAINTER_H
#define CADEULEROPERATION_MODELPAINTER_H

#include "B-rep.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <GLUT/glut.h>

struct TessData {
    GLdouble data[6];

    TessData(glm::vec3 point, glm::vec3 color) {
        data[0] = point.x;
        data[1] = point.y;
        data[2] = point.z;
        data[3] = color.r;
        data[4] = color.g;
        data[5] = color.b;
    }
};

namespace ModelPainter {
    enum Type{
        Solid,
        Wireframe
    };

    static Type drawingType;

    void init();

    void end();

    /**
     * Draw a list of solids
     * @param solid
     */
    void draw(const solid_ptr &solid);

    void drawPoint();

    void drawSolidLoop(const loop_ptr &loop);

    void drawSolidFace(const face_ptr &face);

    void drawWireFrameFace(const face_ptr &face);

    void drawWireFrameLoop(const loop_ptr &loop);

    /**
     * Draw one solid
     * @param solid
     */
    void drawSolid(const solid_ptr &solid);

    void test();
}

#endif //CADEULEROPERATION_MODELPAINTER_H

/**
 * ------------------------------------
 * @author: Weipeng Kong
 * @date: 2020/11/20
 * @email: yjxkwp@foxmail.com
 * @site: https://x86.design
 * @description: 
 * ------------------------------------
**/

#include "ModelPainter.h"

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <GLUT/GLUT.h>


// Global Data Area
GLFWwindow *window; // (In the accompanying source code, this variable is global for simplicity)
const int screen_x = 1280;
const int screen_y = 720;
float z_angle = 0;
float y_angle = 0;
bool keys[1024] = {false};

void CALLBACK tessBeginCB(GLenum which) {
    glBegin(which);
}

void CALLBACK tessEndCB() {
    glEnd();
}

void CALLBACK tessVertexCB(const GLvoid *data) {
    // cast back to double type
    const GLdouble *ptr = (const GLdouble *) data;
    glColor3dv(ptr + 3);
    glVertex3dv(ptr);
}

void keyboard_cb(GLFWwindow *window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;
    }
    if (action == GLFW_PRESS){
        if(key == GLFW_KEY_W){
            ModelPainter::drawingType = ModelPainter::Wireframe;
        }else if(key == GLFW_KEY_S){
            ModelPainter::drawingType = ModelPainter::Solid;
        }
    }

}

void movement(float deltaTime) {
    deltaTime *= 200;
    // Camera controls
    if (keys[GLFW_KEY_UP])
        y_angle += 1.0f * deltaTime;
    if (keys[GLFW_KEY_DOWN])
        y_angle -= 1.0f * deltaTime;
    if (keys[GLFW_KEY_LEFT])
        z_angle += 1.0f * deltaTime;
    if (keys[GLFW_KEY_RIGHT])
        z_angle -= 1.0f * deltaTime;

}

void ModelPainter::init() {
    if (!glfwInit()) {
        fprintf(stderr, "Fail to initialize GLFW\n");
        exit(1);
    }
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(screen_x, screen_y, "Ruler Operation Display", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr,
                "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        glfwTerminate();
        exit(1);
    }
    glfwMakeContextCurrent(window); // Initialize GLEW
    glewExperimental = true; // Needed in core profile

    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        exit(1);
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    glfwSetKeyCallback(window, keyboard_cb);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);

    // Draw Backgroud Color
    glClearColor(0.1, 0.1, 0.1, 1.0);
}

void ModelPainter::end() {
    glfwTerminate();
}

void ModelPainter::draw(const solid_ptr &solid) {
    GLfloat lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        GLfloat newTime = glfwGetTime();
        movement(newTime - lastTime);
        lastTime = newTime;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
        glViewport(10, 10, screen_x * 2, screen_y * 2);

        // Perspective
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        gluPerspective(45.0f, screen_x / 1.0 / screen_y, 0.1f, 100.0f);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity(); // Reset the model-view matrix
        glTranslatef(0.0f, 0.0f, -10.0f);

        glRotatef(z_angle, 0.0f, 1.0f, 0.0f);
        glRotatef(y_angle, 1.0f, 0.0f, 1.0f);

        glRotatef(180.0, 0.0f, 1.0f, 0.0f);
        glRotatef(90.0, 1.0f, 0.0f, 0.0f);

        // Start drawing
        solid_ptr p = solid;
        while (p) {
            drawSolid(p);
            p = p->s_next;
        }

        // End drawing
        glfwSwapBuffers(window);
    }
}

void ModelPainter::drawSolid(const solid_ptr &solid) {
    face_ptr f = solid->s_faces;
    while(f){
        if(drawingType == Solid){
            drawSolidFace(f);
        }else if(drawingType == Wireframe){
            drawWireFrameFace(f);
        }else{
            throw std::runtime_error("Unrecognized Drawing Type.");
        }
        f = f->f_next;
    }
}

void ModelPainter::drawSolidFace(const face_ptr &face) {
    glEnable(GL_STENCIL_TEST);
    glClearStencil(0);
    glClear(GL_STENCIL_BUFFER_BIT);

    // ------------ Draw inner loop ------------
    // Write 1's into stencil buffer where the hole will be
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);
    glStencilFunc(GL_ALWAYS, 1, ~0);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    for(loop_ptr l = face->f_loop; l != nullptr; l = l->l_next){
        if(l->inner){
            drawSolidLoop(l);
        }
    }

    // ------------ Draw outer loop ------------
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    glStencilFunc(GL_EQUAL, 0, ~0);
    glDepthMask(GL_TRUE);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    for(loop_ptr l = face->f_loop; l != nullptr; l = l->l_next){
        if(!l->inner){
            drawSolidLoop(l);
        }
    }

    glDisable(GL_STENCIL_TEST);
}


void ModelPainter::drawSolidLoop(const loop_ptr &loop) {
    GLuint id = glGenLists(1); // create a display list
    if (!id)
        return; // failed to create a list, return 0

    GLUtesselator *tess = gluNewTess(); // create a tessellator
    if (!tess)
        return; // failed to create tessellation object, return 0

    // register callback functions
    gluTessCallback(tess, GLU_TESS_BEGIN, (void(CALLBACK *)())tessBeginCB);
    gluTessCallback(tess, GLU_TESS_END, (void(CALLBACK *)())tessEndCB);
    gluTessCallback(tess, GLU_TESS_VERTEX, (void(CALLBACK *)())tessVertexCB);

    glNewList(id, GL_COMPILE);
    glColor3f(1, 1, 1);
    gluTessBeginPolygon(tess, 0); // with NULL data
    gluTessBeginContour(tess);
    halfedge_ptr startHe = loop->l_edge;
    halfedge_ptr he = loop->l_edge;
    vertex_ptr startV = he->start_v;
    std::vector<TessData> quadList;
    do
    {
        glm::vec3 point = he->start_v->v_coordinate;
        glm::vec3 color = loop->l_face->color;
        TessData data(point, color);
        quadList.push_back(data);
        he = he->he_next;
    } while (nullptr != he && he != startHe);
    for (int i = 0; i < quadList.size(); i++)
    {
        gluTessVertex(tess, quadList[i].data, quadList[i].data);
    }

    gluTessEndContour(tess);
    gluTessEndPolygon(tess);
    glEndList();

    gluDeleteTess(tess); // delete after tessellation

    glCallList(id);
}


void ModelPainter::drawWireFrameFace(const face_ptr &face){
    loop_ptr loop = face->f_loop;
    while(loop != nullptr){
        drawWireFrameLoop(loop);
        loop = loop->l_next;
    }
}

void ModelPainter::drawWireFrameLoop(const loop_ptr &loop){
    halfedge_ptr he = loop->l_edge;
    glBegin(GL_LINE_LOOP);

    do{
        if(he == nullptr) break;
        glColor3f(1, 1, 1);
        const vec3 &p = he->start_v->v_coordinate;
        glVertex3f(p.x, p.y, p.z);

        he = he->he_next;
    }while(he != loop->l_edge);

    glEnd();
}

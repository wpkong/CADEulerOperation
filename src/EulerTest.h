/**
 * ------------------------------------
 * @author: Weipeng Kong
 * @date: 2020/11/22
 * @email: yjxkwp@foxmail.com
 * @site: https://x86.design
 * @description: 
 * ------------------------------------
**/

#ifndef CADEULEROPERATION_EULERTEST_H
#define CADEULEROPERATION_EULERTEST_H

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <GLUT/glut.h>

using namespace glm;
using std::vector;
using std::cout;
using std::endl;

#include "EulerOperation.h"


solid_ptr create_block() {
    solid_ptr solid;
    vertex_ptr v1, v2, v3, v4;
    solid = EulerOp::mvfs(v1, vec3(-2.0, -2.0, 0));
    face_ptr face = solid->s_faces;
    loop_ptr loop = face->f_loop;

    halfedge_ptr e1 = EulerOp::mev(v1, vec3(2.0, -2.0, 0), loop);
    v2 = e1->couple->start_v;
    halfedge_ptr e2 = EulerOp::mev(v2, vec3(2.0, 2.0, 0), loop);
    v3 = e2->couple->start_v;
    halfedge_ptr e3 = EulerOp::mev(v3, vec3(-2.0, 2.0, 0), loop);
    v4 = e3->couple->start_v;
    loop_ptr inner_loop = EulerOp::mef(v1, v4, loop);

    EulerOp::sweep(face, vec3(0.0f, 0.0f, 1.0f), 1.0f);
    return solid;
}

solid_ptr create_A(const vec3 &offset = vec3(0, 0, 0), float scalar = 1) {
    solid_ptr solid;
    vec3 p[11];
    {
        p[0] = vec3(0, 0, 0);
        p[1] = vec3(2, 0, 0);
        p[2] = vec3(3, 0, 2);
        p[3] = vec3(5, 0, 2);
        p[4] = vec3(6, 0, 0);
        p[5] = vec3(8, 0, 0);
        p[6] = vec3(5, 0, 7);
        p[7] = vec3(3, 0, 7);
        p[8] = vec3(3, 0, 3);
        p[9] = vec3(5, 0, 3);
        p[10] = vec3(4, 0, 5);
    }
    for (int i = 0; i < 11; ++i) {
        p[i] += offset;
        p[i] *= scalar;
    }
    vertex_ptr v[11];

    solid = EulerOp::mvfs(v[0], p[0]);
    face_ptr face_bottom = solid->s_faces;
    loop_ptr loop_bottom = face_bottom->f_loop;

    for (int i = 0; i < 7; ++i) {
        halfedge_ptr e = EulerOp::mev(v[i], p[i + 1], loop_bottom);
        v[i + 1] = e->couple->start_v;
    }
    loop_ptr loop_top = EulerOp::mef(v[0], v[7], loop_bottom);

    EulerOp::sweep(loop_top, vec3(0.0f, 1.0f, 0.0f), 1.0f);

    halfedge_ptr bridge = EulerOp::mev(v[2], p[8], loop_bottom);
    v[8] = bridge->couple->start_v;

    for (int i = 8; i < 10; ++i) {
        halfedge_ptr e = EulerOp::mev(v[i], p[i + 1], loop_bottom);
        v[i + 1] = e->couple->start_v;
    }

    loop_ptr loop_tri_top = EulerOp::mef(v[10], v[8], loop_bottom);

    loop_ptr loop_tri_bottom = EulerOp::kemr(bridge->edge, loop_bottom);

    EulerOp::sweep(loop_tri_top, vec3(0.0f, 1.0f, 0.0f), 1.0f);

    loop_tri_top = solid->last_face()->f_loop;
    EulerOp::kfmrh(loop_tri_top, loop_top);

    loop_tri_bottom->inner = true;
    loop_tri_top->inner = true;

    cout << "loop_tri_bottom " << loop_tri_bottom << endl;
    cout << "loop_tri_top " << loop_tri_top << endl;

//    for (face_ptr f = solid->s_faces; f != nullptr; f = f->f_next) {
//        cout << "f: " << f << endl;
//        for (loop_ptr lp = f->f_loop; lp != nullptr; lp = lp->l_next) {
//            cout << "\tl(" << lp << "): ";
//            lp->debug();
//        }
//        cout << "-------" << endl;
//    }

    return solid;
}

solid_ptr create_C(const vec3 &offset = vec3(0, 0, 0), float scalar = 1) {
    solid_ptr solid;
    vec3 p[16];
    {
        p[0] = vec3(2, 0, 0);
        p[1] = vec3(4, 0, 0);
        p[2] = vec3(6, 0, 2);
        p[3] = vec3(6, 0, 3);
        p[4] = vec3(4, 0, 3);
        p[5] = vec3(4, 0, 2);
        p[6] = vec3(2, 0, 2);
        p[7] = vec3(2, 0, 5);
        p[8] = vec3(4, 0, 5);
        p[9] = vec3(4, 0, 4);
        p[10] = vec3(6, 0, 4);
        p[11] = vec3(6, 0, 5);
        p[12] = vec3(4, 0, 7);
        p[13] = vec3(2, 0, 7);
        p[14] = vec3(0, 0, 5);
        p[15] = vec3(0, 0, 2);
    }
    for (int i = 0; i < 16; ++i) {
        p[i] += offset;
        p[i] *= scalar;
    }
    vertex_ptr v[16];

    solid = EulerOp::mvfs(v[0], p[0]);
    face_ptr face_bottom = solid->s_faces;
    loop_ptr loop_bottom_outer = face_bottom->f_loop;

    for (int i = 0; i < 15; ++i) {
        halfedge_ptr e = EulerOp::mev(v[i], p[i + 1], loop_bottom_outer);
        v[i + 1] = e->couple->start_v;
    }
    loop_ptr loop = EulerOp::mef(v[0], v[15], loop_bottom_outer);
    EulerOp::sweep(face_bottom, vec3(0.0f, 1.0f, 0.0f), 1.0f);

    return solid;
}

solid_ptr create_D(const vec3 &offset = vec3(0, 0, 0), float scalar = 1) {
    solid_ptr solid;
    vec3 p[14];
    {
        p[0] = vec3(0, 0, 0);
        p[1] = vec3(3, 0, 0);
        p[2] = vec3(5, 0, 1);
        p[3] = vec3(6, 0, 3);
        p[4] = vec3(6, 0, 4);
        p[5] = vec3(5, 0, 6);
        p[6] = vec3(3, 0, 7);
        p[7] = vec3(0, 0, 7);
        p[8] = vec3(2, 0, 2);
        p[9] = vec3(3, 0, 2);
        p[10] = vec3(4, 0, 3);
        p[11] = vec3(4, 0, 4);
        p[12] = vec3(3, 0, 5);
        p[13] = vec3(2, 0, 5);
    }
    for (int i = 0; i < 14; ++i) {
        p[i] += offset;
        p[i] *= scalar;
    }
    vertex_ptr v[14];

    solid = EulerOp::mvfs(v[0], p[0]);
    face_ptr face_bottom = solid->s_faces;
    loop_ptr loop_bottom = face_bottom->f_loop;

    for (int i = 0; i < 7; ++i) {
        halfedge_ptr e = EulerOp::mev(v[i], p[i + 1], loop_bottom);
        v[i + 1] = e->couple->start_v;
    }
    loop_ptr loop_top = EulerOp::mef(v[0], v[7], loop_bottom);

    EulerOp::sweep(loop_top, vec3(0.0f, 1.0f, 0.0f), 1.0f);

    halfedge_ptr bridge = EulerOp::mev(v[2], p[8], loop_bottom);
    v[8] = bridge->couple->start_v;

    for (int i = 8; i < 13; ++i) {
        halfedge_ptr e = EulerOp::mev(v[i], p[i + 1], loop_bottom);
        v[i + 1] = e->couple->start_v;
    }

    loop_ptr loop_tri_top = EulerOp::mef(v[13], v[8], loop_bottom);

    loop_ptr loop_tri_bottom = EulerOp::kemr(bridge->edge, loop_bottom);

    EulerOp::sweep(loop_tri_top, vec3(0.0f, 1.0f, 0.0f), 1.0f);

    loop_tri_top = solid->last_face()->f_loop;
    EulerOp::kfmrh(loop_tri_top, loop_top);

    loop_tri_bottom->inner = true;
    loop_tri_top->inner = true;

    cout << "loop_tri_bottom " << loop_tri_bottom << endl;
    cout << "loop_tri_top " << loop_tri_top << endl;

    for (face_ptr f = solid->s_faces; f != nullptr; f = f->f_next) {
        cout << "f: " << f << endl;
        for (loop_ptr lp = f->f_loop; lp != nullptr; lp = lp->l_next) {
            cout << "\tl(" << lp << "): ";
            lp->debug();
        }
        cout << "-------" << endl;
    }
    return solid;
}

solid_ptr create_two_hole() {
    double height = 4;
    solid_ptr solid;

    vertex_ptr topUpperLeft;
    solid = EulerOp::mvfs(topUpperLeft, vec3(-4.0, 0, 4.0));
    loop_ptr topLoop = solid->s_faces->f_loop;
    halfedge_ptr topUpperHalfEdge = EulerOp::mev(topUpperLeft, vec3(4.0, 0, 4.0), topLoop);
    vertex_ptr topUpperRight = topUpperHalfEdge->couple->start_v;
    halfedge_ptr topLeftHalfEdge = EulerOp::mev(topUpperLeft, vec3(-4.0, 0, -4.0), topLoop);
    vertex_ptr topBottomLeft = topLeftHalfEdge->couple->start_v;
    halfedge_ptr topRightHalfEdge = EulerOp::mev(topUpperRight, vec3(4.0, 0, -4.0), topLoop);
    vertex_ptr topBottomRight = topRightHalfEdge->couple->start_v;
    // Make top ring
    loop_ptr downLoop = EulerOp::mef(topBottomLeft, topBottomRight, topLoop);

    halfedge_ptr upperLeftHalfEdge = EulerOp::mev(topUpperLeft, vec3(-4.0, height, 4.0), downLoop);
    vertex_ptr downUpperLeft = upperLeftHalfEdge->couple->start_v;
    halfedge_ptr upperRightHalfEdge = EulerOp::mev(topUpperRight, vec3(4.0, height, 4.0), downLoop);
    vertex_ptr downUpperRight = upperRightHalfEdge->couple->start_v;
    halfedge_ptr bottomLeftHalfEdge = EulerOp::mev(topBottomLeft, vec3(-4.0, height, -4.0), downLoop);
    vertex_ptr downBottomLeft = bottomLeftHalfEdge->couple->start_v;
    halfedge_ptr bottomRightHalfEdge = EulerOp::mev(topBottomRight, vec3(4.0, height, -4.0), downLoop);
    vertex_ptr downBottomRight = bottomRightHalfEdge->couple->start_v;

    loop_ptr upperLoop = EulerOp::mef(downUpperLeft, downUpperRight, downLoop);
    loop_ptr rightLoop = EulerOp::mef(downUpperRight, downBottomRight, downLoop);
    loop_ptr bottomLoop = EulerOp::mef(downBottomRight, downBottomLeft, downLoop);
    loop_ptr leftLoop = EulerOp::mef(downBottomLeft, downUpperLeft, downLoop);

    // Top inner ring

    halfedge_ptr topBridge = EulerOp::mev(topUpperLeft, vec3(-3, 0, 3), topLoop);
    vertex_ptr topInnerUpperLeft = topBridge->couple->start_v;

    halfedge_ptr topInnerUpperHalfEdge = EulerOp::mev(topInnerUpperLeft, vec3(-1, 0, 3), topLoop);
    vertex_ptr topInnerUpperRight = topInnerUpperHalfEdge->couple->start_v;
    halfedge_ptr topInnerRightHalfEdge = EulerOp::mev(topInnerUpperRight, vec3(-1, 0, 1), topLoop);
    vertex_ptr topInnerBottomRight = topInnerRightHalfEdge->couple->start_v;
    halfedge_ptr topInnerLeftHalfEdge = EulerOp::mev(topInnerUpperLeft, vec3(-3, 0, 1), topLoop);
    vertex_ptr topInnerBottomLeft = topInnerLeftHalfEdge->couple->start_v;
    loop_ptr downInnerLoop = EulerOp::mef(topInnerBottomLeft, topInnerBottomRight, topLoop);

    loop_ptr topInnerLoop = EulerOp::kemr(topBridge->edge, topLoop);

    halfedge_ptr upperLeftInnerHalfEdge = EulerOp::mev(topInnerUpperLeft, vec3(-3, height, 3), downInnerLoop);
    vertex_ptr downInnerUpperLeft = upperLeftInnerHalfEdge->couple->start_v;
    halfedge_ptr upperRightInnerHalfEdge = EulerOp::mev(topInnerUpperRight, vec3(-1, height, 3), downInnerLoop);
    vertex_ptr downInnerUpperRight = upperRightInnerHalfEdge->couple->start_v;
    halfedge_ptr bottomLeftInnerHalfEdge = EulerOp::mev(topInnerBottomLeft, vec3(-3, height, 1), downInnerLoop);
    vertex_ptr downInnerBottomLeft = bottomLeftInnerHalfEdge->couple->start_v;
    halfedge_ptr bottomRightInnerHalfEdge = EulerOp::mev(topInnerBottomRight, vec3(-1, height, 1), downInnerLoop);
    vertex_ptr downInnerBottomRight = bottomRightInnerHalfEdge->couple->start_v;

    loop_ptr upperInnerLoop = EulerOp::mef(downInnerUpperLeft, downInnerUpperRight, downInnerLoop);
    loop_ptr rightInnerLoop = EulerOp::mef(downInnerUpperRight, downInnerBottomRight, downInnerLoop);
    loop_ptr bottomInnerLoop = EulerOp::mef(downInnerBottomRight, downInnerBottomLeft, downInnerLoop);
    loop_ptr leftInnerLoop = EulerOp::mef(downInnerBottomLeft, downInnerUpperLeft, downInnerLoop);

    EulerOp::kfmrh(downLoop, downInnerLoop);

    topInnerLoop->inner = true;
    downInnerLoop->inner = true;

    halfedge_ptr topBridge2 = EulerOp::mev(topUpperLeft, vec3(1, 0, -1), topLoop);
    vertex_ptr topInnerUpperLeft2 = topBridge2->couple->start_v;

    halfedge_ptr topInnerUpperHalfEdge2 = EulerOp::mev(topInnerUpperLeft2, vec3(3, 0, -1), topLoop);
    vertex_ptr topInnerUpperRight2 = topInnerUpperHalfEdge2->couple->start_v;
    halfedge_ptr topInnerRightHalfEdge2 = EulerOp::mev(topInnerUpperRight2, vec3(3, 0, -3), topLoop);
    vertex_ptr topInnerBottomRight2 = topInnerRightHalfEdge2->couple->start_v;
    halfedge_ptr topInnerLeftHalfEdge2 = EulerOp::mev(topInnerUpperLeft2, vec3(1, 0, -3), topLoop);
    vertex_ptr topInnerBottomLeft2 = topInnerLeftHalfEdge2->couple->start_v;
    loop_ptr downInnerLoop2 = EulerOp::mef(topInnerBottomLeft2, topInnerBottomRight2, topLoop);

    loop_ptr topInnerLoop2 = EulerOp::kemr(topBridge2->edge, topLoop);

    halfedge_ptr upperLeftInnerHalfEdge2 = EulerOp::mev(topInnerUpperLeft2, vec3(1, height, -1), downInnerLoop2);
    vertex_ptr downInnerUpperLeft2 = upperLeftInnerHalfEdge2->couple->start_v;
    halfedge_ptr upperRightInnerHalfEdge2 = EulerOp::mev(topInnerUpperRight2, vec3(3, height, -1), downInnerLoop2);
    vertex_ptr downInnerUpperRight2 = upperRightInnerHalfEdge2->couple->start_v;
    halfedge_ptr bottomLeftInnerHalfEdge2 = EulerOp::mev(topInnerBottomLeft2, vec3(1, height, -3), downInnerLoop2);
    vertex_ptr downInnerBottomLeft2 = bottomLeftInnerHalfEdge2->couple->start_v;
    halfedge_ptr bottomRightInnerHalfEdge2 = EulerOp::mev(topInnerBottomRight2, vec3(3, height, -3), downInnerLoop2);
    vertex_ptr downInnerBottomRight2 = bottomRightInnerHalfEdge2->couple->start_v;

    loop_ptr upperInnerLoop2 = EulerOp::mef(downInnerUpperLeft2, downInnerUpperRight2, downInnerLoop2);
    loop_ptr rightInnerLoop2 = EulerOp::mef(downInnerUpperRight2, downInnerBottomRight2, downInnerLoop2);
    loop_ptr bottomInnerLoop2 = EulerOp::mef(downInnerBottomRight2, downInnerBottomLeft2, downInnerLoop2);
    loop_ptr leftInnerLoop2 = EulerOp::mef(downInnerBottomLeft2, downInnerUpperLeft2, downInnerLoop2);

    EulerOp::kfmrh(downInnerLoop2, downLoop);

    topInnerLoop2->inner = true;
    downInnerLoop2->inner = true;

    return solid;
}

#endif //CADEULEROPERATION_EULERTEST_H

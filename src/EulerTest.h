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
    vec3 p[16] = {
            vec3(-4.0, 0, 4.0),
            vec3(4.0, 0, 4.0),
            vec3(-4.0, 0, -4.0),
            vec3(4.0, 0, -4.0),
            vec3(-4.0, height, 4.0),
            vec3(4.0, height, 4.0),
            vec3(-4.0, height, -4.0),
            vec3(4.0, height, -4.0),
            vec3(-3, 0, 3),
            vec3(-1, 0, 3),
            vec3(-1, 0, 1),
            vec3(-3, 0, 1),
            vec3(-3, height, 3),
            vec3(-1, height, 3),
            vec3(-3, height, 1),
            vec3(-1, height, 1)
    };

    vertex_ptr v[24];

    /**
     * ************* outer
     */
    solid = EulerOp::mvfs(v[0], p[0]);
    loop_ptr loop_top = solid->s_faces->f_loop;
    halfedge_ptr h1 = EulerOp::mev(v[0], p[1], loop_top);
    v[1] = h1->couple->start_v;
    halfedge_ptr h2 = EulerOp::mev(v[0], p[2], loop_top);
    v[2] = h2->couple->start_v;
    halfedge_ptr h3 = EulerOp::mev(v[1], p[3], loop_top);
    v[3] = h3->couple->start_v;

    loop_ptr loop_bottom = EulerOp::mef(v[2], v[3], loop_top);

    halfedge_ptr h4 = EulerOp::mev(v[0], p[4], loop_bottom);
    v[4] = h4->couple->start_v;
    halfedge_ptr h5 = EulerOp::mev(v[1], p[5], loop_bottom);
    v[5] = h5->couple->start_v;
    halfedge_ptr h6 = EulerOp::mev(v[2], p[6], loop_bottom);
    v[6] = h6->couple->start_v;
    halfedge_ptr h7 = EulerOp::mev(v[3], p[7], loop_bottom);
    v[7] = h7->couple->start_v;

    EulerOp::mef(v[4], v[5], loop_bottom);
    EulerOp::mef(v[5], v[7], loop_bottom);
    EulerOp::mef(v[7], v[6], loop_bottom);
    EulerOp::mef(v[6], v[4], loop_bottom);

    /**
     * ************* outer
     */

    /**
     * ************* inner 1
     */

    halfedge_ptr h_connect_1 = EulerOp::mev(v[0], p[8], loop_top);
    v[8] = h_connect_1->couple->start_v;

    halfedge_ptr h8 = EulerOp::mev(v[8], p[9], loop_top);
    v[9] = h8->couple->start_v;
    halfedge_ptr h9 = EulerOp::mev(v[9], p[10], loop_top);
    v[10] = h9->couple->start_v;
    halfedge_ptr h10 = EulerOp::mev(v[8], p[11], loop_top);
    v[11] = h10->couple->start_v;

    loop_ptr loop_inner_bottom = EulerOp::mef(v[11], v[10], loop_top);

    loop_ptr loop_inner_top = EulerOp::kemr(h_connect_1->edge, loop_top);

    halfedge_ptr h11 = EulerOp::mev(v[8], p[12], loop_inner_bottom);
    v[12] = h11->couple->start_v;
    halfedge_ptr h12 = EulerOp::mev(v[9], p[13], loop_inner_bottom);
    v[13] = h12->couple->start_v;
    halfedge_ptr h13 = EulerOp::mev(v[11], p[14], loop_inner_bottom);
    v[14] = h13->couple->start_v;
    halfedge_ptr h14 = EulerOp::mev(v[10], p[15], loop_inner_bottom);
    v[15] = h14->couple->start_v;

    EulerOp::mef(v[12], v[13], loop_inner_bottom);
    EulerOp::mef(v[13], v[15], loop_inner_bottom);
    EulerOp::mef(v[15], v[14], loop_inner_bottom);
    EulerOp::mef(v[14], v[12], loop_inner_bottom);
    EulerOp::kfmrh(loop_bottom, loop_inner_bottom);
    loop_inner_top->inner = true;
    loop_inner_bottom->inner = true;

    /**
    * ************* inner 1
    */

    /**
    * ************* inner 2
    */

    halfedge_ptr h15 = EulerOp::mev(v[0], vec3(1, 0, -1), loop_top);
    v[16] = h15->couple->start_v;
    halfedge_ptr h16 = EulerOp::mev(v[16], vec3(3, 0, -1), loop_top);
    v[17] = h16->couple->start_v;
    halfedge_ptr h17 = EulerOp::mev(v[17], vec3(3, 0, -3), loop_top);
    v[18] = h17->couple->start_v;
    halfedge_ptr h18 = EulerOp::mev(v[16], vec3(1, 0, -3), loop_top);
    v[19] = h18->couple->start_v;

    loop_ptr loop_inner_bottom_2 = EulerOp::mef(v[19], v[18], loop_top);
    loop_ptr loop_inner_top_2 = EulerOp::kemr(h15->edge, loop_top);

    halfedge_ptr h19 = EulerOp::mev(v[16], vec3(1, height, -1), loop_inner_bottom_2);
    v[20] = h19->couple->start_v;
    halfedge_ptr h20 = EulerOp::mev(v[17], vec3(3, height, -1), loop_inner_bottom_2);
    v[21] = h20->couple->start_v;
    halfedge_ptr h21 = EulerOp::mev(v[19], vec3(1, height, -3), loop_inner_bottom_2);
    v[22] = h21->couple->start_v;
    halfedge_ptr h22 = EulerOp::mev(v[18], vec3(3, height, -3), loop_inner_bottom_2);
    v[23] = h22->couple->start_v;

    EulerOp::mef(v[20], v[21], loop_inner_bottom_2);
    EulerOp::mef(v[21], v[23], loop_inner_bottom_2);
    EulerOp::mef(v[23], v[22], loop_inner_bottom_2);
    EulerOp::mef(v[22], v[20], loop_inner_bottom_2);

    EulerOp::kfmrh(loop_inner_bottom_2, loop_bottom);

    loop_inner_top_2->inner = true;
    loop_inner_bottom_2->inner = true;

    /**
    * ************* inner 2
    */
    return solid;
}

#endif //CADEULEROPERATION_EULERTEST_H

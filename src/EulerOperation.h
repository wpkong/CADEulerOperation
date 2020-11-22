/**
 * ------------------------------------
 * @author: Weipeng Kong
 * @date: 2020/11/12
 * @email: yjxkwp@foxmail.com
 * @site: https://x86.design
 * @description: 
 * ------------------------------------
**/

#ifndef CADEULEROPERATION_EULEROPERATION_H
#define CADEULEROPERATION_EULEROPERATION_H

#include "B-rep.h"

namespace EulerOp {
    // Make one vertex, one face and one solid
    solid_ptr mvfs(vertex_ptr &v, const vec3 &new_coord);

    // Make one vertex and a edge
    halfedge_ptr mev(vertex_ptr &v1, const vec3 &new_coord, loop_ptr &loop);

    // Make one edge, one face and one loop
    loop_ptr mef(vertex_ptr &v1, vertex_ptr &v2, loop_ptr &loop);

    // Kill one edge and make one ring
    loop_ptr kemr(edge_ptr e, loop_ptr &loop);

    // kill one face and make one ring and one hole
    void kfmrh(loop_ptr &inner, loop_ptr &outer);

    // make a sweep solid
    solid_ptr sweep(face_ptr &face, const vec3 &direct, float dist);

    // make a sweep solid
    void sweep(loop_ptr &loop, const vec3 &direct, float dist);
}


#endif //CADEULEROPERATION_EULEROPERATION_H

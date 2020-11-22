/**
 * ------------------------------------
 * @author: Weipeng Kong
 * @date: 2020/11/12
 * @email: yjxkwp@foxmail.com
 * @site: https://x86.design
 * @description: 
 * ------------------------------------
**/

#include "EulerOperation.h"

#include <memory>
#include <iostream>

using std::cout;
using std::endl;
using std::make_shared;

solid_ptr EulerOp::mvfs(vertex_ptr &new_vertex, const vec3 &new_coord) {

    solid_ptr new_solid = new Solid();
    face_ptr new_face = new Face();
    loop_ptr new_loop = new Loop();

    new_vertex = new Vertex();
    new_vertex->v_coordinate = new_coord;

    new_solid->add_face(new_face);
    new_face->add_loop(new_loop);
    return new_solid;
}

halfedge_ptr EulerOp::mev(vertex_ptr &v1, const vec3 &new_coord, loop_ptr &loop) {
    nullptr_assertion({v1, loop});

    halfedge_ptr new_he1 = new HalfEdge(),
            new_he2 = new HalfEdge();
    edge_ptr new_edge = new Edge();
    vertex_ptr new_v2 = new Vertex();

    new_v2->v_coordinate = new_coord;

    new_edge->connect_half_edge(new_he1, new_he2);

    new_he1->start_v = v1;
    new_he2->start_v = new_v2;

    loop->add_edge(new_he1, new_he2);
    // add to new_edge list
    solid_ptr solid = loop->l_face->f_solid;

    solid->add_edge(new_edge);

    return new_he1;
}


loop_ptr EulerOp::mef(vertex_ptr &v1, vertex_ptr &v2, loop_ptr &loop) {
    nullptr_assertion({v1, v2, loop});

    loop_ptr new_loop = new Loop();
    face_ptr new_face = new Face();
//    face_ptr face = loop->l_face;
    halfedge_ptr new_he1 = new HalfEdge();
    halfedge_ptr new_he2 = new HalfEdge();
    edge_ptr new_edge = new Edge();

    new_he1->start_v = v1;
    new_he2->start_v = v2;

    new_edge->connect_half_edge(new_he1, new_he2);

    // find half edge
    halfedge_ptr the1, the2;
    the1 = loop->find_half_edge(v1);
    the2 = loop->find_half_edge(v2);

    loop->l_edge = new_he1;
    new_loop->l_edge = new_he2;

//    the1->he_prev->he_next = new_he1;
//    new_he1->he_prev = the1->he_prev;
//    the2->he_prev->he_next = new_he2;
//    new_he2->he_prev = the2->he_prev;
//    new_he2->he_next = the1;
//    the1->he_prev = new_he2;
//    new_he1->he_next = the2;
//    the2->he_prev = new_he1;
    new_he2->he_next = the1;
    new_he2->he_prev = the2->he_prev;
    new_he1->he_next = the2;
    new_he1->he_prev = the1->he_prev;
    the1->he_prev->he_next = new_he1;
    the1->he_prev = new_he2;
    the2->he_prev->he_next = new_he2;
    the2->he_prev = new_he1;

    solid_ptr solid = loop->l_face->f_solid;
    solid->add_face(new_face);
    new_face->add_loop(new_loop);
//    face->add_loop(new_loop);

    if (new_loop->edge_n() > loop->edge_n()) {
        loop_ptr tmp = new_loop;
        new_loop = loop;
        loop = tmp;
    }

    return new_loop;
}

loop_ptr EulerOp::kemr(edge_ptr e, loop_ptr &loop) {
    nullptr_assertion({e, loop});
    halfedge_ptr he1, he2;
    he1 = e->he1;
    he2 = e->he2;
    nullptr_assertion({he1, he2});

    vertex_ptr v1, v2;
    v1 = he1->start_v;
    v2 = he2->start_v;
    nullptr_assertion({v1, v2});

    face_ptr face = loop->l_face;
    nullptr_assertion({face});

    loop_ptr new_loop = new Loop();
    new_loop->l_face = loop->l_face;

    he1->he_next->he_prev = he2->he_prev;
    he2->he_prev->he_next = he1->he_next;

    he1->he_prev->he_next = he2->he_next;
    he2->he_next->he_prev = he1->he_prev;

    new_loop->l_edge = he1->he_next;
    // TODO: Validation
    loop->l_edge = he1->he_prev;

    delete he1;
    delete he2;

    // delete edge from the solid

    solid_ptr solid = loop->l_face->f_solid;
    if (e == solid->s_edges) {
        solid->s_edges = nullptr;
    } else {
        if (e->e_next == nullptr) {
            e->e_prev->e_next = nullptr;
        } else {
            e->e_prev->e_next = e->e_next;
            e->e_next->e_prev = e->e_prev;
        }
    }
    delete e;
    face->add_loop(new_loop);

    if (new_loop->edge_n() > loop->edge_n()) {
        loop_ptr tmp = new_loop;
        new_loop = loop;
        loop = tmp;
    }
    return new_loop;
}


void EulerOp::kfmrh(loop_ptr &inner, loop_ptr &outer) {
    nullptr_assertion({inner, outer});
//    assert(inner->l_face == outer->l_face && inner->l_face->f_solid == outer->l_face->f_solid);
    solid_ptr solid = inner->l_face->f_solid;
    face_ptr deleting_face = inner->l_face;

    if (deleting_face == solid->s_faces) {
        solid->s_edges = nullptr;
    } else {
        if (deleting_face->f_next == nullptr) {
            deleting_face->f_prev->f_next = nullptr;
        } else {
            deleting_face->f_prev->f_next = deleting_face->f_next;
            deleting_face->f_next->f_prev = deleting_face->f_prev;
        }
    }
    delete deleting_face;

    outer->l_face->add_loop(inner);
}


solid_ptr EulerOp::sweep(face_ptr &face, const vec3 &direct, float dist) {
    solid_ptr solid = face->f_solid;
    loop_ptr loop;
    halfedge_ptr he;

    for (loop = face->f_loop; loop != nullptr; loop = loop->l_next) {
        he = loop->l_edge;
        vertex_ptr first_v = he->start_v;
        vertex_ptr last_up_v, first_up_v;
        vec3 first_up_position = first_v->v_coordinate + direct * dist;
        halfedge_ptr first_new_edge = mev(first_v, first_up_position, loop);
        first_up_v = last_up_v = first_new_edge->couple->start_v;

        he = he->he_next;
        vertex_ptr v = he->start_v;
//        while (he != loop->l_edge && he != nullptr){
        while (v != first_v) {
            vec3 up_position = v->v_coordinate + direct * dist;
            halfedge_ptr new_edge = mev(v, up_position, loop);
            vertex_ptr up_v = new_edge->couple->start_v;
            mef(up_v, last_up_v, loop);  // Face attribute has been initialized in the function
            last_up_v = up_v;
            he = he->he_next;
            v = he->start_v;
        }
        mef(first_up_v, last_up_v, loop);
    }
    return solid;
}

void EulerOp::sweep(loop_ptr &loop, const vec3 &direct, float dist) {
    halfedge_ptr he;
    he = loop->l_edge;
    vertex_ptr first_v = he->start_v;
    vertex_ptr last_up_v, first_up_v;
    vec3 first_up_position = first_v->v_coordinate + direct * dist;
    halfedge_ptr first_new_edge = mev(first_v, first_up_position, loop);
    first_up_v = last_up_v = first_new_edge->couple->start_v;

    he = he->he_next;
    vertex_ptr v = he->start_v;
//        while (he != loop->l_edge && he != nullptr){
    while (v != first_v) {
        vec3 up_position = v->v_coordinate + direct * dist;
        halfedge_ptr new_edge = mev(v, up_position, loop);
        vertex_ptr up_v = new_edge->couple->start_v;
        mef(up_v, last_up_v, loop);  // Face attribute has been initialized in the function
        last_up_v = up_v;
        he = he->he_next;
        v = he->start_v;
    }
    mef(first_up_v, last_up_v, loop);
}
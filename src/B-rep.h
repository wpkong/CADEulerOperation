/**
 * ------------------------------------
 * @author: Weipeng Kong
 * @date: 2020/11/12
 * @email: yjxkwp@foxmail.com
 * @site: https://x86.design
 * @description:
 *      关于指针的说明：
 *          按照半边数据结构的结构，从上到下（从高层到底层，比如solid->face）使用
 *          shared_ptr，指回、链表的指针（face->solid）用weak_ptr
 * ------------------------------------
**/

#ifndef CADEULEROPERATION_B_REP_H
#define CADEULEROPERATION_B_REP_H

#include <memory>
#include "common.h"

class Solid;

class Vertex;

class Edge;

class HalfEdge;

class Face;

class Loop;


#if 0
typedef std::shared_ptr<Solid> solid_s_ptr;
typedef std::shared_ptr<Vertex> vertex_s_ptr;
typedef std::shared_ptr<Edge> edge_s_ptr;
typedef std::shared_ptr<Loop> loop_s_ptr;
typedef std::shared_ptr<HalfEdge> halfedge_s_ptr;
typedef std::shared_ptr<Face> face_s_ptr;

typedef std::weak_ptr<Solid> solid_w_ptr;
typedef std::weak_ptr<Vertex> vertex_w_ptr;
typedef std::weak_ptr<Edge> edge_w_ptr;
typedef std::weak_ptr<Loop> loop_w_ptr;
typedef std::weak_ptr<HalfEdge> halfedge_w_ptr;
typedef std::weak_ptr<Face> face_w_ptr;
#endif

#if 1
typedef Solid *solid_ptr;
typedef Vertex *vertex_ptr;
typedef Edge *edge_ptr;
typedef Loop *loop_ptr;
typedef HalfEdge *halfedge_ptr;
typedef Face *face_ptr;
#endif

/**
 * The link table in Solid and Face is Non-recursive
 */
class Solid {
public:
    face_ptr s_faces;
    edge_ptr s_edges;
//    vertex_ptr s_vertices;
    solid_ptr s_next;
    solid_ptr s_prev;

public:
    explicit Solid();

    void add_face(face_ptr face);

    void add_edge(edge_ptr edge);

//    void add_vertex(vertex_ptr v);

    size_t face_n() const;

    face_ptr last_face();
};


class Face {
public:
    solid_ptr f_solid;
    loop_ptr f_loop;
    // TODO: face expression
    vec3 color;
    face_ptr f_prev, f_next;

public:
    explicit Face();

    void add_loop(loop_ptr loop);

    size_t loop_n() const;
};

/**
 * The link table of edge in Loop is recursive
 */
class Loop {
public:
    halfedge_ptr l_edge;
    face_ptr l_face;
    loop_ptr l_prev, l_next;
    bool inner;

public:
    explicit Loop();

    void add_edge(halfedge_ptr he);

    void add_edge(halfedge_ptr he1, halfedge_ptr he2);

    halfedge_ptr find_half_edge(vertex_ptr v) const;

    size_t edge_n() const;

    void debug();

    bool is_vertex_in(vertex_ptr v);
};

class Edge {
public:
    halfedge_ptr he1, he2;
    edge_ptr e_prev, e_next;

public:
    explicit Edge();

    void connect_half_edge(halfedge_ptr he1, halfedge_ptr he2);
};


class HalfEdge {
public:
    edge_ptr edge;
    vertex_ptr start_v;  // starting vertex
    loop_ptr w_loop;
    halfedge_ptr he_prev, he_next;
    halfedge_ptr couple;

public:
    explicit HalfEdge();
};

class Vertex {
public:
    halfedge_ptr v_edge;
    vec3 v_coordinate;
    vertex_ptr v_prev, v_next;

public:
    explicit Vertex();
};


#endif //CADEULEROPERATION_B_REP_H

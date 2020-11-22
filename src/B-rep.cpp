/**
 * ------------------------------------
 * @author: Weipeng Kong
 * @date: 2020/11/17
 * @email: yjxkwp@foxmail.com
 * @site: https://x86.design
 * @description: 
 * ------------------------------------
**/

#include "B-rep.h"
#include <iostream>

using namespace std;

void Solid::add_edge(edge_ptr edge) {
    nullptr_assertion({edge});

    if (this->s_edges == nullptr) {
        this->s_edges = edge;
    } else {
        edge_ptr tmp = this->s_edges;
        while (tmp->e_next) tmp = tmp->e_next;
        tmp->e_next = edge;
        edge->e_next = nullptr;
        edge->e_prev = tmp;
    }
}

void Solid::add_face(face_ptr face) {
    nullptr_assertion({face});

    if (this->s_faces == nullptr) {
        this->s_faces = face;
        face->f_prev = face->f_next = nullptr;
    } else {
        face_ptr tmp = this->s_faces;
        while (tmp->f_next) tmp = tmp->f_next;
        tmp->f_next = face;
        face->f_next = nullptr;
        face->f_prev = tmp;
    }
    face->f_solid = this;
}

size_t Solid::face_n() const {
    size_t n = 0;
    face_ptr f = s_faces;
    while (f) {
        n++;
        f = f->f_next;
    }
    return n;
}

Solid::Solid() : s_faces(nullptr), s_edges(nullptr), s_next(nullptr), s_prev(nullptr) {

}

face_ptr Solid::last_face() {
    face_ptr f = s_faces;
    if(s_faces == nullptr) return nullptr;
    while (f->f_next) {
        f = f->f_next;
    }
    return f;
}

//void Solid::add_vertex(vertex_ptr v) {
//    nullptr_assertion({v});
//    if(this->s_vertices == nullptr){
//        this->s_vertices = v;
//        v->v_prev = v->v_next = nullptr;
//    }else{
//        vertex_ptr tmp = this->s_vertices;
//        while(tmp->v_next) tmp = tmp->v_next;
//        tmp->v_next = v;
//        v->v_next = nullptr;
//        v->v_prev = tmp;
//    }
//}

void Face::add_loop(loop_ptr loop) {
    nullptr_assertion({loop});
    if (this->f_loop == nullptr) {
        this->f_loop = loop;
//        loop->l_prev = loop->l_next = nullptr;
    } else {
        loop_ptr tmp = this->f_loop;
        while (tmp->l_next != nullptr) {
            tmp = tmp->l_next;
        }
        tmp->l_next = loop;
        loop->l_next = nullptr;
        loop->l_prev = tmp;
    }
    loop->l_face = this;
}

size_t Face::loop_n() const {
    size_t n = 0;
    loop_ptr l = f_loop;
    while (l) {
        n++;
        l = l->l_next;
    }
    return n;
}

Face::Face() : f_solid(nullptr), f_loop(nullptr), f_prev(nullptr), f_next(nullptr),
               color(rand() % 255 / 255.0f, rand() % 255 / 255.0f, rand() % 255 / 255.0f) {
}

void Loop::add_edge(halfedge_ptr he) {
    nullptr_assertion({he});
    if (this->l_edge == nullptr) {
        this->l_edge = he;
    } else {
        auto tmp = this->l_edge;
        while (tmp->he_next) tmp = tmp->he_next;
        tmp->he_next = he;
        he->he_prev = tmp;
    }
    he->w_loop = this;
}

void Loop::add_edge(halfedge_ptr he1, halfedge_ptr he2) {
    nullptr_assertion({he1, he2});
    he1->couple = he2;
    he2->couple = he1;
    he1->he_next = he2;
    he2->he_prev = he1;

    if (this->l_edge == nullptr) {
        this->l_edge = he1;
        he1->he_prev = he2;
        he2->he_next = he1;
    } else {
        if(!this->is_vertex_in(he1->start_v)){
            throw std::logic_error("404");
        }
        halfedge_ptr tmp = this->l_edge;
        while (tmp->he_next->start_v != he1->start_v) {
//            std::cout << tmp->start_v->v_coordinate.x << " " << tmp->start_v->v_coordinate.y << " "
//                      << tmp->start_v->v_coordinate.z << " " << endl;
            tmp = tmp->he_next;
        }   // TODO: There should be a previous validation ,which guarantees such edge can be found, in the beginning of this function.
        he2->he_next = tmp->he_next;
        tmp->he_next->he_prev = he2;
        tmp->he_next = he1;
        he1->he_prev = tmp;
    }
    he1->w_loop = this;
    he2->w_loop = this;
}

halfedge_ptr Loop::find_half_edge(vertex_ptr v) const {
    if (this->l_edge == nullptr) return nullptr;

    halfedge_ptr tmp = this->l_edge;
    while (tmp->start_v != v) {
        tmp = tmp->he_next;
        if (tmp == this->l_edge) {
            return nullptr;  // not found
        }
    }
    return tmp;
}

size_t Loop::edge_n() const {
    size_t n = 0;
    halfedge_ptr e = l_edge;
    do {
//        cout << e << endl;
        if (e == nullptr) break;
        n++;
        e = e->he_next;
    } while (e && e != l_edge);
    return n;
}

Loop::Loop() : l_edge(nullptr), l_face(nullptr), l_prev(nullptr), l_next(nullptr), inner(false) {

}

void Loop::debug() {
    halfedge_ptr e = l_edge;
    do {
        if (e == nullptr) break;
        cout << "(" << e->start_v->v_coordinate.x << "," <<
            e->start_v->v_coordinate.y << "," <<e->start_v->v_coordinate.z << ") -> ";
        e = e->he_next;
    } while (e && e != l_edge);
    cout << endl;
}

bool Loop::is_vertex_in(vertex_ptr v) {
    halfedge_ptr e = l_edge;
    do {
        if (e == nullptr) break;
        if(e->start_v == v) return true;
        e = e->he_next;
    } while (e && e != l_edge);

    return false;
}

void Edge::connect_half_edge(halfedge_ptr new_he1, halfedge_ptr new_he2) {
    this->he1 = new_he1;
    this->he2 = new_he2;
    new_he1->edge = new_he2->edge = this;
    new_he1->couple = new_he2;
    new_he2->couple = new_he1;
}

Edge::Edge() : he1(nullptr), he2(nullptr), e_prev(nullptr), e_next(nullptr) {

}

HalfEdge::HalfEdge() : edge(nullptr), start_v(nullptr), w_loop(nullptr), he_prev(nullptr), he_next(nullptr),
                       couple(nullptr) {

}

Vertex::Vertex() : v_edge(nullptr), v_prev(nullptr), v_next(nullptr) {

}

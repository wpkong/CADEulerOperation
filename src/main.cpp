#include <iostream>
#include <glm/glm.hpp>
#include "ModelPainter.h"

using namespace glm;
using std::vector;
using std::cout;
using std::endl;

#include "EulerOperation.h"
#include "EulerTest.h"


int main(int argc, char **argv) {
    vec3 base(-10, 0, -5);
    solid_ptr solid;
    if (argc == 1) {
        solid = create_C(base + vec3(0, 0, 0), 0.5f);
        solid->s_next = create_A(base + vec3(7, 0, 0), 0.5f);
        solid->s_next->s_next = create_D(base + vec3(16, 0, 0), 0.5f);
    } else {
        switch (argv[1][0]) {
            case '1':
                solid = create_block();
                break;
            case '2':
                solid = create_two_hole();
                break;
            case '3':
                solid = create_C(base + vec3(0, 0, 0), 0.5f);
                solid->s_next = create_A(base + vec3(7, 0, 0), 0.5f);
                solid->s_next->s_next = create_D(base + vec3(16, 0, 0), 0.5f);
                break;
            default:
                return 1;
        }
    }

    cout << "face n: " << solid->face_n() << endl;
    face_ptr f = solid->s_faces;
    while (f) {
        cout << "\tf loop n: " << f->loop_n() << endl;
        f = f->f_next;
    }

    ModelPainter::init();

    ModelPainter::draw(solid);

    ModelPainter::end();

    return 0;
}

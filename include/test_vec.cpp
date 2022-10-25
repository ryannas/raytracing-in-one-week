#include <iostream>
#include "vec.h"

using std::cout;
using std::endl;

int main() {
    Vec3 a(1, 2, 3);
    Vec3 b(1, 2, 3);
    cout << "vec1: " << a << endl;
    cout << "vec2: " << b << endl;
    cout << "dot: " << a.dot(b) << endl;
    cout << "cross: " << a.cross(b) << endl;
    cout << "normalize: " << a.normalized() << endl;
    cout << "op +: " << a + b << endl;
    cout << "op -: " << a - b << endl;
    cout << "op *: " << a * b << endl;
    return 0;
}

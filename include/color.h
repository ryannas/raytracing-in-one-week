#ifndef COLOR_H
#define COLOR_H

#include "vec.h"

#include <iostream>

using color = Vec3;

void write_color(std::ostream& out, color pixel_color) {  
    out << static_cast<int>(255.999 * pixel_color.x) << " "
        << static_cast<int>(255.999 * pixel_color.y) << " "
        << static_cast<int>(255.999 * pixel_color.z) << "\n";
}

#endif


#ifndef PPM_H
#define PPM_H

#include "color.h"

#include <vector>
#include <iostream>
#include <fstream>

using std::vector;

class PPM {
public:
    int width;
    int height;
    int samples_per_pixel;
    int comp;
    vector<vector<color>> ppm;

    PPM(int w, int h, int s)
        : width(w), height(h), samples_per_pixel(s), comp(0) {
        ppm.resize(width);
        for (auto &r : ppm)
            r.resize(height);
    }

    void shade(int i, int j, const color& c) {
        ppm[i][j] = c;
        comp++;
        // std::cerr << "\rcomplete " << comp / width / height * 100 << "% " << std::flush;
    }

    void write_to_file(const std::string& file_name = "output.ppm") {
        std::ofstream os;
        os.open(file_name, std::ios::out);
        os << "P3\n" << width << " " << height << "\n255\n";
        for (int j = height - 1; j >= 0; --j)
            for (int i = 0; i < width; ++i)
                write_color(os, ppm[i][j], samples_per_pixel);
        os.close();
    }
};

#endif //PPM_H

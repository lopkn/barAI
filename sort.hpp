#pragma once

#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <string>
#include <iostream>


/**
 * Sorts n triplets of floats in a flat array based on their 
 * squared Euclidean distance from (0.0, 0.0, 0.0).
 */
inline void sortTriplets(float* arr, float* pos, int n) {
    // 1. Create an index map so we don't move large chunks of data until the end
    std::vector<int> indices(n);
    std::iota(indices.begin(), indices.end(), 0);

    // 2. Sort indices based on distance calculation
    std::sort(indices.begin(), indices.end(), [&](int a, int b) {
        // Point A
        float ax = arr[a * 3]-pos[0];
        // float ay = arr[a * 3 + 1];
        float az = arr[a * 3 + 2]-pos[2];
        float distA = (ax * ax) + (az * az);

        // Point B
        float bx = arr[b * 3]-pos[0];
        // float by = arr[b * 3 + 1];
        float bz = arr[b * 3 + 2]-pos[2];
        float distB = (bx * bx) + (bz * bz);

        return distA < distB;
    });

    // 3. Reorder the original array using a temporary buffer
    std::vector<float> temp(n * 3);
    for (int i = 0; i < n; ++i) {
        temp[i * 3]     = arr[indices[i] * 3];
        temp[i * 3 + 1] = arr[indices[i] * 3 + 1];
        temp[i * 3 + 2] = arr[indices[i] * 3 + 2];
    }

    // 4. Copy the sorted data back to the original pointer
    for (int i = 0; i < n * 3; ++i) {
        arr[i] = temp[i];
    }
}

inline std::string joinTriplet(int a, int b, int c) {
    return std::to_string(a) +","+ std::to_string(b)+"," + std::to_string(c);
}

void log3(float pos[3]){
    std::cout<< "x:" << pos[0] << " y:" << pos[1] << " z:" << pos[2] << std::endl;
}


void towards3(float from[3], float to[3], float magnitude, float out[3]){
    float vx = to[0] - from[0];
    float vy = to[1] - from[1];
    float vz = to[2] - from[2];
    float d = std::sqrt(vx*vx + vy*vy + vz*vz);

    out[0] = from[0]+vx/d*magnitude;
    out[1] = from[1]+vy/d*magnitude;
    out[2] = from[2]+vz/d*magnitude;
}


void replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty()) return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        // Advance position past the replacement to avoid infinite loops 
        // if 'to' contains 'from'
        start_pos += to.length(); 
    }
}

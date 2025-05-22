#pragma once

#include <vector>
#include<iostream>
#include <utility>

using namespace std;

class Vec3
{
public:
    float x, y, z;

    Vec3(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}
};
class Cube
{

public:
    Cube(Vec3 pos, float side) : position(pos), sideLength(side)
    {
        initializeVertices();
        initializeEdges();
    }

    Vec3 position;
    float sideLength;
    vector<Vec3> vertices;
    vector<vector<int>> edges;

    void initializeVertices()
    {
        float halfSide = sideLength / 2.0f;

        vertices = {
            {position.x - halfSide, position.y - halfSide, position.z - halfSide},
            {position.x + halfSide, position.y - halfSide, position.z - halfSide},
            {position.x + halfSide, position.y + halfSide, position.z - halfSide},
            {position.x - halfSide, position.y + halfSide, position.z - halfSide},
            {position.x - halfSide, position.y - halfSide, position.z + halfSide},
            {position.x + halfSide, position.y - halfSide, position.z + halfSide},
            {position.x + halfSide, position.y + halfSide, position.z + halfSide},
            {position.x - halfSide, position.y + halfSide, position.z + halfSide}};
    }

    void initializeEdges()
    {
        edges = {
            {0, 1}, {1, 2}, {2, 3}, {3, 0}, {4, 5}, {5, 6}, {6, 7}, {7, 4}, {0, 4}, {1, 5}, {2, 6}, {3, 7}};
    }
};
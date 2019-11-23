#pragma once

#include <stdio.h>
#include <math.h>
#include <vector>
#include <nav_msgs/OccupancyGrid.h>
#include <geometry_msgs/Pose2D.h>

using namespace geometry_msgs;

class Map : public nav_msgs::OccupancyGrid {
private:
    float m_InflationRadius;
    float m_StepSize;
    int m_nNodes;

public:
    Map();

    ~Map() = default;

    int getRows() { return info.height / info.resolution; }
    int getCols() { return info.width / info.resolution; }
    float getStepSize() { return m_StepSize; }

    int8_t& getOccupancy(int row, int column);
    int8_t& getOccupancy(Pose2D position);

    Pose2D getClosestPoint(Pose2D position);

    void UpdateOccupancyMap();

    void InflateOccupancyMap();

    void InflatePoint(int row, int col);
};
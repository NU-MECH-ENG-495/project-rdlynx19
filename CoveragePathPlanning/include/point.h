#ifndef POINT_H
#define POINT_H

/**
 * @class Point
 * @brief A simple class to represent a 3D point.
 *
 * This class stores the coordinates of a point in 3D space using `x`, `y`, and `z` values.
 * It is commonly used to represent waypoints, positions, or other spatial data.
 */
class Point
{
public:
    double x; ///< The x-coordinate of the point.
    double y; ///< The y-coordinate of the point.
    double z; ///< The z-coordinate of the point.
};

#endif


#include "include/vec.hpp"

typedef struct Circle{
    vec2 center;
    int radius;
}Circle;

typedef struct Rect{
    vec2 center;
    int width;
    int height;
    double angle;
}Rect;

bool circle_circle(Circle* A, Circle* B){
    double distance_sqr = A->center.sqr_distance(B->center);
    double sqr_radius_sum = (A->radius*A->radius) + (B->radius*B->radius);

    return distance_sqr <= sqr_radius_sum;
}

bool rect_rect(Rect* A, Rect* B){
    Rect* rectSelf = static_cast<Rect*>(self->shape.get());
        Rect* rectOther = static_cast<Rect*>(other->shape.get());

        // Collect the vertices of both rectangles
        std::array<vec2, 4> vertices_self = {rectSelf->A, rectSelf->B, rectSelf->C, rectSelf->D};
        std::array<vec2, 4> vertices_other = {rectOther->A, rectOther->B, rectOther->C, rectOther->D};

        // Calculate the axes for both rectangles
        // The axes are the normals to the edges of the rectangles
        std::array<vec2, 4> axes = {
            vec2{rectSelf->B.x - rectSelf->A.x, rectSelf->B.y - rectSelf->A.y}, // Edge AB
            vec2{rectSelf->C.x - rectSelf->B.x, rectSelf->C.y - rectSelf->B.y}, // Edge BC
            vec2{rectOther->B.x - rectOther->A.x, rectOther->B.y - rectOther->A.y}, // Edge B1B2
            vec2{rectOther->C.x - rectOther->B.x, rectOther->C.y - rectOther->B.y}  // Edge B2B3
        };

        // Normalize the axes
        for (auto& axis : axes) {
            double length = std::sqrt(axis.x * axis.x + axis.y * axis.y);
            axis.x /= length;
            axis.y /= length;
        }

        // Check projections on all axes
        for (const auto& axis : axes) {
            auto [minA, maxA] = project_vertices(vertices_self, axis);
            auto [minB, maxB] = project_vertices(vertices_other, axis);

            if (maxA < minB || maxB < minA) {
                return false; // No overlap on this axis
            }
        }

        return true;
}


bool circle_rect(Circle* circle, Rect* rect){
    vec2 circle_center_local = rotatePoint(circle->center - rect->center, -rect->angle);

    vec2 closest_point;

    // Compute the closest x coordinate of the rectangle
    if (circle_center_local.x < -rect->width / 2.0) {
        closest_point.x = -rect->width / 2.0;
    } else if (circle_center_local.x > rect->width / 2.0) {
        closest_point.x = rect->width / 2.0;
    } else {
        closest_point.x = circle_center_local.x;
    }

    // Compute the closest y coordinate of the rectangle
    if (circle_center_local.y < -rect->height / 2.0) {
        closest_point.y = -rect->height / 2.0;
    } else if (circle_center_local.y > rect->height / 2.0) {
        closest_point.y = rect->height / 2.0;
    } else {
        closest_point.y = circle_center_local.y;
    }

    // Step 3: Calculate the distance squared between the circle's center and the closest point
    vec2 closest_point_global = rotatePoint(closest_point, rect->angle) + rect->center;
    double distance_sq = circle->center.sqr_distance(closest_point_global);

    // Check if the distance is less than or equal to the circle's radius squared
    return distance_sq <= (circle->radius * circle->radius);
    
}



bool rect_circle(Circle* A, Circle* B);


int main(){




    return 0;
}
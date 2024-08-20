#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <memory>
#include <algorithm>
#include <array>

#include "RB2D.hpp"

class Simulation{
public:
    std::vector<std::unique_ptr<RigidBody2D>> bodies;

    SDL_Renderer* renderer;

    double dt;

    Simulation(double _dt) : dt(_dt) {}

    void apply_ext_force(vec2 force){
        for(const auto& rb : bodies){
            rb->apply_force(force);
        }
    }

    bool circle_circle_collision(RigidBody2D* self, RigidBody2D* other, vec2* point_of_contact){
        if(self->moveable == false && other->moveable == false) return false;
        // Cast shapes to circles
        Circle* circle1 = static_cast<Circle*>(self->shape.get());
        Circle* circle2 = static_cast<Circle*>(other->shape.get());

        // Get the centers and radii of the circles
        vec2 center1 = circle1->center;
        double radius1 = circle1->radius;
        vec2 center2 = circle2->center;
        double radius2 = circle2->radius;

        // Calculate the distance between the centers
        double dx = center1.x - center2.x;
        double dy = center1.y - center2.y;
        double distance_squared = dx * dx + dy * dy;

        // Calculate the sum of the radii squared
        double radius_sum = radius1 + radius2;
        double radius_sum_squared = radius_sum * radius_sum;

        vec2 vecC1C2 = center2 - center1;
        vec2 dir = vecC1C2.normalized();
        vec2 point = center1 + dir * radius1;
        
        *point_of_contact = point;

        // Check if the distance squared is less than or equal to the radius sum squared
        return distance_squared <= radius_sum_squared;
    }

    

    bool circle_rect_collision(RigidBody2D* self, RigidBody2D* other, vec2* point_of_contact){
        if(self->moveable == false && other->moveable == false) return false;
        // Cast shapes to appropriate types
        Circle* circle = static_cast<Circle*>(self->shape.get());
        Rect* rect = static_cast<Rect*>(other->shape.get());

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

        *point_of_contact = closest_point_global;


        // Check if the distance is less than or equal to the circle's radius squared
        return distance_sq <= (circle->radius * circle->radius);


    }

    vec2 find_collision_point(const Rect& rectSelf, const Rect& rectOther) {
        vec2 closest_intersection = {std::numeric_limits<double>::max(), std::numeric_limits<double>::max()};
        double min_dist = std::numeric_limits<double>::max();

        std::vector<vec2> rect1;
        std::vector<vec2> rect2;

        rect1.push_back(rectSelf.A);  rect2.push_back(rectOther.A); 
        rect1.push_back(rectSelf.B);  rect2.push_back(rectOther.B); 
        rect1.push_back(rectSelf.C);  rect2.push_back(rectOther.C); 
        rect1.push_back(rectSelf.D);  rect2.push_back(rectOther.D);  

        for (int i = 0; i < 4; ++i) {
            vec2 A = rect1[i];
            vec2 B = rect1[(i + 1) % 4];

            for (int j = 0; j < 4; ++j) {
                vec2 C = rect2[j];
                vec2 D = rect2[(j + 1) % 4];

                vec2 intersection;
                if (line_intersection(A, B, C, D, intersection)) {
                    // Calculate distance to the centroid of rect1
                    vec2 centroid1 = (rect1[0] + rect1[1] + rect1[2] + rect1[3]) / 4.0;
                    double dist = (intersection - centroid1).cross(intersection - centroid1);

                    if (dist < min_dist) {
                        min_dist = dist;
                        closest_intersection = intersection;
                    }
                }
            }
        }

        return closest_intersection;
    }

    bool rect_rect_collision(RigidBody2D* self, RigidBody2D* other, vec2* point_of_contact){
        if(self->moveable == false && other->moveable == false) return false;
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

        vec2 point = find_collision_point(*rectSelf, *rectOther);
        *point_of_contact = point;


        return true;

    }

    bool collides(RigidBody2D* self, RigidBody2D* other, vec2* point_of_contact){
        
        if(self->shape->type == ST_CIRC){
            if(other->shape->type == ST_CIRC){
                // circle circle collision
                return circle_circle_collision(self,other,point_of_contact);
            }else if(other->shape->type == ST_RECT){
                // circle rect coll
                return circle_rect_collision(self,other,point_of_contact);
            }else{}
        }else if(self->shape->type == ST_RECT){
            if(other->shape->type == ST_RECT){
                // rect rect collision
                return rect_rect_collision(self,other,point_of_contact);
            }else if(other->shape->type == ST_CIRC){
                // rect circ coll
                return circle_rect_collision(other,self,point_of_contact);
            }else{}
        }
        return 0;
    }

    RigidBody2D* check_collisions(RigidBody2D* self, vec2* point_of_contact){
        for(auto& rb : bodies){
            if(rb.get() == self) continue;
            
            if(collides(self, rb.get(), point_of_contact)) {
                //SDL_RenderDrawLine(renderer, self->shape->center.x,self->shape->center.y, point_of_contact->x, point_of_contact->y);
                return rb.get();
            }

        }
        return NULL;     
    }

    void circle_circle_solver(RigidBody2D* self, RigidBody2D* other, vec2 point_of_contact){
        Circle* circleSelf = static_cast<Circle*>(self->shape.get());
        Circle* circleOther = static_cast<Circle*>(other->shape.get());

        vec2 collision_normal = (circleOther->center - circleSelf->center).normalized();

        double distance = (circleOther->center - circleSelf->center).mag();
        double depth = circleSelf->radius + circleOther->radius - distance;

        if(depth > 0){
            vec2 correction = collision_normal * (depth / 2.0);
            circleSelf->center -= correction;
            circleOther->center += correction;
        }

        vec2 relative_velocity = other->velocity - self->velocity;
        double v_normal = relative_velocity.dot(collision_normal);

        if(v_normal > 0) return;

        // Coefficient of restitution (e) 
        double e = 0.7;  // can be between 0 (inelastic) and 1 (elastic)

        // Calculate the impulse scalar
        double j = -(1 + e) * v_normal / (1 / self->mass + 1 / other->mass);

        // 5. Apply the impulse to both circles
        vec2 impulse = collision_normal * j;
        self->velocity -= impulse / self->mass;
        other->velocity += impulse / other->mass;

        
    }
    void circle_rect_solver(RigidBody2D* self, RigidBody2D* other, vec2 point_of_contact){
        Circle* circleSelf = static_cast<Circle*>(self->shape.get());
        Rect* rectOther = static_cast<Rect*>(other->shape.get());
        
        vec2 collision_normal = (circleSelf->center - point_of_contact).normalized();
        double depth = circleSelf->radius - (circleSelf->center - point_of_contact).mag();

        circleSelf->center += collision_normal * depth;

        vec2 relativ_velocity = self->velocity;
        double v_normal = relativ_velocity.dot(collision_normal);

        if(v_normal > 0) return;

        double e = 0.7; // coeffizient of restitution 0-1

        double j = -(1 + e) * v_normal / (1 / self->mass);

        self->velocity += collision_normal * (j / self->mass);

        SDL_RenderDrawLine(renderer, circleSelf->center.x, circleSelf->center.y, point_of_contact.x, point_of_contact.y);

        // 5. Apply the impulse to linear velocities
        vec2 impulse = collision_normal * j;
        
        //other->velocity -= impulse / other->mass;

        vec2 r = point_of_contact - rectOther->center;

        // 6. Apply the impulse to angular velocity of the rectangle
        double torque = r.cross(impulse);  // Torque is r cross F
        self->angular_velocity -= torque / self->MOI;

        


    }
    void rect_rect_solver(RigidBody2D* self, RigidBody2D* other, vec2 point_of_contact){
        Rect* rectSelf = static_cast<Rect*>(self->shape.get());
        Rect* rectOther = static_cast<Rect*>(other->shape.get());
    }
    // void circle_rect_solver(RigidBody2D* self, RigidBody2D* other, vec2 point_of_contact){
    //     Circle* circleSelf = static_cast<Circle*>(self->shape.get());
    //     Rect* rectOther = static_cast<Rect*>(other->shape.get());
    // }

    void solve_contraints(RigidBody2D* self, RigidBody2D* other, vec2 point_of_contact){
        // printf("collision happend between %p and %p\n",self,collider);
        // self->moveable = false;
        // collider->moveable = false;
        if(self->shape->type == ST_CIRC){
            if(other->shape->type == ST_CIRC){
                // circle circle collision
                return circle_circle_solver(self,other,point_of_contact);
            }else if(other->shape->type == ST_RECT){
                // circle rect coll
                return circle_rect_solver(self,other,point_of_contact);
            }else{}
        }else if(self->shape->type == ST_RECT){
            if(other->shape->type == ST_RECT){
                // rect rect collision
                return rect_rect_solver(self,other,point_of_contact);
            }else if(other->shape->type == ST_CIRC){
                // rect circ coll
                return circle_rect_solver(other,self,point_of_contact);
            }else{}
        }
        

        
        return;
    }

    void step(){
        // // apply forces 
        // apply_force(vec2(0,9.81)); // only gravity for now
        // // update position and velocities
        // update(dt);
        // // detect collision
        // RigidBody2D* collider = check_collision(sim.bodies);
        // if(collider){
        //     // solve contraints
        //     solve_contraints(collider);
        // }
        for(auto& rb : bodies){
            // apply forces 
            rb->apply_force(vec2(0,9.81));
            // update position and velocities
            rb->update(dt);
            // detect collision
            vec2 point_of_contact;
            RigidBody2D* collider = check_collisions(rb.get(), &point_of_contact);
            if(collider){
                // solve contraints
                solve_contraints(rb.get(), collider, point_of_contact);
            }
            
        }
    }

    void draw(SDL_Renderer* renderer){
        for(const auto& rb : bodies){
            rb->draw(renderer);
        }
    }


};
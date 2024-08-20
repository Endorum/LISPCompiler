Circle* circle = static_cast<Circle*>(circleBody->shape.get());
Rect* rect = static_cast<Rect*>(rectBody->shape.get());

// 1. Calculate the collision normal
vec2 collision_normal = (point_of_contact - circle->center).normalized();

// 2. Calculate relative velocity at the point of contact
vec2 r = point_of_contact - rect->center;
vec2 rect_velocity_at_contact = rectBody->velocity +  r.perpendicular() * rectBody->angular_velocity; 
vec2 relative_velocity = (circleBody->velocity - rect_velocity_at_contact);

// 3. Calculate the relative velocity in terms of the collision normal
double v_normal = relative_velocity.dot(collision_normal);

// Skip if they are moving apart
if (v_normal > 0) return;

// 4. Calculate the impulse scalar
double e = 0.7;  // Coefficient of restitution, can be adjusted
double j = -(1 + e) * v_normal;
j /= (1 / circleBody->mass) + (1 / rectBody->mass) + pow((r.cross(collision_normal) / rectBody->MOI), 2);

// 5. Apply the impulse to linear velocities
vec2 impulse = collision_normal * j;
circleBody->velocity += impulse / circleBody->mass;
rectBody->velocity -= impulse / rectBody->mass;

// 6. Apply the impulse to angular velocity of the rectangle
double torque = r.cross(impulse);  // Torque is r cross F
rectBody->angular_velocity -= torque / rectBody->MOI;


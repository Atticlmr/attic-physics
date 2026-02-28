#pragma once
#include "math/vec3.h"
#include "math/quat.h"

namespace atp {

struct Transform {
    Vec3 position;
    Quat rotation;

    Transform() : position(), rotation() {}
    Transform(const Vec3& pos, const Quat& rot) : position(pos), rotation(rot) {}

    Vec3 transform_point(const Vec3& local_point) const;
    Vec3 transform_vector(const Vec3& local_vec) const;
    Vec3 inverse_transform_point(const Vec3& world_point) const;
    Vec3 inverse_transform_vector(const Vec3& world_vec) const;

    static Transform identity();
    static Transform from_position(const Vec3& pos);
    static Transform from_rotation(const Quat& rot);
};

Transform operator*(const Transform& a, const Transform& b);
Transform inverse(const Transform& t);

} // namespace atp

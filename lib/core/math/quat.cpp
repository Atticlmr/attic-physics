#include "attic/core/math/math.h"

namespace attic {
namespace math {

// Implementation of Quat::from_mat3 if needed
Quat Quat::from_mat3(const Mat3& mat) {
    // Convert rotation matrix to quaternion
    float trace = mat.m[0][0] + mat.m[1][1] + mat.m[2][2];

    if (trace > 0.0f) {
        float s = std::sqrt(trace + 1.0f) * 2.0f;
        float inv_s = 1.0f / s;
        return Quat((mat.m[2][1] - mat.m[1][2]) * inv_s, (mat.m[0][2] - mat.m[2][0]) * inv_s,
                    (mat.m[1][0] - mat.m[0][1]) * inv_s, s * 0.25f);
    } else if (mat.m[0][0] > mat.m[1][1] && mat.m[0][0] > mat.m[2][2]) {
        float s = std::sqrt(1.0f + mat.m[0][0] - mat.m[1][1] - mat.m[2][2]) * 2.0f;
        float inv_s = 1.0f / s;
        return Quat(s * 0.25f, (mat.m[0][1] + mat.m[1][0]) * inv_s,
                    (mat.m[0][2] + mat.m[2][0]) * inv_s, (mat.m[2][1] - mat.m[1][2]) * inv_s);
    } else if (mat.m[1][1] > mat.m[2][2]) {
        float s = std::sqrt(1.0f + mat.m[1][1] - mat.m[0][0] - mat.m[2][2]) * 2.0f;
        float inv_s = 1.0f / s;
        return Quat((mat.m[0][1] + mat.m[1][0]) * inv_s, s * 0.25f,
                    (mat.m[1][2] + mat.m[2][1]) * inv_s, (mat.m[0][2] - mat.m[2][0]) * inv_s);
    } else {
        float s = std::sqrt(1.0f + mat.m[2][2] - mat.m[0][0] - mat.m[1][1]) * 2.0f;
        float inv_s = 1.0f / s;
        return Quat((mat.m[0][2] + mat.m[2][0]) * inv_s, (mat.m[1][2] + mat.m[2][1]) * inv_s,
                    s * 0.25f, (mat.m[1][0] - mat.m[0][1]) * inv_s);
    }
}

} // namespace math
} // namespace attic

#include "attic/core/math/math.h"

#include <iostream>

int main() {
    std::cout << "=== Vec3 测试开始 ===" << std::endl;

    // 测试1：构造
    attic::math::Vec3 v1;
    attic::math::Vec3 v2(1.0f, 2.0f, 3.0f);
    attic::math::Vec3 v3(4.0f, 5.0f, 6.0f);

    std::cout << "v1 = (" << v1.x << ", " << v1.y << ", " << v1.z << ")" << std::endl;
    std::cout << "v2 = (" << v2.x << ", " << v2.y << ", " << v2.z << ")" << std::endl;

    // 测试2：加法
    attic::math::Vec3 v4 = v2 + v3;
    std::cout << "v2 + v3 = (" << v4.x << ", " << v4.y << ", " << v4.z << ")" << std::endl;

    // 测试3：减法
    attic::math::Vec3 v5 = v3 - v2;
    std::cout << "v3 - v2 = (" << v5.x << ", " << v5.y << ", " << v5.z << ")" << std::endl;

    // 测试4：数乘
    attic::math::Vec3 v6 = v2 * 2.0f;
    std::cout << "v2 * 2 = (" << v6.x << ", " << v6.y << ", " << v6.z << ")" << std::endl;

    // 测试5：反向数乘
    attic::math::Vec3 v7 = 3.0f * v2;
    std::cout << "3 * v2 = (" << v7.x << ", " << v7.y << ", " << v7.z << ")" << std::endl;

    // 测试6：点乘
    float d = attic::math::dot(v2, v3);
    std::cout << "dot(v2, v3) = " << d << " (预期: 32)" << std::endl;

    // 测试7：叉乘
    attic::math::Vec3 x_axis(1, 0, 0);
    attic::math::Vec3 y_axis(0, 1, 0);
    attic::math::Vec3 z_axis = attic::math::cross(x_axis, y_axis);
    std::cout << "X x Y = (" << z_axis.x << ", " << z_axis.y << ", " << z_axis.z << ")"
              << std::endl;

    // 测试8：长度
    attic::math::Vec3 v8(3.0f, 4.0f, 0.0f);
    std::cout << "|v8| = " << v8.length() << " (预期: 5)" << std::endl;

    // 测试9：归一化
    attic::math::Vec3 v9 = v8.normalized();
    std::cout << "normalized v8 length = " << v9.length() << " (预期: 1)" << std::endl;

    // 测试10：+=
    attic::math::Vec3 v10(1, 1, 1);
    v10 += attic::math::Vec3(2, 3, 4);
    std::cout << "v10 after += = (" << v10.x << ", " << v10.y << ", " << v10.z << ")" << std::endl;

    std::cout << "=== 所有测试完成 ===" << std::endl;
    return 0;
}

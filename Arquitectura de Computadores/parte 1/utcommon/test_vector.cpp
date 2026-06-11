// utcommon/test_vector.cpp
#include <gtest/gtest.h>

#include "vector.hpp"  // render::vector

using render::vector;

namespace { // ----------------------- helpers ---------------------------------
void ExpectVecNear(const vector& a, const vector& b, double eps = 1e-12) {
    EXPECT_NEAR(a.x, b.x, eps);
    EXPECT_NEAR(a.y, b.y, eps);
    EXPECT_NEAR(a.z, b.z, eps);
}
} // namespace

// -------------------------- constexpr sanity checks --------------------------
// Operadores y funciones constexpr: +, -, *, /, dot, cross, length_squared, -v.
static_assert([]{
    constexpr vector v{1.0, 2.0, 3.0};
    constexpr vector w{4.0, 5.0, 6.0};
    constexpr vector s = v + w;                      // (5,7,9)
    static_assert(s.x == 5.0 and s.y == 7.0 and s.z == 9.0);

    constexpr vector d = w - v;                      // (3,3,3)
    static_assert(d.x == 3.0 and d.y == 3.0 and d.z == 3.0);

    constexpr vector m = v * 2.0;                    // (2,4,6)
    static_assert(m.x == 2.0 and m.y == 4.0 and m.z == 6.0);

    constexpr vector q = w / 2.0;                    // (2,2.5,3)
    static_assert(q.x == 2.0 and q.y == 2.5 and q.z == 3.0);

    constexpr double ls = v.length_squared();        // 1+4+9 = 14
    static_assert(ls == 14.0);

    constexpr double dt = v.dot(vector{2.0, 0.0, 0.0}); // 2
    static_assert(dt == 2.0);

    constexpr vector cr = vector{1,0,0}.cross(vector{0,1,0}); // (0,0,1)
    static_assert(cr.x == 0.0 and cr.y == 0.0 and cr.z == 1.0);

    constexpr vector neg = -v;                       // (-1,-2,-3)
    static_assert(neg.x == -1.0 and neg.y == -2.0 and neg.z == -3.0);

    // Escalar * vector (libre)
    constexpr vector sv = 3.0 * vector{1.0, -2.0, 0.5}; // (3,-6,1.5)
    static_assert(sv.x == 3.0 and sv.y == -6.0 and sv.z == 1.5);
    return true;
}());

// --------------------------------- Constructores -----------------------------

TEST(Vector, DefaultCtorIsZero) {
    const vector v;
    EXPECT_DOUBLE_EQ(v.x, 0.0);
    EXPECT_DOUBLE_EQ(v.y, 0.0);
    EXPECT_DOUBLE_EQ(v.z, 0.0);
}

TEST(Vector, DirectCtorStoresComponents) {
    const vector v{1.25, -2.5, 0.0};
    EXPECT_DOUBLE_EQ(v.x, 1.25);
    EXPECT_DOUBLE_EQ(v.y, -2.5);
    EXPECT_DOUBLE_EQ(v.z, 0.0);
}

// ------------------------------ Magnitud / norma ----------------------------

TEST(Vector, LengthSquaredAndMagnitude) {
    const vector v{3.0, 4.0, 12.0};        // ||v||^2 = 9 + 16 + 144 = 169; ||v|| = 13
    EXPECT_DOUBLE_EQ(v.length_squared(), 169.0);
    EXPECT_DOUBLE_EQ(v.magnitude(), 13.0);
}

TEST(Vector, NormalizeUnitVector) {
    const vector v{0.0, 0.0, 5.0};
    const vector u = v.normalize();
    ExpectVecNear(u, vector{0.0, 0.0, 1.0});
}

TEST(Vector, NormalizeDegenerateReturnsZero) {
    const vector tiny{1e-16, 0.0, 0.0};   // por debajo del eps por defecto (1e-12)
    const vector u = tiny.normalize();
    ExpectVecNear(u, vector{0.0, 0.0, 0.0});
}

// --------------------------------- Aritmética --------------------------------

TEST(Vector, BinaryArithmeticOperators) {
    const vector a{1.0, 2.0, 3.0};
    const vector b{-2.0, 0.5, 1.0};

    ExpectVecNear(a + b, vector{-1.0, 2.5, 4.0});
    ExpectVecNear(a - b, vector{ 3.0, 1.5, 2.0});
    ExpectVecNear(a * 2.0, vector{2.0, 4.0, 6.0});
    ExpectVecNear(a / 2.0, vector{0.5, 1.0, 1.5});

    // Unary minus
    ExpectVecNear(-a, vector{-1.0, -2.0, -3.0});
}

TEST(Vector, CompoundAssignmentOperators) {
    vector v{1.0, 2.0, 3.0};
    const vector b{0.5, -1.0, 2.0};

    v += b;  ExpectVecNear(v, vector{1.5, 1.0, 5.0});
    v -= b;  ExpectVecNear(v, vector{1.0, 2.0, 3.0});
    v *= 2.0;ExpectVecNear(v, vector{2.0, 4.0, 6.0});
    v /= 2.0;ExpectVecNear(v, vector{1.0, 2.0, 3.0});
}

TEST(Vector, ScalarTimesVectorFreeFunction) {
    const vector v{1.0, -2.0, 0.5};
    ExpectVecNear(2.0 * v, vector{2.0, -4.0, 1.0});
}

// --------------------------------- dot / cross -------------------------------

TEST(Vector, DotProductBasicAndOrthogonality) {
    const vector x{1.0, 0.0, 0.0};
    const vector y{0.0, 1.0, 0.0};
    const vector v{1.0, 2.0, 3.0};

    EXPECT_DOUBLE_EQ(x.dot(x), 1.0);
    EXPECT_DOUBLE_EQ(x.dot(y), 0.0);  // ortogonales
    EXPECT_DOUBLE_EQ(v.dot(vector{2.0, 0.0, -1.0}), 2.0 + 0.0 - 3.0); // = -1
}

TEST(Vector, CrossProductRightHandRuleAndParallelCase) {
    const vector x{1.0, 0.0, 0.0};
    const vector y{0.0, 1.0, 0.0};
    const vector z{0.0, 0.0, 1.0};

    ExpectVecNear(x.cross(y), z);              // mano derecha
    ExpectVecNear(y.cross(x), -z);             // anti conmutatividad
    ExpectVecNear(x.cross(x), vector{0.0,0.0,0.0}); // paralelos → cero

    // Ortogonalidad: (x×y)·x = (x×y)·y = 0
    const vector c = x.cross(y);
    EXPECT_DOUBLE_EQ(c.dot(x), 0.0);
    EXPECT_DOUBLE_EQ(c.dot(y), 0.0);
}

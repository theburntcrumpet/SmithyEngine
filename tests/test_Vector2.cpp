#include <catch2/catch_test_macros.hpp>
#include "engine/Vector2.hpp"

TEST_CASE("Vector2 default constructor initializes to zero", "[Vector2]") {
    Engine::Vector2<int> v;
    REQUIRE(v.GetX() == 0);
    REQUIRE(v.GetY() == 0);
}

TEST_CASE("Vector2 single-value constructor", "[Vector2]") {
    Engine::Vector2<int> v(5);
    REQUIRE(v.GetX() == 5);
    REQUIRE(v.GetY() == 5);
}

TEST_CASE("Vector2 two-value constructor", "[Vector2]") {
    Engine::Vector2<int> v(3, 7);
    REQUIRE(v.GetX() == 3);
    REQUIRE(v.GetY() == 7);
}

TEST_CASE("Vector2 addition", "[Vector2]") {
    Engine::Vector2<int> a(3, 4);
    Engine::Vector2<int> b(1, 2);

    SECTION("Vector + Vector") {
        auto result = a + b;
        REQUIRE(result.GetX() == 4);
        REQUIRE(result.GetY() == 6);
    }

    SECTION("Vector + Scalar") {
        auto result = a + 10;
        REQUIRE(result.GetX() == 13);
        REQUIRE(result.GetY() == 14);
    }

    SECTION("Vector += Vector") {
        a += b;
        REQUIRE(a.GetX() == 4);
        REQUIRE(a.GetY() == 6);
    }
}

TEST_CASE("Vector2 subtraction", "[Vector2]") {
    Engine::Vector2<int> a(10, 8);
    Engine::Vector2<int> b(3, 2);

    SECTION("Vector - Vector") {
        auto result = a - b;
        REQUIRE(result.GetX() == 7);
        REQUIRE(result.GetY() == 6);
    }

    SECTION("Vector - Scalar") {
        auto result = a - 5;
        REQUIRE(result.GetX() == 5);
        REQUIRE(result.GetY() == 3);
    }

    SECTION("Vector -= Vector") {
        a -= b;
        REQUIRE(a.GetX() == 7);
        REQUIRE(a.GetY() == 6);
    }
}

TEST_CASE("Vector2 multiplication", "[Vector2]") {
    Engine::Vector2<int> a(3, 4);
    Engine::Vector2<int> b(2, 5);

    SECTION("Vector * Vector (component-wise)") {
        auto result = a * b;
        REQUIRE(result.GetX() == 6);
        REQUIRE(result.GetY() == 20);
    }

    SECTION("Vector * Scalar") {
        auto result = a * 3;
        REQUIRE(result.GetX() == 9);
        REQUIRE(result.GetY() == 12);
    }

    SECTION("Vector *= Vector") {
        a *= b;
        REQUIRE(a.GetX() == 6);
        REQUIRE(a.GetY() == 20);
    }
}

TEST_CASE("Vector2 setters", "[Vector2]") {
    Engine::Vector2<int> v(0, 0);

    SECTION("Set both values") {
        v.Set(5, 10);
        REQUIRE(v.GetX() == 5);
        REQUIRE(v.GetY() == 10);
    }

    SECTION("SetX") {
        v.SetX(42);
        REQUIRE(v.GetX() == 42);
        REQUIRE(v.GetY() == 0);
    }

    SECTION("SetY") {
        v.SetY(99);
        REQUIRE(v.GetX() == 0);
        REQUIRE(v.GetY() == 99);
    }
}

TEST_CASE("Vector2 ToString", "[Vector2]") {
    Engine::Vector2<int> v(3, 7);
    REQUIRE(v.ToString() == "(3,7)");
}

TEST_CASE("Vector2 works with float", "[Vector2]") {
    Engine::Vector2<float> v(1.5f, 2.5f);
    auto result = v * 2.0f;
    REQUIRE(result.GetX() == 3.0f);
    REQUIRE(result.GetY() == 5.0f);
}

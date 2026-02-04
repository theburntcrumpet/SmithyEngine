#include <catch2/catch_test_macros.hpp>
#include "engine/CollisionRectangle.hpp"
#include "engine/Rectangle.hpp"
#include "engine/Vector2.hpp"

TEST_CASE("CollisionRectangle overlaps", "[CollisionRectangle]"){
    Engine::Rectangle<int> rectangleA(Engine::Vector2<int>(1), Engine::Vector2<int>(3));
    Engine::Rectangle<int> rectangleB(Engine::Vector2<int>(2), Engine::Vector2<int>(3));
    Engine::CollisionRectangle<int> collisionRectangleA(&rectangleA);
    Engine::CollisionRectangle<int> collisionRectangleB(&rectangleB);
    REQUIRE(collisionRectangleA.IsColliding(collisionRectangleB) == true);
    REQUIRE(collisionRectangleB.IsColliding(collisionRectangleA) == true);

    // Rectangle moves out of range
    rectangleA.SetPosition(Engine::Vector2<int>(8));
    REQUIRE(collisionRectangleA.IsColliding(collisionRectangleB) == false );
}

TEST_CASE("CollisionRectangle doesn't overlap", "[CollisionRectangle]") {
    Engine::Rectangle<int> rectangleA(Engine::Vector2<int>(1),Engine::Vector2<int>(2));
    Engine::Rectangle<int> rectangleB(Engine::Vector2<int>(3),Engine::Vector2<int>(2));
    Engine::CollisionRectangle<int> collisionRectangleA(&rectangleA);
    Engine::CollisionRectangle<int> collisionRectangleB(&rectangleB);
    REQUIRE(collisionRectangleA.IsColliding(collisionRectangleB) == false);
    
    // Rectangle moves into range
    rectangleB.SetPosition(Engine::Vector2(2));
    REQUIRE(collisionRectangleA.IsColliding(collisionRectangleB) == true);
}

TEST_CASE("CollisionRectangle checks collision on null Rectangle","[CollisionRectangle]") {
    Engine::CollisionRectangle<int> collisionRectangle(nullptr);
    Engine::CollisionRectangle<int> otherCollisionRectangle(nullptr);
    REQUIRE(collisionRectangle.IsColliding(otherCollisionRectangle) == false);
}
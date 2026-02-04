#include <catch2/catch_test_macros.hpp>
#include "engine/Input.hpp"

TEST_CASE("InputAction default state", "[Input]") {
    Engine::InputAction action;
    REQUIRE(action.GetIsPressed() == false);
    REQUIRE(action.GetJustPressed() == false);
    REQUIRE(action.GetJustReleased() == false);
}

TEST_CASE("InputAction state changes", "[Input]") {
    Engine::InputAction action;

    SECTION("Press action") {
        action.SetIsPressed(true);
        action.SetJustPressed(true);
        REQUIRE(action.GetIsPressed() == true);
        REQUIRE(action.GetJustPressed() == true);
        REQUIRE(action.GetJustReleased() == false);
    }

    SECTION("Release action") {
        action.SetIsPressed(true);
        action.SetIsPressed(false);
        action.SetJustReleased(true);
        REQUIRE(action.GetIsPressed() == false);
        REQUIRE(action.GetJustReleased() == true);
    }
}

TEST_CASE("Input default state", "[Input]") {
    Engine::Input input;

    SECTION("Direction starts at zero") {
        auto dir = input.GetDirection();
        REQUIRE(dir.GetX() == 0.0f);
        REQUIRE(dir.GetY() == 0.0f);
    }

    SECTION("Actions start unpressed") {
        REQUIRE(input.GetPrimary().GetIsPressed() == false);
        REQUIRE(input.GetSecondary().GetIsPressed() == false);
        REQUIRE(input.GetStart().GetIsPressed() == false);
        REQUIRE(input.GetSelect().GetIsPressed() == false);
    }
}

TEST_CASE("Input direction can be set", "[Input]") {
    Engine::Input input;
    input.SetDirection(Engine::Vector2<float>(1.0f, -1.0f));

    auto dir = input.GetDirection();
    REQUIRE(dir.GetX() == 1.0f);
    REQUIRE(dir.GetY() == -1.0f);
}

TEST_CASE("Input actions can be modified via reference", "[Input]") {
    Engine::Input input;

    input.GetPrimary().SetIsPressed(true);
    input.GetPrimary().SetJustPressed(true);

    REQUIRE(input.GetPrimary().GetIsPressed() == true);
    REQUIRE(input.GetPrimary().GetJustPressed() == true);

    // Other actions should be unaffected
    REQUIRE(input.GetSecondary().GetIsPressed() == false);
}

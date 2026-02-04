#include <catch2/catch_test_macros.hpp>
#include "engine/Camera.hpp"

TEST_CASE("Camera default constructor", "[Camera]") {
    Engine::Camera cam;
    REQUIRE(cam.GetPosition().GetX() == 0.0f);
    REQUIRE(cam.GetPosition().GetY() == 0.0f);
    REQUIRE(cam.GetSize().GetX() == 0.0f);
    REQUIRE(cam.GetSize().GetY() == 0.0f);
}

TEST_CASE("Camera viewport constructor", "[Camera]") {
    Engine::Camera cam(320.0f, 180.0f);
    REQUIRE(cam.GetPosition().GetX() == 0.0f);
    REQUIRE(cam.GetPosition().GetY() == 0.0f);
    REQUIRE(cam.GetSize().GetX() == 320.0f);
    REQUIRE(cam.GetSize().GetY() == 180.0f);
}

TEST_CASE("Camera position", "[Camera]") {
    Engine::Camera cam(320.0f, 180.0f);

    SECTION("SetPosition with Vector2") {
        cam.SetPosition(Engine::Vector2<float>(100.0f, 50.0f));
        REQUIRE(cam.GetPosition().GetX() == 100.0f);
        REQUIRE(cam.GetPosition().GetY() == 50.0f);
    }

    SECTION("SetPosition with floats") {
        cam.SetPosition(200.0f, 75.0f);
        REQUIRE(cam.GetPosition().GetX() == 200.0f);
        REQUIRE(cam.GetPosition().GetY() == 75.0f);
    }
}

TEST_CASE("Camera WorldToScreen conversion", "[Camera]") {
    Engine::Camera cam(320.0f, 180.0f);
    cam.SetPosition(100.0f, 50.0f);

    SECTION("With Vector2") {
        auto screenPos = cam.WorldToScreen(Engine::Vector2<float>(150.0f, 80.0f));
        REQUIRE(screenPos.GetX() == 50.0f);
        REQUIRE(screenPos.GetY() == 30.0f);
    }

    SECTION("With floats") {
        auto screenPos = cam.WorldToScreen(150.0f, 80.0f);
        REQUIRE(screenPos.GetX() == 50.0f);
        REQUIRE(screenPos.GetY() == 30.0f);
    }
}

TEST_CASE("Camera ScreenToWorld conversion", "[Camera]") {
    Engine::Camera cam(320.0f, 180.0f);
    cam.SetPosition(100.0f, 50.0f);

    auto worldPos = cam.ScreenToWorld(Engine::Vector2<float>(50.0f, 30.0f));
    REQUIRE(worldPos.GetX() == 150.0f);
    REQUIRE(worldPos.GetY() == 80.0f);
}

TEST_CASE("Camera CenterOn", "[Camera]") {
    Engine::Camera cam(320.0f, 180.0f);

    SECTION("With Vector2") {
        cam.CenterOn(Engine::Vector2<float>(200.0f, 100.0f));
        // Camera position should be (200 - 160, 100 - 90) = (40, 10)
        REQUIRE(cam.GetPosition().GetX() == 40.0f);
        REQUIRE(cam.GetPosition().GetY() == 10.0f);
    }

    SECTION("With floats") {
        cam.CenterOn(200.0f, 100.0f);
        REQUIRE(cam.GetPosition().GetX() == 40.0f);
        REQUIRE(cam.GetPosition().GetY() == 10.0f);
    }
}

TEST_CASE("Camera FollowWithDeadzone", "[Camera]") {
    Engine::Camera cam(320.0f, 180.0f);
    cam.SetPosition(0.0f, 0.0f);

    SECTION("No movement when target inside deadzone") {
        // Target at center of screen (160, 90) - well inside 64px deadzone
        cam.FollowWithDeadzone(Engine::Vector2<float>(160.0f, 90.0f), 64.0f);
        REQUIRE(cam.GetPosition().GetX() == 0.0f);
        REQUIRE(cam.GetPosition().GetY() == 0.0f);
    }

    SECTION("Camera moves when target exceeds right edge") {
        // Target at screen x=300, deadzone=64, so right edge is at 320-64=256
        // Target is 44 pixels past the edge, camera should move right by 44
        cam.FollowWithDeadzone(Engine::Vector2<float>(300.0f, 90.0f), 64.0f);
        REQUIRE(cam.GetPosition().GetX() == 44.0f);
        REQUIRE(cam.GetPosition().GetY() == 0.0f);
    }

    SECTION("Camera moves when target exceeds left edge") {
        cam.SetPosition(100.0f, 0.0f);  // Camera looking at world x=100
        // Target at world x=120, screen x=20, deadzone=64
        // Target is 44 pixels inside left edge, camera should move left by 44
        cam.FollowWithDeadzone(Engine::Vector2<float>(120.0f, 90.0f), 64.0f);
        REQUIRE(cam.GetPosition().GetX() == 56.0f);  // 100 - 44 = 56
    }

    SECTION("Camera moves when target exceeds bottom edge") {
        // Target at screen y=150, deadzone=64, so bottom edge is at 180-64=116
        // Target is 34 pixels past the edge
        cam.FollowWithDeadzone(Engine::Vector2<float>(160.0f, 150.0f), 64.0f);
        REQUIRE(cam.GetPosition().GetY() == 34.0f);
    }
}

TEST_CASE("Camera bounds", "[Camera]") {
    Engine::Camera cam(320.0f, 180.0f);

    REQUIRE(cam.HasBounds() == false);

    // Set world bounds (0,0) to (640, 360)
    cam.SetBounds(0.0f, 0.0f, 640.0f, 360.0f);
    REQUIRE(cam.HasBounds() == true);

    SECTION("Position clamped to min bounds") {
        cam.SetPosition(-50.0f, -50.0f);
        REQUIRE(cam.GetPosition().GetX() == 0.0f);
        REQUIRE(cam.GetPosition().GetY() == 0.0f);
    }

    SECTION("Position clamped to max bounds") {
        // Max camera position is (640-320, 360-180) = (320, 180)
        cam.SetPosition(500.0f, 300.0f);
        REQUIRE(cam.GetPosition().GetX() == 320.0f);
        REQUIRE(cam.GetPosition().GetY() == 180.0f);
    }

    SECTION("CenterOn respects bounds") {
        // Try to center on (0, 0) - would put camera at (-160, -90)
        cam.CenterOn(0.0f, 0.0f);
        REQUIRE(cam.GetPosition().GetX() == 0.0f);
        REQUIRE(cam.GetPosition().GetY() == 0.0f);
    }

    SECTION("ClearBounds removes constraints") {
        cam.ClearBounds();
        REQUIRE(cam.HasBounds() == false);
        cam.SetPosition(-50.0f, -50.0f);
        REQUIRE(cam.GetPosition().GetX() == -50.0f);
        REQUIRE(cam.GetPosition().GetY() == -50.0f);
    }
}

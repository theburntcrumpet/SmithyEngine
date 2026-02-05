#ifndef CAMERA_H
#define CAMERA_H
#include "engine/Vector2.hpp"
#include <cmath>
namespace Engine {
    class Camera {
        private:
            Vector2<float> m_position;
            Vector2<float> m_size;

            // Optional bounds to constrain camera movement
            bool m_hasBounds = false;
            Vector2<float> m_boundsMin;
            Vector2<float> m_boundsMax;

        public:
            Camera() : m_position(0.0f, 0.0f), m_size(0.0f, 0.0f) {}

            Camera(float viewportWidth, float viewportHeight)
                : m_position(0.0f, 0.0f), m_size(viewportWidth, viewportHeight) {}

            // Position
            const Vector2<float>& GetPosition() const { return m_position; }
            void SetPosition(const Vector2<float>& position) {
                m_position = position;
                ClampToBounds();
            }
            void SetPosition(float x, float y) {
                m_position.Set(x, y);
                ClampToBounds();
            }

            // Viewport size
            const Vector2<float>& GetSize() const { return m_size; }
            void SetSize(float width, float height) { m_size.Set(width, height); }

            // Convert world coordinates to screen coordinates
            // This is what you subtract from object positions when rendering
            Vector2<float> WorldToScreen(const Vector2<float>& worldPos) const {
                return worldPos - m_position;
            }

            Vector2<float> WorldToScreen(float worldX, float worldY) const {
                return Vector2<float>(worldX - m_position.GetX(), worldY - m_position.GetY());
            }

            // Convert screen coordinates back to world coordinates
            Vector2<float> ScreenToWorld(const Vector2<float>& screenPos) const {
                return screenPos + m_position;
            }

            // Center the camera on a world position
            void CenterOn(const Vector2<float>& worldPos) {
                m_position.Set(
                    worldPos.GetX() - m_size.GetX() / 2.0f,
                    worldPos.GetY() - m_size.GetY() / 2.0f
                );
                ClampToBounds();
            }

            void CenterOn(float worldX, float worldY) {
                m_position.Set(
                    worldX - m_size.GetX() / 2.0f,
                    worldY - m_size.GetY() / 2.0f
                );
                ClampToBounds();
            }

            // Smoothly move camera towards a target position
            // smoothing: 0.0 = instant, higher = slower (typical values: 0.1 - 0.9)
            void FollowSmooth(const Vector2<float>& targetWorldPos, float smoothing, float deltaTime) {
                Vector2<float> targetCamPos(
                    targetWorldPos.GetX() - m_size.GetX() / 2.0f,
                    targetWorldPos.GetY() - m_size.GetY() / 2.0f
                );

                // Lerp towards target
                float t = 1.0f - smoothing;
                // Scale by deltaTime for frame-rate independence (assuming ~60fps baseline)
                t = 1.0f - static_cast<float>(std::pow(smoothing, deltaTime * 60.0f));

                m_position.Set(
                    m_position.GetX() + (targetCamPos.GetX() - m_position.GetX()) * t,
                    m_position.GetY() + (targetCamPos.GetY() - m_position.GetY()) * t
                );
                ClampToBounds();
            }

            // Follow target only when they reach screen edges (deadzone camera)
            // deadzone: margin from screen edge where camera won't move (e.g., 64 pixels)
            // Target can move freely within the deadzone rectangle in the center
            void FollowWithDeadzone(const Vector2<float>& targetWorldPos, float deadzone) {
                // Convert target to screen position
                Vector2<float> screenPos = WorldToScreen(targetWorldPos);

                // Calculate how far outside the deadzone the target is
                float pushX = 0.0f;
                float pushY = 0.0f;

                // Left edge
                if (screenPos.GetX() < deadzone) {
                    pushX = screenPos.GetX() - deadzone;
                }
                // Right edge
                else if (screenPos.GetX() > m_size.GetX() - deadzone) {
                    pushX = screenPos.GetX() - (m_size.GetX() - deadzone);
                }

                // Top edge
                if (screenPos.GetY() < deadzone) {
                    pushY = screenPos.GetY() - deadzone;
                }
                // Bottom edge
                else if (screenPos.GetY() > m_size.GetY() - deadzone) {
                    pushY = screenPos.GetY() - (m_size.GetY() - deadzone);
                }

                // Move camera by the push amount
                if (pushX != 0.0f || pushY != 0.0f) {
                    m_position.Set(
                        m_position.GetX() + pushX,
                        m_position.GetY() + pushY
                    );
                    ClampToBounds();
                }
            }

            // Smooth deadzone - combines deadzone behavior with smoothing
            void FollowWithDeadzoneSmooth(const Vector2<float>& targetWorldPos, float deadzone, float smoothing, float deltaTime) {
                Vector2<float> screenPos = WorldToScreen(targetWorldPos);

                float targetPushX = 0.0f;
                float targetPushY = 0.0f;

                if (screenPos.GetX() < deadzone) {
                    targetPushX = screenPos.GetX() - deadzone;
                } else if (screenPos.GetX() > m_size.GetX() - deadzone) {
                    targetPushX = screenPos.GetX() - (m_size.GetX() - deadzone);
                }

                if (screenPos.GetY() < deadzone) {
                    targetPushY = screenPos.GetY() - deadzone;
                } else if (screenPos.GetY() > m_size.GetY() - deadzone) {
                    targetPushY = screenPos.GetY() - (m_size.GetY() - deadzone);
                }

                if (targetPushX != 0.0f || targetPushY != 0.0f) {
                    float t = 1.0f - static_cast<float>(std::pow(smoothing, deltaTime * 60.0f));
                    m_position.Set(
                        m_position.GetX() + targetPushX * t,
                        m_position.GetY() + targetPushY * t
                    );
                    ClampToBounds();
                }
            }

            // Set world bounds that the camera cannot move outside of
            void SetBounds(float minX, float minY, float maxX, float maxY) {
                m_hasBounds = true;
                m_boundsMin.Set(minX, minY);
                m_boundsMax.Set(maxX, maxY);
                ClampToBounds();
            }

            void ClearBounds() {
                m_hasBounds = false;
            }

            bool HasBounds() const { return m_hasBounds; }

        private:
            void ClampToBounds() {
                if (!m_hasBounds) return;

                // Clamp so camera viewport stays within bounds
                float maxCamX = m_boundsMax.GetX() - m_size.GetX();
                float maxCamY = m_boundsMax.GetY() - m_size.GetY();

                if (m_position.GetX() < m_boundsMin.GetX()) m_position.SetX(m_boundsMin.GetX());
                if (m_position.GetY() < m_boundsMin.GetY()) m_position.SetY(m_boundsMin.GetY());
                if (m_position.GetX() > maxCamX) m_position.SetX(maxCamX);
                if (m_position.GetY() > maxCamY) m_position.SetY(maxCamY);
            }
    };
}
#endif

#ifndef INPUT_H
#define INPUT_H
#include "engine/Vector2.hpp"
namespace Engine {
    class InputAction {
        private:
            bool m_bIsPressed;
            bool m_bJustPressed;
            bool m_bJustReleased;
        public:
            InputAction() : m_bIsPressed(false), m_bJustPressed(false), m_bJustReleased(false) {}
            ~InputAction(){};
            bool GetIsPressed() const {return m_bIsPressed;}
            bool GetJustPressed() const {return m_bJustPressed;}
            bool GetJustReleased() const { return m_bJustReleased;}
            void SetIsPressed(bool isPressed) {m_bIsPressed = isPressed;}
            void SetJustPressed(bool justPressed) {m_bJustPressed = justPressed; }
            void SetJustReleased(bool justReleased) {m_bJustReleased = justReleased; }

    };
    class Input {
        private:
            Vector2<float> m_direction;
            InputAction m_primary;
            InputAction m_secondary;
            InputAction m_start;
            InputAction m_select;
        public:
            Input(){};
            ~Input(){};

            // Getters (const for reading)
            const Vector2<float>& GetDirection() const { return m_direction; }
            const InputAction& GetPrimary() const { return m_primary; }
            const InputAction& GetSecondary() const { return m_secondary; }
            const InputAction& GetStart() const { return m_start; }
            const InputAction& GetSelect() const { return m_select; }

            // Getters (non-const for modifying)
            InputAction& GetPrimary() { return m_primary; }
            InputAction& GetSecondary() { return m_secondary; }
            InputAction& GetStart() { return m_start; }
            InputAction& GetSelect() { return m_select; }

            // Setters
            void SetDirection(const Vector2<float>& direction) { m_direction = direction; }
    };
}
#endif
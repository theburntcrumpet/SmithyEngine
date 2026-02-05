#ifndef VECTOR2_H
#define VECTOR2_H
#include <sstream>
#include <string>
namespace Engine {
    template<typename T> class Vector2 {
        private:
            T m_nX;
            T m_nY;
        public:
            // Constructors
            Vector2() : m_nX(0), m_nY(0) {}

            Vector2(T n) : m_nX(n), m_nY(n) {}

            Vector2(T x, T y) : m_nX(x), m_nY(y) {}

            // Getters
            T GetX() const { return m_nX; }
            T GetY() const { return m_nY; }

            // Setters
            void Set(T x, T y) { m_nX = x; m_nY = y; }
            void SetX(T x) { m_nX = x; }
            void SetY(T y) { m_nY = y; }

            // Utility
            std::string ToString() const {
                std::stringstream ss;
                ss << "(" << m_nX << "," << m_nY << ")";
                return ss.str();
            }

            // Operators
            Vector2<T> operator+(const Vector2<T>& rhs) const {
                return Vector2<T>(m_nX + rhs.m_nX, m_nY + rhs.m_nY);
            }

            Vector2<T> operator+(T rhs) const {
                return Vector2<T>(m_nX + rhs, m_nY + rhs);
            }

            Vector2<T>& operator+=(const Vector2<T>& rhs) {
                m_nX += rhs.m_nX;
                m_nY += rhs.m_nY;
                return *this;
            }

            Vector2<T> operator-(const Vector2<T>& rhs) const {
                return Vector2<T>(m_nX - rhs.m_nX, m_nY - rhs.m_nY);
            }

            Vector2<T> operator-(T rhs) const {
                return Vector2<T>(m_nX - rhs, m_nY - rhs);
            }

            Vector2<T>& operator-=(const Vector2<T>& rhs) {
                m_nX -= rhs.m_nX;
                m_nY -= rhs.m_nY;
                return *this;
            }

            Vector2<T> operator*(const Vector2<T>& rhs) const {
                return Vector2<T>(m_nX * rhs.m_nX, m_nY * rhs.m_nY);
            }

            Vector2<T> operator*(T rhs) const {
                return Vector2<T>(m_nX * rhs, m_nY * rhs);
            }

            Vector2<T>& operator*=(const Vector2<T>& rhs) {
                m_nX *= rhs.m_nX;
                m_nY *= rhs.m_nY;
                return *this;
            }
    };
    typedef Vector2<int> Vector2i;
    typedef Vector2<unsigned int> Vector2ui;
    typedef Vector2<float> Vector2f;
    typedef Vector2<double> Vector2d;
}
#endif

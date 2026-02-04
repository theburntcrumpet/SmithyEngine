#ifndef RECTANGLE_H
#define RECTANGLE_H
#include "engine/Vector2.hpp"
#include <array>

namespace Engine {
template <typename T> class Rectangle {
private:
  Vector2<T> m_position;
  Vector2<T> m_size;

public:
  Rectangle() = default;

  Rectangle(Vector2<T> position, Vector2<T> size)
        : m_position(position), m_size(size) {}

  std::array<Vector2<T>, 4> GetVertices() const {
    return {
        m_position, // top-left
        Vector2<T>(m_position.GetX() + m_size.GetX(),
                   m_position.GetY()), // top-right
        m_position + m_size,           // bottom-right
        Vector2<T>(m_position.GetX(),
                   m_position.GetY() + m_size.GetY()) // bottom-left
    };
  }

  T GetArea() const {
    return m_size.GetX() * m_size.GetY();
  }

  Vector2<T> GetPosition() const {
    return m_position;
  }

  Vector2<T> GetSize() const {
    return m_size;
  }

  void SetPosition(Vector2<T> position) {
    m_position = position;
  }
  
  void SetSize(Vector2<T> size) {
    m_size = size;
  }

};
} // namespace Engine
#endif
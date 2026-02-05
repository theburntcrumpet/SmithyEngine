#ifndef COLLISION_RECTANGLE_H
#define COLLISION_RECTANGLE_H
#include "engine/Rectangle.hpp"

namespace Engine {
    template <typename T> class CollisionRectangle {
    private:
        Rectangle<T>* m_pRectangle;

    public:
        CollisionRectangle(Rectangle<T>* pRectangle) : m_pRectangle(pRectangle) {}

        Rectangle<T>* GetRectangle() const { return m_pRectangle; }

        bool IsColliding(const CollisionRectangle<T>& other) const {
            if (m_pRectangle == nullptr || other.GetRectangle() == nullptr) return false;

            Vector2<T> thisPos = m_pRectangle->GetPosition();
            Vector2<T> thisSize = m_pRectangle->GetSize();
            Vector2<T> otherPos = other.GetRectangle()->GetPosition();
            Vector2<T> otherSize = other.GetRectangle()->GetSize();

            // AABB collision: rectangles overlap if they intersect on both axes
            return (thisPos.GetX() < otherPos.GetX() + otherSize.GetX() &&
                    thisPos.GetX() + thisSize.GetX() > otherPos.GetX() &&
                    thisPos.GetY() < otherPos.GetY() + otherSize.GetY() &&
                    thisPos.GetY() + thisSize.GetY() > otherPos.GetY());
        }
    };
}
#endif

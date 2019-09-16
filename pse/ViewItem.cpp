#ifndef M_PI
# define M_PI 3.14159265358979323846264338327950288f
#endif
#include "ViewItem.hpp"

namespace pse {
    ViewItem::ViewItem() noexcept
            :
            _pos(0, 0),
            _destination(-1, -1),
            _moveTimer(-1) {}

    void ViewItem::draw(sf::RenderWindow & window) noexcept {
        if (_drawable == nullptr)
            _drawable = &getDrawable();
        window.draw(*_drawable);
    }

    void ViewItem::setPosition(const sf::Vector2f & pos) noexcept {
        if (_transformable == nullptr)
            _transformable = &getTransformable();
        _pos = pos;
        _transformable->setPosition(_pos.x + _size.x / 2, _pos.y + _size.y / 2);
    }

    void ViewItem::setRotation(float radians) noexcept {
        if (_transformable == nullptr)
            _transformable = &getTransformable();
        _transformable->setRotation(radians * 180.f / M_PI);
    }

    void ViewItem::setSize(const sf::Vector2f & size) noexcept {
        _size = size;
        const auto current = getSize();
        const sf::Vector2f scale(size.x / current.x, size.y / current.y);
        if (_transformable == nullptr)
            _transformable = &getTransformable();
        _transformable->setOrigin(current.x / 2, current.y / 2);
        _transformable->setScale(scale);
    }

    void ViewItem::goTo(const sf::Vector2f & destination, float time) noexcept {
        setPreviousPosition(getPosition());
        setDestination(destination);
        setDelay(time);
        startMove();
    }
}

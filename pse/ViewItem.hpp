#pragma once

#include <memory>
#include "SFML/Graphics.hpp"
#include "Timer.hpp"

namespace pse {
    //
    // Interface for any SFML item
    //
    class ViewItem {
        // Constructor
    public:
        ViewItem() noexcept;

        // Destructor
    public:
        virtual ~ViewItem() noexcept {}

        // Coplien
    public:
        ViewItem(const ViewItem & other) noexcept = default;

        ViewItem & operator=(const ViewItem & rhs) noexcept = default;

        // Move
    public:
        ViewItem(ViewItem && other) noexcept = default;

        ViewItem & operator=(ViewItem && rhs) noexcept = default;

        // Clone pattern
    public:
        virtual std::unique_ptr<ViewItem> copy() const noexcept = 0;

        // Size getters and setters
    public:
        virtual sf::Vector2f getSize() const noexcept = 0;

        virtual void setSize(const sf::Vector2f & size) noexcept;

        // Rendering
    public:
        virtual void draw(sf::RenderWindow & window) noexcept;

        virtual const sf::Drawable & getDrawable() noexcept = 0;

        virtual sf::Transformable & getTransformable() noexcept = 0;

        // Move this item to a specific destination
        // Time can be used to make the item move at a regular speed over a certain time
    public:
        void goTo(const sf::Vector2f & destination, float time = 0) noexcept;

        // Position getters and setters
    public:
        const sf::Vector2f & getPosition() const noexcept { return _pos; }

        virtual void setPosition(const sf::Vector2f & pos) noexcept;

        virtual void setRotation(float radians) noexcept;

        // Destination getters and setters
    public:
        const sf::Vector2f & getDestination() const noexcept { return _destination; }

        void setDestination(const sf::Vector2f & destination) noexcept { _destination = destination; }

        // Previous position getters and setters
    public:
        const sf::Vector2f & getPreviousPosition() const noexcept { return _previousPosition; }

        void setPreviousPosition(const sf::Vector2f & pos) noexcept { _previousPosition = pos; }

        // Movement getters and setters (generally no need to call these, go straight through goTo
    public:
        void setDelay(float delay) noexcept { _moveTimer.setDuration(std::chrono::duration<float>(delay)); }

        void setDelay(const std::chrono::duration<float> & delay) noexcept { _moveTimer.setDuration(delay); }

        void startMove() noexcept { _moveTimer.restart(); }

        float getMoveDuration() const noexcept { return _moveTimer.getDuration().count(); }

        float getMoveTimeLeft() const noexcept { return _moveTimer.getTimeLeft().count(); }

        // Attributes
    private:
        sf::Transformable * _transformable = nullptr;
        const sf::Drawable * _drawable = nullptr;
        sf::Vector2f _pos;
        sf::Vector2f _size;
        sf::Vector2f _destination;
        sf::Vector2f _previousPosition;
        putils::Timer _moveTimer;
    };

    using ViewItemPtr = std::shared_ptr<ViewItem>;
}

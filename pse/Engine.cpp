#include "Engine.hpp"

namespace pse {
    Engine::Engine(size_t screenWidth, size_t screenHeight,
                   const std::string & screenName, int sfWindowStyle,
                   float refreshRate) noexcept
            : _window(sf::VideoMode(screenWidth, screenHeight), screenName, sfWindowStyle),
              _refreshTimer(1 / refreshRate)
    {
        addView("default",
                { (float)screenWidth / 2, (float)screenHeight / 2 }, { (float)screenWidth, (float)screenHeight },
                0);
    }

    void Engine::update(bool clear, const std::function<void()> & preDisplay) noexcept {
        if (clear)
            displayColor(sf::Color::Black);

        drawViews();

        if (preDisplay != nullptr)
            preDisplay();

        _window.display();
    }

    void Engine::drawViews() noexcept {
        size_t drawn = 0;
        size_t height = 0;

        while (drawn < _views.size()) {
            for (const auto & p : _views) {
                const auto & pair = p.second;

                const auto h = pair.second;
                if (h == height) {
                    const auto & view = pair.first;
                    _window.setView(view);
                    drawItems();
                    ++drawn;
                }
            }

            ++height;
        }
    }

    void Engine::drawItems() noexcept {
        size_t drawn = 0;
        size_t height = 0;

        while (drawn < _items.size()) {
            for (const auto & p : _items) {
                if (p.second == height) {
                    p.first->draw(_window);
                    ++drawn;
                }
            }

            ++height;
        }
    }
}

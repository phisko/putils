#pragma once

#include <unordered_map>
#include <chrono>
#include <memory>
#include "SFML/Graphics.hpp"
#include "ViewItem.hpp"

namespace pse {
    //
    // Sprite class for SFML
    //
    class Sprite : public ViewItem {
        // Constructor
    public:
        Sprite(const std::string & texture, const sf::Vector2f & pos, const sf::Vector2f & size);

        // ViewItem functions
    public:
        std::unique_ptr<ViewItem> copy() const noexcept override;

        sf::Vector2f getSize() const noexcept override {
            const auto bounds = _sprite.getLocalBounds();
            return sf::Vector2f(bounds.width, bounds.height);
        }

        const sf::Drawable & getDrawable() noexcept override { return _sprite; }

        sf::Transformable & getTransformable() noexcept override { return _sprite; }

        // Texture getters and setters
    public:
        virtual void setTexture(const std::string & texture);

        const std::string & getTextureFile() const noexcept { return _textureFile; }

        void repeat(const sf::IntRect & rect) {
            _sprite.setScale(1, 1);
            _sprite.setTextureRect(rect);
            textures[_textureFile].setRepeated(true);
        }

        void unrepeat() {
            textures[_textureFile].setRepeated(false);
        }

        // Attributes
    protected:
        sf::Sprite _sprite;
        std::string _textureFile;
        sf::Vector2f _size;

    private:
        static std::unordered_map<std::string, sf::Texture> textures;

        // Coplien
    public:
        Sprite(const Sprite & other) = delete;

        Sprite operator=(const Sprite & other) = delete;
    };
}

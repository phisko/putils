#include "CompositeAnimatedSprite.hpp"
#include "casts.hpp"

pse::CompositeAnimatedSprite::CompositeAnimatedSprite(
        const std::string & texture,
        const sf::Vector2f & pos,
        const sf::Vector2f & size,
        size_t animations,
        const std::vector<size_t> & tilesPerAnimation,
        const sf::Vector2f & tileDimensions,
        float refreshDelay) noexcept
        : AnimatedSprite(
        texture,
        pos,
        size,
        animations,
        tilesPerAnimation,
        tileDimensions,
        refreshDelay
),
          _children(0) {
}

std::unique_ptr<pse::ViewItem> pse::CompositeAnimatedSprite::copy() const noexcept {
    auto ret(std::make_unique<pse::CompositeAnimatedSprite>(
            getTextureFile(),
            getPosition(),
            getSize(),
            getAnimations(),
            getTilesPeranimation(),
            getTileDimensions(),
            getOriginalRefreshDelay()
    ));

    for (auto & c : _children)
        ret->addChild(
                putils::static_unique_ptr_cast<AnimatedSprite>(c->copy())
        );

    return std::move(ret);
}

void pse::CompositeAnimatedSprite::draw(sf::RenderWindow & window) noexcept {
    window.draw(getDrawable());

    for (auto & c : _children)
        c->draw(window);

    refresh();
}

void pse::CompositeAnimatedSprite::setPosition(const sf::Vector2f & pos) noexcept {
    ViewItem::setPosition(pos);

    for (auto & c : _children)
        c->setPosition(pos);
}

void pse::CompositeAnimatedSprite::setSize(const sf::Vector2f & size) noexcept {
    ViewItem::setSize(size);

    for (auto & c : _children)
        c->setSize(size);
}

void pse::CompositeAnimatedSprite::setAnimation(size_t animation) noexcept {
    for (auto & c : _children)
        c->setAnimation(animation);

    AnimatedSprite::setAnimation(animation);
}

void pse::CompositeAnimatedSprite::setTile(size_t currentTile) noexcept {
    for (auto & c : _children)
        c->setTile(currentTile);

    AnimatedSprite::setTile(currentTile);
}

void pse::CompositeAnimatedSprite::refresh() noexcept {
    for (auto & c : _children)
        c->refresh();

    AnimatedSprite::refresh();
}

void pse::CompositeAnimatedSprite::advance() noexcept {
    for (auto & c : _children)
        c->advance();

    AnimatedSprite::advance();
}

void pse::CompositeAnimatedSprite::stopAnimation() noexcept {
    for (auto & c : _children)
        c->stopAnimation();

    AnimatedSprite::stopAnimation();
}

void pse::CompositeAnimatedSprite::updateTexture() noexcept {
    for (auto & c : _children)
        c->updateTexture();

    AnimatedSprite::updateTexture();
}

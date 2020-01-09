# pse
Phisko's SFML Engine, a simple graphics engine built over SFML

## Classes

* [Engine](Engine.md): main class, manages `ViewItems`
* ViewItem: base class managed by `Engine`. Provides methods for size and position manipulation.
* Grid: allows for simple placement of `ViewItems` in a grid format.
* Sprite: `ViewItem` class for a static image
* AnimatedSprite: `ViewItem` class for an animated image
* CompositeAnimatedSprite: `ViewItem` class for several, simultaneously animated images
* Text: `ViewItem` class for text
* Shape: `ViewItem` class for any sf::Shape

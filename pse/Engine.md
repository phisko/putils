# [Engine](Engine.hpp)

Manages and renders `ViewItems`.

### Members

##### Constructor

```cpp
Engine(size_t screenWidth, size_t screenHeight,
    const std::string &screenName,
    double refreshRate = 120);
```

Creates a new window with specified properties.

##### addItem

```cpp
void addItem(ViewItem *item, size_t height = 0);
```

Adds a new `ViewItem` to manage, at the specified `height` (determines whether it should be drawn over others).

##### removeItem

```cpp
void removeItem(ViewItem *item);
```

Removes a ViewItem

##### update

```cpp
void update(bool clear = true);
```

Updates the window, updating all `ViewItems`' positions according to their movement.

##### pollEvent

```cpp
bool pollEvent(sf::Event &e);
```

Lets the user poll events straight from the SFML window.

##### displayColor

```cpp
void displayColor(const sf::Color &color);
```

Splashes a color on the window.

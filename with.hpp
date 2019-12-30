#pragma once

#define putils_with(object) auto & _(object);

// Lets you do things like:
//
// kengine::EntityManager em;
// { putils_with(em.createEntity<kengine::GameObject>("Bob") {
//      { putils_with(_.attachComponent<kengine::TransformComponent>()) {  // "_" is provided by putils_with
//          _.boundingBox.position.x = 0;                                    // "_" is provided by putils_with
//          _.boundingBox.position.y = 1;
//      }}
//
//      { putils_with(_.attachComponent<kengine::GraphicsComponent>()) {
//          _.appearance = "sprite.png";
//      }}
// }}

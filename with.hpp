#pragma once

#define pmeta_with(object) auto & _(object);

// Lets you do things like:
//
// kengine::EntityManager em;
// { pmeta_with(em.createEntity<kengine::GameObject>("Bob") {
//      { pmeta_with(_.attachComponent<kengine::TransformComponent3f>()) {  // "_" is provided by pmeta_with
//          _.boundingBox.position.x = 0;                                    // "_" is provided by pmeta_with
//          _.boundingBox.position.y = 1;
//      }}
//
//      { pmeta_with(_.attachComponent<kengine::GraphicsComponent>()) {
//          _.appearance = "sprite.png";
//      }}
// }}

#include <gf/Action.h>
#include <gf/Clock.h>
#include <gf/Color.h>
#include <gf/EntityContainer.h>
#include <gf/Event.h>
#include <gf/RenderWindow.h>
#include <gf/ViewContainer.h>
#include <gf/Views.h>
#include <gf/Window.h>

#include "local/Hero.h"

#include <gf/Log.h>

int main() {
  static constexpr gf::Vector2u ScreenSize(1024, 576);
  static constexpr gf::Vector2f ViewSize(100.0f, 100.0f); // dummy values
  static constexpr gf::Vector2f ViewCenter(50.0f, 50.0f); // dummy values

  // initialization

  gf::Window window("Bygone Islands", ScreenSize);
  gf::RenderWindow renderer(window);

  // views

  gf::ViewContainer views;

  gf::ExtendView mainView(ViewCenter, ViewSize);
  views.addView(mainView);

  gf::ScreenView hudView;
  views.addView(hudView);

  views.setInitialScreenSize(ScreenSize);

  // actions

  gf::ActionContainer actions;

  gf::Action closeWindowAction("Close window");
  closeWindowAction.addCloseControl();
  closeWindowAction.addKeycodeKeyControl(gf::Keycode::Escape);
  actions.addAction(closeWindowAction);

  gf::Action fullscreenAction("Fullscreen");
  fullscreenAction.addKeycodeKeyControl(gf::Keycode::F);
  actions.addAction(fullscreenAction);

  gf::Action leftAction("Left");
  leftAction.addScancodeKeyControl(gf::Scancode::A);
  leftAction.addScancodeKeyControl(gf::Scancode::Left);
  leftAction.setContinuous();
  actions.addAction(leftAction);

  gf::Action rightAction("Right");
  rightAction.addScancodeKeyControl(gf::Scancode::D);
  rightAction.addScancodeKeyControl(gf::Scancode::Right);
  rightAction.setContinuous();
  actions.addAction(rightAction);

  gf::Action upAction("Up");
  upAction.addScancodeKeyControl(gf::Scancode::W);
  upAction.addScancodeKeyControl(gf::Scancode::Up);
  upAction.setContinuous();
  actions.addAction(upAction);

  gf::Action downAction("Down");
  downAction.addScancodeKeyControl(gf::Scancode::S);
  downAction.addScancodeKeyControl(gf::Scancode::Down);
  downAction.setContinuous();
  actions.addAction(downAction);

  // entities
  bi::Hero hero({ 0.0f, 0.0f });

  gf::EntityContainer mainEntities;
  mainEntities.addEntity(hero);

  gf::EntityContainer hudEntities;
  // add entities to hudEntities

  // game loop

  renderer.clear(gf::Color::Black);

  gf::Clock clock;

  while (window.isOpen()) {
    // 1. input

    gf::Event event;

    while (window.pollEvent(event)) {
      actions.processEvent(event);
      views.processEvent(event);
    }

    if (closeWindowAction.isActive()) {
      window.close();
    }

    if (fullscreenAction.isActive()) {
      window.toggleFullscreen();
    }

    if (rightAction.isActive()) {
      // gf::Log::print("RIGHT\n");
      hero.turnRight();
    } else if (leftAction.isActive()) {
      // gf::Log::print("LEFT\n");
      hero.turnLeft();
    } else {
      // gf::Log::print("TURN OFF");
      hero.turnNone();
    }

    if (upAction.isActive()) {
      // gf::Log::print("UP\n");
      hero.moveForward();
    } else if (downAction.isActive()) {
      // gf::Log::print("DOWN\n");
      hero.moveBackward();
    } else {
      // gf::Log::print("STOP");
      hero.moveStop();
    }


    // 2. update

    float dt = clock.restart().asSeconds();
    mainEntities.update(dt);
    hudEntities.update(dt);


    // 3. draw

    renderer.clear();

    renderer.setView(mainView);
    mainEntities.render(renderer);

    renderer.setView(hudView);
    hudEntities.render(renderer);

    renderer.display();

    actions.reset();
  }

  return 0;
}

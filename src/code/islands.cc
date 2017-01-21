#include <gf/Action.h>
#include <gf/Clock.h>
#include <gf/Color.h>
#include <gf/EntityContainer.h>
#include <gf/Event.h>
#include <gf/Log.h>
#include <gf/Random.h>
#include <gf/RenderWindow.h>
#include <gf/ViewContainer.h>
#include <gf/Views.h>
#include <gf/Window.h>

#include "config.h"
#include "local/Hero.h"
#include "local/Messages.h"
#include "local/Sea.h"
#include "local/Singletons.h"
#include "local/Steam.h"
#include "local/TreasuresManager.h"

int main() {
  static constexpr gf::Vector2u ScreenSize(1024, 576);
  static constexpr gf::Vector2f ViewSize(576.0f, 576.0f);
  static constexpr gf::Vector2f ViewCenter(288.0f, 288.0f);

  // initialization
  gf::Log::setLevel(gf::Log::Level::Debug);

  gf::Window window("Bygone Islands", ScreenSize);
  gf::RenderWindow renderer(window);

  gf::SingletonStorage<gf::MessageManager> storageForMessageManager(bi::gMessageManager);
  gf::SingletonStorage<gf::ResourceManager> storageForResourceManager(bi::gResourceManager);
  gf::SingletonStorage<gf::Random> storageForRandom(bi::gRandom);

  bi::gResourceManager().addSearchDir(ISLANDS_DATA_DIR);

  // views

  gf::ViewContainer views;

  gf::ExtendView mainView(ViewCenter, ViewSize);
  views.addView(mainView);

  gf::ScreenView hudView;
  views.addView(hudView);

  views.setInitialScreenSize(ScreenSize);

  bi::gMessageManager().registerHandler<bi::HeroPosition>([&mainView](gf::Id type, gf::Message *msg) {
    assert(type == bi::HeroPosition::type);
    auto positionHeroMessage = static_cast<bi::HeroPosition*>(msg);
    mainView.setCenter(positionHeroMessage->position);
    return gf::MessageStatus::Keep;
  });

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
  bi::Steam steam;
//   steam.run();

  bi::Hero hero({ 4000.0f, 4000.0f });

  bi::Sea sea;
  sea.generate();

  bi::TreasuresManager treasures;
  treasures.addTreasure({ 500.0f, 0.0f });
  treasures.addTreasure({ 500.0f, 50.0f });

  gf::EntityContainer mainEntities;
  mainEntities.addEntity(hero);
  mainEntities.addEntity(sea);
  mainEntities.addEntity(treasures);
  mainEntities.addEntity(steam);

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

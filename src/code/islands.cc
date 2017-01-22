#include <gf/Action.h>
#include <gf/Clock.h>
#include <gf/Color.h>
#include <gf/EntityContainer.h>
#include <gf/Event.h>
#include <gf/Gamepad.h>
#include <gf/Log.h>
#include <gf/Random.h>
#include <gf/RenderWindow.h>
#include <gf/ViewContainer.h>
#include <gf/Views.h>
#include <gf/Window.h>

#include "config.h"
#include "local/Compass.h"
#include "local/Gold.h"
#include "local/DecorationManager.h"
#include "local/Hero.h"
#include "local/Messages.h"
#include "local/ResourceManager.h"
#include "local/Sea.h"
#include "local/Singletons.h"
#include "local/SoundManager.h"
#include "local/Steam.h"
#include "local/TreasureManager.h"
#include "local/TurretManager.h"
#include "local/WaveManager.h"

int main() {
  static constexpr gf::Vector2u ScreenSize(1024, 576);
  static constexpr gf::Vector2f ViewSize(576.0f, 576.0f);
  static constexpr gf::Vector2f ViewCenter(288.0f, 288.0f);

  // initialization
  gf::Log::setLevel(gf::Log::Level::Debug);

  gf::Window window("Bygone Islands", ScreenSize);
  window.setFullscreen(true);
  gf::RenderWindow renderer(window);

  gf::SingletonStorage<gf::MessageManager> storageForMessageManager(bi::gMessageManager);
  gf::SingletonStorage<bi::ResourceManager> storageForResourceManager(bi::gResourceManager);
  gf::SingletonStorage<gf::Random> storageForRandom(bi::gRandom);
  gf::SingletonStorage<gf::WindowGeometryTracker> storageForWindowGeometryTracker(bi::gWinGeometry);

  bi::gResourceManager().addSearchDir(ISLANDS_DATA_DIR);

  gf::Gamepad::initialize();

  // views

  gf::ViewContainer views;

  gf::ExtendView mainView(ViewCenter, ViewSize);
  views.addView(mainView);

  gf::ScreenView hudView;
  views.addView(hudView);

  views.setInitialScreenSize(ScreenSize);
  bi::gWinGeometry().setInitialScreenSize(ScreenSize);

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

  gf::Action scanAction("Scan");
  scanAction.addScancodeKeyControl(gf::Scancode::Space);
  actions.addAction(scanAction);

  // entities
  bi::Steam steam;
//   steam.run();

  bi::Hero hero(steam, { 4000.0f, 4000.0f });

  bi::TreasureManager treasures;

  bi::DecorationManager decorationsAbove(15);
  bi::DecorationManager decorationsBelow(5);

  bi::TurretManager turrets;

  bi::Sea sea;
  sea.generate(treasures, decorationsAbove, decorationsBelow, turrets);

  bi::Compass compass;
  bi::Gold gold;

  bi::WaveManager waves;

  bi::SoundManager sounds;

  // turrets.addTurret({ 4500.0f, 4000.0f });

  gf::EntityContainer mainEntities;
  mainEntities.addEntity(hero);
  mainEntities.addEntity(sea);
  mainEntities.addEntity(treasures);
  mainEntities.addEntity(decorationsAbove);
  mainEntities.addEntity(decorationsBelow);
  mainEntities.addEntity(steam);
  mainEntities.addEntity(waves);
  mainEntities.addEntity(turrets);

  gf::EntityContainer hudEntities;
  // add entities to hudEntities
  hudEntities.addEntity(compass);
  hudEntities.addEntity(gold);

  // game loop
  renderer.clear(gf::Color::Black);

  gf::Clock clock;

  while (window.isOpen()) {
    // 1. input

    gf::Event event;

    while (window.pollEvent(event)) {
      actions.processEvent(event);
      views.processEvent(event);
      bi::gWinGeometry().processEvent(event);
      // Gamepad Input
      gf::GamepadId id;
      if (event.type == gf::EventType::GamepadConnected) {
        id = gf::Gamepad::open(event.gamepadConnection.id);
        leftAction.addGamepadButtonControl(id,gf::GamepadButton::DPadLeft);
        leftAction.addGamepadAxisControl(id,gf::GamepadAxis::LeftX,gf::GamepadAxisDirection::Negative);
        rightAction.addGamepadButtonControl(id,gf::GamepadButton::DPadRight);
        rightAction.addGamepadAxisControl(id,gf::GamepadAxis::LeftX,gf::GamepadAxisDirection::Positive);
        upAction.addGamepadButtonControl(id,gf::GamepadButton::DPadUp);
        upAction.addGamepadAxisControl(id,gf::GamepadAxis::LeftY,gf::GamepadAxisDirection::Negative);
        downAction.addGamepadButtonControl(id,gf::GamepadButton::DPadDown);
        downAction.addGamepadAxisControl(id,gf::GamepadAxis::LeftY,gf::GamepadAxisDirection::Positive);
        scanAction.addGamepadButtonControl(id,gf::GamepadButton::A);
        closeWindowAction.addGamepadButtonControl(id,gf::GamepadButton::Start);
      }

      if (event.type == gf::EventType::GamepadDisconnected) {
          gf::Gamepad::close(event.gamepadDisconnection.id);
      }
    }

    if (closeWindowAction.isActive()) {
      window.close();
    }

    if (fullscreenAction.isActive()) {
      window.toggleFullscreen();
    }

    if (scanAction.isActive()) {
      if (hero.scanAvailable()) {
        // Send the start scan
        bi::StartScan message;
        bi::gMessageManager().sendMessage(&message);
      }
    }

    if (rightAction.isActive()) {
      hero.turnRight();
    } else if (leftAction.isActive()) {
      hero.turnLeft();
    } else {
      hero.turnNone();
    }

    if (upAction.isActive()) {
      hero.moveForward();
    } else if (downAction.isActive()) {
      hero.moveBackward();
    } else {
      hero.moveStop();
    }

    // 2. update

    float dt = clock.restart().asSeconds();
    if (!gold.isGameOver()) {
      mainEntities.update(dt);
      hudEntities.update(dt);
    }


    // 3. draw

    renderer.clear();

    if (gold.isGameOver() || gold.isWin()) {
      renderer.setView(hudView);
      gold.render(renderer);
    } else {
      renderer.setView(mainView);
      mainEntities.render(renderer);

      renderer.setView(hudView);
      hudEntities.render(renderer);
    }

    renderer.display();

    actions.reset();
  }

  return 0;
}

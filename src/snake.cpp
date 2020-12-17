#include "snake.h"
#include "screen.h"
#include <chrono>

CSnake::CSnake(CRect r, char _c):
  CFramedWindow(r, _c)
{
  gamePaused = true;
  showHelp = true;
  level = 1;
  timer = std::chrono::high_resolution_clock::now();
  refresh_rate = std::chrono::milliseconds(1000);
}

void CSnake::paint() {
  CFramedWindow::paint();

  if(gamePaused && showHelp) {
    paintInstruction();
  }

  if(!gamePaused) {
    gotoyx(geom.topleft.y + 3, geom.topleft.x + 4);

    printl("Refresh: ", rand() % 255);
  }

}

void CSnake::paintInstruction() {
  gotoyx(geom.topleft.y + 3, geom.topleft.x + 4);
  printl("h - toggle help information");
  gotoyx(geom.topleft.y + 4, geom.topleft.x + 4);
  printl("p - toggle pause/play mode");
  gotoyx(geom.topleft.y + 5, geom.topleft.x + 4);
  printl("r - restart");
  gotoyx(geom.topleft.y + 6, geom.topleft.x + 4);
  printl("arrows - move snake (in play mode) or");
  gotoyx(geom.topleft.y + 7, geom.topleft.x + 4);
  printl("         move window (in pause mode)");
}

bool CSnake::handleEvent(int key) {
  if(key == 1337) {
    auto newTimer = std::chrono::high_resolution_clock::now();
    auto elapsedTime = newTimer - timer;
    if(elapsedTime >= refresh_rate) {
      timer = newTimer;
      return true;
    }
  }

  if(key == 112) {
    gamePaused = !gamePaused;
    showHelp = false;
    return true;
  }

  if(key == 104 && gamePaused) {
    showHelp = !showHelp;
    return true;
  }

  if(gamePaused) {
    return CFramedWindow::handleEvent(key);
  }

  return false;
}
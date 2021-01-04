#include "snake.h"
#include "screen.h"
#include "cpoint.h"
#include <chrono>

CSnake::CSnake(CRect r, char _c):
  CFramedWindow(r, _c)
{
  gamePaused = true;
  showHelp = true;
  level = 1;
  timer = std::chrono::high_resolution_clock::now();
  refresh_rate = std::chrono::milliseconds(500);

  // snake.push_back(CPoint(geom.topleft.x + 22, geom.topleft.y + 10));
  // snake.push_back(CPoint(geom.topleft.x + 21, geom.topleft.y + 10));
  // snake.push_back(CPoint(geom.topleft.x + 20, geom.topleft.y + 10));
  snake.push_back(CPoint(20, 10));

  snakeDirection = DIR_LEFT;
}

void CSnake::paint() {
  CFramedWindow::paint();

  if(gamePaused && showHelp) {
    paintInstruction();
  }

  if(!showHelp) {
    if(!gamePaused) moveSnake();
    paintSnake();
  }

}

void CSnake::paintSnake() {
  for(auto chunk : snake) {
    gotoyx(geom.topleft.y + chunk.y, geom.topleft.x + chunk.x);
    printl("#");
  }
}

void CSnake::moveSnake() {
  switch(snakeDirection) {
    case DIR_LEFT:
      snake.insert(snake.begin(), CPoint(
        geom.topleft.x + snake[0].x - 1 == geom.topleft.x ? geom.size.x - 2 : snake[0].x - 1,
        snake[0].y
        ));
    break;
    case DIR_TOP:
      snake.insert(snake.begin(), CPoint(
        snake[0].x, 
        geom.topleft.y + snake[0].y - 1 == geom.topleft.y ? geom.size.y - 2 : snake[0].y - 1
        ));
    break;
    case DIR_RIGHT:
      snake.insert(snake.begin(), CPoint(
        geom.topleft.x + snake[0].x + 1 == geom.topleft.x + geom.size.x - 1 ? 1 : snake[0].x + 1, 
        snake[0].y
        ));
    break;
    case DIR_DOWN:
      snake.insert(snake.begin(), CPoint(
        snake[0].x, 
        geom.topleft.y + snake[0].y + 1 == geom.topleft.y + geom.size.y - 1 ? 1 : snake[0].y + 1
        ));
    break;
  }
  snake.pop_back();

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
  if(key == REFRESH_KEY) {
    if(shouldRefresh() && !gamePaused) {
      return true;
    }
  }

  if(key == '\t') {
    gamePaused = true;
    return false;
  }

  if(key == 112) {
    if(gamePaused) timer = std::chrono::high_resolution_clock::now();
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

bool CSnake::shouldRefresh() {
  auto newTimer = std::chrono::high_resolution_clock::now();
  auto elapsedTime = newTimer - timer;
  if(elapsedTime > refresh_rate) {
    timer = newTimer;
    return true;
  }

  return false;
}
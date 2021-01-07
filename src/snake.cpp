#include <chrono>
#include <algorithm>
#include "snake.h"
#include "screen.h"
#include "cpoint.h"

CSnake::CSnake(CRect r, char _c):
  CFramedWindow(r, _c)
{
  gamePaused = true;
  showHelp = true;
  gameOver = false;
  initialRefreshRate = 300;
  level = 1;
  score = 3;
  refreshRateNum = initialRefreshRate;
  timer = std::chrono::high_resolution_clock::now();
  refreshRate = std::chrono::milliseconds(initialRefreshRate);

  snake.push_back(CPoint(20, 10));
  snake.push_back(CPoint(21, 10));
  snake.push_back(CPoint(22, 10));

  generateFruit();

  snakeDirection = DIR_LEFT;
  directionLock = false;
}

void CSnake::paint() {
  CFramedWindow::paint();

  if(gamePaused && showHelp) {
    paintInstruction();
  }

  if(!showHelp) {
    if(!gamePaused && !gameOver) moveSnake();
    paintSnake();
    paintFruit();
    paintScore();
  }

  if(gameOver) {
    paintGameOverScreen();
  }
}

void CSnake::paintGameOverScreen() {
  gotoyx(geom.topleft.y + 1, geom.topleft.x + 1);
  printl("GAME OVER. Result: %u", score);
}

void CSnake::paintFruit() {
  gotoyx(geom.topleft.y + fruit.y, geom.topleft.x + fruit.x);
  printl("@");
}

void CSnake::paintSnake() {
  for(int i = snake.size() - 1; i>=0; i--) {
    gotoyx(geom.topleft.y + snake[i].y, geom.topleft.x + snake[i].x);
    if(i == 0) 
      printl("*");
    else 
      printl("+");
  }
}

void CSnake::paintScore() {
  gotoyx(geom.topleft.y - 1, geom.topleft.x);
  printl("| LEVEL: %u |", level);
}

void CSnake::moveSnake() {
  directionLock = false;
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
  if(!checkIfEaten()) 
    snake.pop_back();
  checkSnakeIntegrity();

}

void CSnake::generateFruit() {
  do {
    fruit.x = (rand() % (geom.size.x - 2)) + 1;
    fruit.y = (rand() % (geom.size.y - 2)) + 1;
  }while(std::find_if(snake.begin(), snake.end(), [&](CPoint& a) { return fruit == a; }) != snake.end());
}

bool CSnake::checkIfEaten() {
  if(fruit == snake[0]) {
    generateFruit();
    advanceLevel();
    score += 1;
    return true;
  }
  else return false;
}

void CSnake::advanceLevel() {
  if(score / 3 > level) {
    level += 1;
    refreshRateNum -= 40;
    refreshRate = std::chrono::milliseconds(refreshRateNum);
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
  if(!directionLock && !gameOver){
    if(key == KEY_UP && snakeDirection != DIR_DOWN) {
      snakeDirection = DIR_TOP;
      directionLock = true;
      return false;
    }
    if(key == KEY_RIGHT && snakeDirection != DIR_LEFT) {
      snakeDirection = DIR_RIGHT;
      directionLock = true;
      return false;
    }
    if(key == KEY_DOWN && snakeDirection != DIR_TOP) {
      snakeDirection = DIR_DOWN;
      directionLock = true;
      return false;
    }
    if(key == KEY_LEFT && snakeDirection != DIR_RIGHT) {
      snakeDirection = DIR_LEFT;
      directionLock = true;
      return false;
    }
  }

  if(key == 114) {
    resetGame();
  }

  if(gamePaused) {
    return CFramedWindow::handleEvent(key);
  }

  return false;
}

bool CSnake::shouldRefresh() {
  auto newTimer = std::chrono::high_resolution_clock::now();
  auto elapsedTime = newTimer - timer;
  if(elapsedTime > refreshRate) {
    timer = newTimer;
    return true;
  }

  return false;
}

void CSnake::resetGame() {
  gamePaused = true;
  showHelp = true;
  gameOver = false;
  level = 1;
  score = 3;
  timer = std::chrono::high_resolution_clock::now();
  refreshRate = std::chrono::milliseconds(initialRefreshRate);
  refreshRateNum = initialRefreshRate;
  snake.clear();
  snake.push_back(CPoint(20, 10));
  snake.push_back(CPoint(21, 10));
  snake.push_back(CPoint(22, 10));

  generateFruit();

  snakeDirection = DIR_LEFT;
  directionLock = false;
  paint();
}

void CSnake::checkSnakeIntegrity() {
  for(auto i = snake.begin() + 1; i < snake.end(); i++){
    if(*i == snake[0]) {
      gameOver = true;
      paint();
    }
  }
}
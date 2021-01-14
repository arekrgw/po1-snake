#ifndef __SNAKE_H__
#define __SNAKE_H__

#include <chrono>
#include <vector>
#include "winsys.h"
#include "cpoint.h"

class CSnake : public CFramedWindow
{
public:
  CSnake(CRect r, char _c = ' ');

  static const int REFRESH_KEY = 666333;
  
  //ncurses public required functions
  void paint();
  bool handleEvent(int);

private:
  std::vector<CPoint> snake;
  enum SnakeDirection { DIR_TOP, DIR_RIGHT, DIR_DOWN, DIR_LEFT };
  SnakeDirection snakeDirection;
  bool directionLock;
  bool gamePaused;
  bool gameOver;
  CPoint fruit;
  unsigned int level;
  unsigned int score;
  bool showHelp;

  //automatic refresh
  std::chrono::high_resolution_clock::time_point timer;
  std::chrono::milliseconds refreshRate;
  unsigned int refreshRateNum;
  unsigned int initialRefreshRate;
  bool shouldRefresh();

  //render functions
  void paintInstruction();
  void paintSnake();
  void paintScore();
  void paintFruit();
  void paintGameOverScreen();

  //game logic functions
  void generateFruit();
  bool checkIfEaten();
  void resetGame(bool = false);
  void moveSnake();
  void advanceLevel();
  void checkSnakeIntegrity();
};

#endif

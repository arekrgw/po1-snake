#ifndef __SNAKE_H__
#define __SNAKE_H__

#include <chrono>
#include "winsys.h"
#include "cpoint.h"
#include <vector>

class CSnake : public CFramedWindow
{
public:
  CSnake(CRect r, char _c = ' ');

  static const int REFRESH_KEY = 666333;
  void paint();
  bool handleEvent(int key);
private:
  std::vector<CPoint> snake;
  enum SnakeDirection {DIR_TOP, DIR_RIGHT, DIR_DOWN, DIR_LEFT};
  SnakeDirection snakeDirection;
  bool directionLock;
  bool gamePaused;
  bool gameOver;
  CPoint fruit;
  unsigned int level;
  unsigned int score;
  bool showHelp;
  void paintInstruction();
  void paintSnake();
  void paintScore();
  void generateFruit();
  bool checkIfEaten();
  void paintFruit();
  void paintGameOverScreen();
  void resetGame();
  void moveSnake();
  void advanceLevel();
  void checkSnakeIntegrity();
  bool shouldRefresh();
  std::chrono::high_resolution_clock::time_point timer;
  std::chrono::milliseconds refreshRate;
  unsigned int refreshRateNum;
  unsigned int initialRefreshRate = 300;
};

#endif

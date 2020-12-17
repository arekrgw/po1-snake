#ifndef __SNAKE_H__
#define __SNAKE_H__

#include <chrono>
#include "winsys.h"

class CSnake : public CFramedWindow
{
public:
  CSnake(CRect r, char _c = ' ');

  static const int REFRESH_KEY = 666333;
  void paint();
  bool handleEvent(int key);
private:
  bool gamePaused;
  unsigned int level;
  bool showHelp;
  void paintInstruction();
  void repaintSnake();
  bool shouldRefresh();
  std::chrono::high_resolution_clock::time_point timer;
  std::chrono::milliseconds refresh_rate;
};

#endif

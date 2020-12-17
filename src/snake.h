#ifndef __SNAKE_H__
#define __SNAKE_H__

#include <chrono>
#include "winsys.h"

class CSnake : public CFramedWindow
{
public:
  CSnake(CRect r, char _c = ' ');

  void paint();
  bool handleEvent(int key);
  void frameRate();
private:
  bool gamePaused;
  unsigned int level;
  bool showHelp;
  void paintInstruction();
  void repaintSnake();
  std::chrono::high_resolution_clock::time_point timer;
  std::chrono::milliseconds refresh_rate;
};

#endif

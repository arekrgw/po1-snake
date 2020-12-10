#ifndef __SNAKE_H__
#define __SNAKE_H__

#include "winsys.h"

class CSnake:public CFramedWindow
{
public:
  CSnake(CRect r, char _c = ' ');

  void paint();
  bool handleEvent(int key);
private:
  bool gamePaused;
  unsigned int level;
  bool showHelp;
  void paintInstruction();
};

#endif

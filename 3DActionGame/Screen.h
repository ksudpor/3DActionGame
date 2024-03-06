#ifndef SCREEN_H_
#define SCREEN_H_

#include <GSgame.h>

class Screen {
public:
	static int Width;
	static int Height;

    static bool Is_Fullscreen;
    static int Refresh_Rate;
    static GSvector2 Scale;
};

#endif

#include "Screen.h"

int Screen::Width = 1280;
int Screen::Height = 720;
bool Screen::Is_Fullscreen{ false };
int Screen::Refresh_Rate{ 60 };
GSvector2 Screen::Scale{ (float)Screen::Width / 1920.0f, (float)Screen::Height / 1080.0f };
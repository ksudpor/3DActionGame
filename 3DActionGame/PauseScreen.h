#ifndef PAUSE_SCREEN_H_
#define PAUSE_SCREEN_H_

#include <GSgame.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

class PauseScreen {
	enum class OptionMenu {
		Resolution,
		ScreenMode,
		BGM,
		SE,
		LangMenu,
		EndMenu
	};

	enum class EndMenu {
		ApplyOption,
		ReturnTitle
	};

public:
	PauseScreen() = default;
	void start();
	void update(float delta_time);
	void draw() const;
	void end();
	bool is_end() const;

private:
	bool is_end_{ false };
	OptionMenu menu_{ OptionMenu::Resolution };
	EndMenu end_menu_{ EndMenu::ApplyOption };
};

#endif

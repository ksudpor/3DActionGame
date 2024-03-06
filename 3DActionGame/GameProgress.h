#ifndef GAME_PROGRESS_H_
#define GAME_PROGRESS_H_

#include <string>
#include <gslib.h>

class GameProgress {
public:
	enum class Weapon {
		None,
		Axe,
		SwordShield,
		GreatSword,
		Pistol,
		Rifle,
		Shotgun
	};

	enum class Reward {
		None,
		Health,
		Stamina,
		Speed,
		Attack
	};

	enum class Resolution {
		Res640,
		Res800,
		Res1024,
		Res1280,
		Res1600,
		Res1920
	};

	enum class ScreenMode {
		Window,
		Fullscreen
	};

	enum class LangMenu {
		Japanese,
		English
	};

	enum class StageProgress {
		One,
		Two,
		Three
	};

	static GSuint& stage_image(StageProgress stage);
	static Resolution resolution_;
	static ScreenMode screen_mode_;
	static LangMenu lang_menu_;
	static Weapon primary_weapon;
	static Weapon secondary_weapon;
	static int score_stage;
	static float time_stage;
	static StageProgress stage_;
	static int max_stage_;

	static Reward reward_;
	static int player_health_;
	static float player_stamina_;
	static float player_speed_rate_;
	static float player_attack_rate_;
};

#endif
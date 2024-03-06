#ifndef LANGUAGE_H_
#define LANGUAGE_H_

#include <gslib.h>

class Language {
public:
	static enum class Select {
		English,
		Japanese,
	};

	static void change_language(Select language);
	static Select language;
	static GSuint resolution_image;
	static GSuint screenmode_image;
	static GSuint bgm_image;
	static GSuint se_image;
	static GSuint language_image;
	static GSuint check_control_image;
	static GSuint window_image;
	static GSuint fullscreen_image;
	static GSuint option_image;
	static GSuint play_icon_image;
	static GSuint option_icon_image;
	static GSuint quit_icon_image;
	static GSuint confirm_image;
	static GSuint weapon1_image;
	static GSuint weapon2_image;
	static GSuint stage1_image;
	static GSuint stage2_image;
	static GSuint stage3_image;
	static GSuint quest_stage1_image;
	static GSuint quest_stage2_image;
	static GSuint quest_stage3_image;
	static GSuint select_weapon_image;
	static GSuint select_reward_image;
	static GSuint health_reward_image;
	static GSuint stamina_reward_image;
	static GSuint speed_reward_image;
	static GSuint attack_reward_image;
	static GSuint same_weapon_image;
	static GSuint game_over_image;
	static GSuint game_clear_image;
	static GSuint apply_image;
	static GSuint retry_image;
	static GSuint retry_reset_image;
	static GSuint next_image;
	static GSuint result_image;
	static GSuint return_title_image;
	static GSuint time_spent_image;
	static GSuint total_score_image;
};

#endif

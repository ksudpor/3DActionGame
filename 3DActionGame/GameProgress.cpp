#include "GameProgress.h"
#include "Language.h"

GameProgress::Resolution GameProgress::resolution_ = GameProgress::Resolution::Res1280;
GameProgress::ScreenMode GameProgress::screen_mode_ = GameProgress::ScreenMode::Window;
GameProgress::LangMenu GameProgress::lang_menu_ = GameProgress::LangMenu::Japanese;
GameProgress::Weapon GameProgress::primary_weapon = GameProgress::Weapon::None;
GameProgress::Weapon GameProgress::secondary_weapon = GameProgress::Weapon::None;
GameProgress::StageProgress GameProgress::stage_ = GameProgress::StageProgress::One;
int GameProgress::score_stage = 0;
float GameProgress::time_stage = 0;
int GameProgress::max_stage_ = 3;

GameProgress::Reward GameProgress::reward_ = GameProgress::Reward::None;
int GameProgress::player_health_ = 1000; //1000 -> 1500 -> 2000
float GameProgress::player_stamina_ = 120.0f; //120 -> 160 -> 200
float GameProgress::player_speed_rate_ = 1.0f; // 1.0 -> 1.5 -> 2.0
float GameProgress::player_attack_rate_ = 1.0f; // 1.0 -> 1.5 -> 2.0

GSuint& GameProgress::stage_image(StageProgress stage){
	switch (stage){
	case GameProgress::StageProgress::One:
		return Language::stage1_image;
		break;
	case GameProgress::StageProgress::Two:
		return Language::stage2_image;
		break;
	case GameProgress::StageProgress::Three:
		return Language::stage3_image;
		break;
	default:
		break;
	}
}
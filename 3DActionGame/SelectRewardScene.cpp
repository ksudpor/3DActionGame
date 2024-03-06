#include "SelectRewardScene.h"
#include "Screen.h"
#include "Assets.h"
#include "Language.h"
#include "GameProgress.h"
#include "UIManager.h"
#include "Fade.h"

BoundingRectangle button_reward[4];
BoundingRectangle button_confirm_reward;

void SelectRewardScene::start() {
    // 画像の読み込み
    gsLoadTexture(Image_ConfirmEN, "Assets/Image/confirm_en.png");
    gsLoadTexture(Image_ConfirmJP, "Assets/Image/confirm_jp.png");
    gsLoadTexture(Image_HealthRewardEN, "Assets/Image/health_reward_en.png");
    gsLoadTexture(Image_HealthRewardJP, "Assets/Image/health_reward_jp.png");
    gsLoadTexture(Image_StaminaRewardEN, "Assets/Image/stamina_reward_en.png");
    gsLoadTexture(Image_StaminaRewardJP, "Assets/Image/stamina_reward_jp.png");
    gsLoadTexture(Image_SpeedRewardEN, "Assets/Image/speed_reward_en.png");
    gsLoadTexture(Image_SpeedRewardJP, "Assets/Image/speed_reward_jp.png");
    gsLoadTexture(Image_AttackRewardEN, "Assets/Image/attack_reward_en.png");
    gsLoadTexture(Image_AttackRewardJP, "Assets/Image/attack_reward_jp.png");
    gsLoadTexture(Image_SelectRewardEN, "Assets/Image/select_reward_en.png");
    gsLoadTexture(Image_SelectRewardJP, "Assets/Image/select_reward_jp.png");
    gsLoadSE(SE_Select, "Assets/Sound/seSelect.wav", 1, GWAVE_DEFAULT);
    gsLoadSE(SE_Confirm, "Assets/Sound/seConfirm.wav", 1, GWAVE_DEFAULT);

    is_end_ = false;
    menu_ = SelectMenu::Reward;

    fade_ = new Fade();
    fade_->start_fade_in(60.0f);
}

void SelectRewardScene::update(float delta_time) {
    fade_->update(delta_time);
    if (fade_->end_scene()) is_end_ = true;
    if (fade_->is_fading()) return;

    // キーボード操作
    if (gsGetKeyTrigger(GKEY_UP)) {
        UIManager::DecrementEnum(menu_, 0, 1);
        gsPlaySE(SE_Select);
    }
    if (gsGetKeyTrigger(GKEY_DOWN)) {
        UIManager::IncrementEnum(menu_, 0, 1);
        gsPlaySE(SE_Select);
    }

    if (menu_ == SelectMenu::Reward) {
        if (gsGetKeyTrigger(GKEY_LEFT)) {
            UIManager::DecrementEnum(reward_, 0, 3);
            gsPlaySE(SE_Select);
        }
        if (gsGetKeyTrigger(GKEY_RIGHT)) {
            UIManager::IncrementEnum(reward_, 0, 3);
            gsPlaySE(SE_Select);
        }
    }

    // マウス操作
    int mx, my;
    gsGetMouseCursorPosition(&mx, &my);
    BoundingRectangle mouse_pos{ GSvector2{(float)mx, (float)my}, GSvector2{(float)mx, (float)my} };

    for (int i = 0; i < 4; ++i) {
        button_reward[i] = BoundingRectangle{ UIManager::CreateButtonBySize(0.1422f + i * 0.2045f, 0.32f, 256.0f, 384.0f) };
        if (mouse_pos.intersects(button_reward[i]) && gsGetMouseButtonDetach(GMOUSE_BUTTON_1)) {
            menu_ = SelectMenu::Reward;
            reward_ = static_cast<RewardMenu>(i);
            gsPlaySE(SE_Select);
        }
    }

    button_confirm_reward = BoundingRectangle{ UIManager::CreateButtonBySize(0.43f, 0.84f, 256.0f, 96.0f) };

    // メニューの処理
    if ((menu_ == SelectMenu::Confirm && gsGetKeyTrigger(GKEY_RETURN))
        || (mouse_pos.intersects(button_confirm_reward) && gsGetMouseButtonDetach(GMOUSE_BUTTON_1))) {
        switch (reward_) {
        case RewardMenu::Health:
            GameProgress::player_health_ += 500;
            break;
        case RewardMenu::Stamina:
            GameProgress::player_stamina_ += 40;
            break;
        case RewardMenu::Speed:
            GameProgress::player_speed_rate_ += 0.5f;
            break;
        case RewardMenu::Attack:
            GameProgress::player_attack_rate_ += 0.5f;
            break;
        }
        gsPlaySE(SE_Confirm);
        fade_->start_fade_out(90.0f);
        //is_end_ = true;
    }
}

void SelectRewardScene::draw() const {
    GScolor* grey = new GScolor{ 0.5f, 0.5f, 0.5f, 0.3f };
    GScolor transparent{ 0.9f, 0.6f, 0.9f, 0.4f };
    GScolor opaque{ 1.0f, 1.0f, 1.0f, 1.0f };

    switch (menu_) {
    case SelectMenu::Reward:   UIManager::DrawRectangleByPosition(0.0f, 0.25f, 1.0f, 0.75f, grey);    break;
    }

    UIManager::DrawSprite(Language::select_reward_image, 0.3125f, 0.12f, new GScolor(opaque));

    int mx, my;
    gsGetMouseCursorPosition(&mx, &my);
    BoundingRectangle mouse_pos{ GSvector2{(float)mx, (float)my}, GSvector2{(float)mx, (float)my} };

    for (int i = 0; i < 4; ++i) {
        GSuint image = Language::health_reward_image + i;;
        RewardMenu reward_menu = static_cast<RewardMenu>(i);
     
        UIManager::DrawSprite(image, 0.1422f + i * 0.2045f, 0.32f, new GScolor(UIManager::SwitchColor(reward_, reward_menu, transparent, opaque)));

        if (mouse_pos.intersects(button_reward[i])) UIManager::DrawRectangleBySize(0.1422f + i * 0.2045f, 0.32f, 256.0f, 384.0f, grey);
    }

    UIManager::DrawSprite(Language::confirm_image, 0.43f, 0.84f, new GScolor(UIManager::SwitchColor(menu_, SelectMenu::Confirm, transparent, opaque)));
    if (mouse_pos.intersects(button_confirm_reward)) UIManager::DrawRectangleBySize(0.43f, 0.84f, 256.0f, 96.0f, grey);

    fade_->draw();
}

bool SelectRewardScene::is_end() const {
    return is_end_;
}

std::string SelectRewardScene::next() const {
    return "SelectWeaponScene";
}

void SelectRewardScene::end() {
    gsDeleteTexture(Image_ConfirmEN);
    gsDeleteTexture(Image_ConfirmJP);
    gsDeleteTexture(Image_HealthRewardEN);
    gsDeleteTexture(Image_HealthRewardJP);
    gsDeleteTexture(Image_StaminaRewardEN);
    gsDeleteTexture(Image_StaminaRewardJP);
    gsDeleteTexture(Image_SpeedRewardEN);
    gsDeleteTexture(Image_SpeedRewardJP);
    gsDeleteTexture(Image_AttackRewardEN);
    gsDeleteTexture(Image_AttackRewardJP);
    gsDeleteTexture(Image_SelectRewardEN);
    gsDeleteTexture(Image_SelectRewardJP);
    gsDeleteSE(SE_Select);
    gsDeleteSE(SE_Confirm);
}

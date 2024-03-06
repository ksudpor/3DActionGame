#include "GameOverScene.h"
#include "Language.h"
#include "Assets.h"
#include "UIManager.h"
#include "GameProgress.h"

#define MAX_STAGE GameProgress::stage_ == GameProgress::StageProgress::Three

BoundingRectangle button_retry;
BoundingRectangle gameover_button_return;
BoundingRectangle button_retry_reset;

void GameOverScene::start(){
    gsLoadTexture(Image_GameOverEN, "Assets/Image/gameover_en.png");
    gsLoadTexture(Image_GameOverJP, "Assets/Image/gameover_jp.png");
    gsLoadTexture(Image_ReturnTitleEN, "Assets/Image/return_title_en.png");
    gsLoadTexture(Image_ReturnTitleJP, "Assets/Image/return_title_jp.png");
    gsLoadTexture(Image_RetryEN, "Assets/Image/retry_en.png");
    gsLoadTexture(Image_RetryJP, "Assets/Image/retry_jp.png");
    gsLoadTexture(Image_RetryResetEN, "Assets/Image/retry_reset_en.png");
    gsLoadTexture(Image_RetryResetJP, "Assets/Image/retry_reset_jp.png");
    gsLoadBGM(BGM_GamePlay, "Assets/Sound/bgmGameplay.ogg", GS_TRUE);
    gsLoadSE(SE_Select, "Assets/Sound/seSelect.wav", 1, GWAVE_DEFAULT);
    gsLoadSE(SE_Confirm, "Assets/Sound/seConfirm.wav", 1, GWAVE_DEFAULT);

    gsPlayBGM(BGM_GamePlay);
    is_end_ = false;
    fade_ = new Fade();
    fade_->start_fade_in(60.0f);
}

void GameOverScene::update(float delta_time){
    fade_->update(delta_time);
    if (fade_->end_scene()) is_end_ = true;
    if (fade_->is_fading()) return;
   
    // キーボード操作
    std::string next;
    if (gsGetKeyTrigger(GKEY_LEFT)) {
        if (MAX_STAGE) {
            UIManager::DecrementEnum(menu_, 0, 2);
        }
        else {
            UIManager::DecrementEnum(menu_, 0, 1);
        }
        gsPlaySE(SE_Select);
    }
    if (gsGetKeyTrigger(GKEY_RIGHT)) {
        if (MAX_STAGE) {
            UIManager::IncrementEnum(menu_, 0, 2);
        }
        else {
            UIManager::IncrementEnum(menu_, 0, 1);
        }
        gsPlaySE(SE_Select);
    }
    
    switch (menu_) {
    case GameOverMenu::Retry:       next = "SelectWeaponScene";    break;
    case GameOverMenu::ReturnTitle: next = "TitleScene";           break;
    case GameOverMenu::RetryReset:  next = "SelectWeaponScene";    break;
    }

    if (gsGetKeyTrigger(GKEY_RETURN)) {
        if (menu_ == GameOverMenu::RetryReset) GameProgress::stage_ = GameProgress::StageProgress::One;

        next_ = next;
        gsPlaySE(SE_Confirm);
        gsStopBGM();
        fade_->start_fade_out(90.0f);
        //is_end_ = true;
    }

    // マウス操作
    int mx, my;
    gsGetMouseCursorPosition(&mx, &my);
    BoundingRectangle mouse_pos{ GSvector2{(float)mx, (float)my}, GSvector2{(float)mx, (float)my} };

    button_retry = UIManager::CreateButtonBySize(0.2f, 0.8f, 400.0f, 80.0f);
    gameover_button_return = UIManager::CreateButtonBySize(0.6f, 0.8f, 400.0f, 80.0f);
    button_retry_reset = UIManager::CreateButtonBySize(0.2f, 0.9f, 400.0f, 80.0f);

    if (MAX_STAGE) {
    }

    if (mouse_pos.intersects(button_retry) && gsGetMouseButtonDetach(GMOUSE_BUTTON_1)) {
        next_ = "SelectWeaponScene";
        gsPlaySE(SE_Confirm);
        gsStopBGM();
        fade_->start_fade_out(90.0f);
        //is_end_ = true;
    }
    if (mouse_pos.intersects(gameover_button_return) && gsGetMouseButtonDetach(GMOUSE_BUTTON_1)) {
        next_ = "TitleScene";
        gsPlaySE(SE_Confirm);
        gsStopBGM();
        fade_->start_fade_out(90.0f);
        //is_end_ = true;
    }
    if (mouse_pos.intersects(button_retry_reset) && gsGetMouseButtonDetach(GMOUSE_BUTTON_1)) {
        GameProgress::stage_ = GameProgress::StageProgress::One;
        next_ = "SelectWeaponScene";
        gsPlaySE(SE_Confirm);
        gsStopBGM();
        fade_->start_fade_out(90.0f);
        //is_end_ = true;
    }
}

void GameOverScene::draw() const{
    GScolor* grey = new GScolor{ 0.5f, 0.5f, 0.5f, 0.3f };
    GScolor transparent{ 0.9f, 0.6f, 0.9f, 0.4f };
    GScolor opaque{ 1.0f, 1.0f, 1.0f, 1.0f };

    UIManager::DrawSprite(Language::game_over_image, 0.0f, 0.0f);
    UIManager::DrawSprite(Language::retry_image, 0.2f, 0.8f, &UIManager::SwitchColor(menu_, GameOverMenu::Retry, transparent, opaque));
    UIManager::DrawSprite(Language::return_title_image, 0.6f, 0.8f, &UIManager::SwitchColor(menu_, GameOverMenu::ReturnTitle, transparent, opaque));
    if (MAX_STAGE) UIManager::DrawSprite(Language::retry_reset_image, 0.2f, 0.9f, &UIManager::SwitchColor(menu_, GameOverMenu::RetryReset, transparent, opaque));

    int mx, my;
    gsGetMouseCursorPosition(&mx, &my);
    BoundingRectangle mouse_pos{ GSvector2{(float)mx, (float)my}, GSvector2{(float)mx, (float)my} };

    if (mouse_pos.intersects(button_retry)) UIManager::DrawRectangleBySize(0.2f, 0.8f, 400.0f, 80.0f, grey);
    if (mouse_pos.intersects(gameover_button_return)) UIManager::DrawRectangleBySize(0.6f, 0.8f, 400.0f, 80.0f, grey);
    if (MAX_STAGE) {
        if (mouse_pos.intersects(button_retry_reset)) UIManager::DrawRectangleBySize(0.2f, 0.9f, 400.0f, 80.0f, grey);
    }
    fade_->draw();
}

bool GameOverScene::is_end() const{
    return is_end_;
}

std::string GameOverScene::next() const{
    return next_;
}

void GameOverScene::end(){
    gsDeleteTexture(Image_GameOverEN);
    gsDeleteTexture(Image_GameOverJP);
    gsDeleteTexture(Image_ReturnTitleEN);
    gsDeleteTexture(Image_ReturnTitleJP);
    gsDeleteTexture(Image_RetryEN);
    gsDeleteTexture(Image_RetryJP);
    gsDeleteTexture(Image_RetryResetEN);
    gsDeleteTexture(Image_RetryResetJP);
    gsDeleteBGM(BGM_GamePlay);
    gsDeleteSE(SE_Select);
    gsDeleteSE(SE_Confirm);
}

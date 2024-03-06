#include "GameClearScene.h"
#include "Language.h"
#include "Assets.h"
#include "UIManager.h"
#include "GameProgress.h"

BoundingRectangle gameclear_button_return;

void GameClearScene::start(){
    gsLoadTexture(Image_GameClearEN, "Assets/Image/gameclear_en.png");
    gsLoadTexture(Image_GameClearJP, "Assets/Image/gameclear_jp.png");
    gsLoadTexture(Image_ReturnTitleEN, "Assets/Image/return_title_en.png");
    gsLoadTexture(Image_ReturnTitleJP, "Assets/Image/return_title_jp.png");
    gsLoadBGM(BGM_GamePlay, "Assets/Sound/bgmGameplay.ogg", GS_TRUE);
    gsLoadSE(SE_Confirm, "Assets/Sound/seConfirm.wav", 1, GWAVE_DEFAULT);

    gsPlayBGM(BGM_GamePlay);
    GameProgress::stage_ = GameProgress::StageProgress::One;
    is_end_ = false;
    fade_ = new Fade();
    fade_->start_fade_in(60.0f);
}

void GameClearScene::update(float delta_time){
    fade_->update(delta_time);
    if (fade_->end_scene()) is_end_ = true;
    if (fade_->is_fading()) return;

    // ƒ}ƒEƒX‘€ì
    int mx, my;
    gsGetMouseCursorPosition(&mx, &my);
    BoundingRectangle mouse_pos{ GSvector2{(float)mx, (float)my}, GSvector2{(float)mx, (float)my} };

    gameclear_button_return = UIManager::CreateButtonBySize(0.4f, 0.8f, 400.0f, 80.0f);

    if ((mouse_pos.intersects(gameclear_button_return) && gsGetMouseButtonDetach(GMOUSE_BUTTON_1)) || gsGetKeyTrigger(GKEY_RETURN)) {
        gsPlaySE(SE_Confirm);
        gsStopBGM();
        fade_->start_fade_out(90.0f);
        //is_end_ = true;
    }
    GameProgress::stage_ = GameProgress::StageProgress::One;
}

void GameClearScene::draw() const{
    GScolor* grey = new GScolor{ 0.5f, 0.5f, 0.5f, 0.3f };

    UIManager::DrawSprite(Language::game_clear_image, 0.0f, 0.0f);
    UIManager::DrawSprite(Language::return_title_image, 0.4f, 0.8f);

    int mx, my;
    gsGetMouseCursorPosition(&mx, &my);
    BoundingRectangle mouse_pos{ GSvector2{(float)mx, (float)my}, GSvector2{(float)mx, (float)my} };

    if (mouse_pos.intersects(gameclear_button_return)) UIManager::DrawRectangleBySize(0.4f, 0.8f, 400.0f, 80.0f, grey);

    fade_->draw();
}

bool GameClearScene::is_end() const{
    return is_end_;
}

std::string GameClearScene::next() const
{
    return "TitleScene";
}

void GameClearScene::end(){
    gsDeleteTexture(Image_GameClearEN);
    gsDeleteTexture(Image_GameClearJP);
    gsDeleteTexture(Image_ReturnTitleEN);
    gsDeleteTexture(Image_ReturnTitleJP);
    gsDeleteBGM(BGM_GamePlay);
    gsDeleteSE(SE_Confirm);
    GameProgress::stage_ = GameProgress::StageProgress::One;
}

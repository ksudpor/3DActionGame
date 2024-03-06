#include "ResultScene.h"
#include <gslib.h>
#include "UIManager.h"
#include "GameProgress.h"
#include "Assets.h"
#include "Language.h"
#include "NumberTexture.h"
#include "Screen.h"

BoundingRectangle button_next;
BoundingRectangle result_button_return;

void ResultScene::start(){
    gsLoadTexture(Image_StageOneEN, "Assets/Image/stage_one_en.png");
    gsLoadTexture(Image_StageOneJP, "Assets/Image/stage_one_jp.png");
    gsLoadTexture(Image_StageTwoEN, "Assets/Image/stage_two_en.png");
    gsLoadTexture(Image_StageTwoJP, "Assets/Image/stage_two_jp.png");
    gsLoadTexture(Image_StageThreeEN, "Assets/Image/stage_three_en.png");
    gsLoadTexture(Image_StageThreeJP, "Assets/Image/stage_three_jp.png");
    gsLoadTexture(Image_ResultEN, "Assets/Image/result_en.png");
    gsLoadTexture(Image_ResultJP, "Assets/Image/result_jp.png");
    gsLoadTexture(Image_TimeSpentEN, "Assets/Image/time_spent_en.png");
    gsLoadTexture(Image_TimeSpentJP, "Assets/Image/time_spent_jp.png");
    gsLoadTexture(Image_TotalScoreEN, "Assets/Image/total_score_en.png");
    gsLoadTexture(Image_TotalScoreJP, "Assets/Image/total_score_jp.png");
    gsLoadTexture(Image_ReturnTitleEN, "Assets/Image/return_title_en.png");
    gsLoadTexture(Image_ReturnTitleJP, "Assets/Image/return_title_jp.png");
    gsLoadTexture(Image_NextEN, "Assets/Image/next_en.png");
    gsLoadTexture(Image_NextJP, "Assets/Image/next_jp.png");
    gsLoadTexture(Image_TextureNumber, "Assets/Image/texture_number.png");
    gsLoadTexture(Image_Colon, "Assets/Image/colon.png");
    gsLoadBGM(BGM_GamePlay, "Assets/Sound/bgmGameplay.ogg", GS_TRUE);
    gsLoadSE(SE_Select, "Assets/Sound/seSelect.wav", 1, GWAVE_DEFAULT);
    gsLoadSE(SE_Confirm, "Assets/Sound/seConfirm.wav", 1, GWAVE_DEFAULT);

    gsPlayBGM(BGM_GamePlay);
    is_end_ = false;

    fade_ = new Fade();
    fade_->start_fade_in(60.0f);
}

void ResultScene::update(float delta_time){
    fade_->update(delta_time);
    if (fade_->end_scene()) {
        int cast = static_cast<int>(GameProgress::stage_);
        ++cast;
        if (cast >= GameProgress::max_stage_) {
            cast = 0;
            next_ = "GameClearScene";
        }
        GameProgress::stage_ = static_cast<GameProgress::StageProgress>(cast);
        is_end_ = true;
    }
    if (fade_->is_fading()) return;

    // キーボード操作
    std::string next;

    if (gsGetKeyTrigger(GKEY_LEFT)) {
        UIManager::DecrementEnum(menu_, 0, 1);
        gsPlaySE(SE_Select);
    }
    if (gsGetKeyTrigger(GKEY_RIGHT)) {
        UIManager::IncrementEnum(menu_, 0, 1);
        gsPlaySE(SE_Select);
    }

    switch (menu_) {
    case ResultMenu::NextStage:   next = "SelectWeaponScene";    break;
    case ResultMenu::ReturnTitle: next = "TitleScene";           break;
    }

    if (gsGetKeyTrigger(GKEY_RETURN)) {
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

    button_next = UIManager::CreateButtonBySize(0.2f, 0.8f, 400.0f, 80.0f);
    result_button_return = UIManager::CreateButtonBySize(0.6f, 0.8f, 400.0f, 80.0f);

    if (mouse_pos.intersects(button_next) && gsGetMouseButtonDetach(GMOUSE_BUTTON_1)) {
        gsPlaySE(SE_Confirm);
        next_ = "SelectRewardScene";
        fade_->start_fade_out(90.0f);
        gsStopBGM();
        //is_end_ = true;
    }
    if (mouse_pos.intersects(result_button_return) && gsGetMouseButtonDetach(GMOUSE_BUTTON_1)) {
        gsPlaySE(SE_Confirm);
        next_ = "TitleScene";
        fade_->start_fade_out(90.0f);
        gsStopBGM();
        //is_end_ = true;
    }
}

void ResultScene::draw() const{
    UIManager::DrawSprite(GameProgress::stage_image(GameProgress::stage_), 0.1f, 0.04f);
    UIManager::DrawSprite(Language::result_image, 0.15f, 0.12f);

    UIManager::DrawSprite(Language::time_spent_image, 0.15f, 0.34f);
    UIManager::DrawSprite(Language::total_score_image, 0.15f, 0.5f);

    float sec = GameProgress::time_stage / 60.0f;
    int second = (int)sec % 60;
    int minute = (int)sec / 60;

    NumberTexture number{ Image_TextureNumber, 64, 64 };
    number.set_scale(1.75f);
    number.draw(GSvector2{ Screen::Width * 0.54f, Screen::Height * 0.5f }, GameProgress::score_stage, 5);
    number.draw(GSvector2{ Screen::Width * 0.54f, Screen::Height * 0.34f }, minute, 2, '0');
    number.draw(GSvector2{ Screen::Width * 0.68f, Screen::Height * 0.34f }, second, 2, '0');
    GSvector2 colon_scale{ Screen::Scale * 1.75f };
    GSvector2 colon_pos{ Screen::Width * 0.64f, Screen::Height * 0.34f };
    gsDrawSprite2D(Image_Colon, &colon_pos, NULL, NULL, NULL, &colon_scale, 0.0f);

    GScolor* grey = new GScolor{ 0.5f, 0.5f, 0.5f, 0.3f };
    GScolor transparent{ 0.9f, 0.6f, 0.9f, 0.4f };
    GScolor opaque{ 1.0f, 1.0f, 1.0f, 1.0f };

    UIManager::DrawSprite(Language::next_image, 0.2f, 0.8f, &UIManager::SwitchColor(menu_, ResultMenu::NextStage, transparent, opaque));
    UIManager::DrawSprite(Language::return_title_image, 0.6f, 0.8f, &UIManager::SwitchColor(menu_, ResultMenu::ReturnTitle, transparent, opaque));

    int mx, my;
    gsGetMouseCursorPosition(&mx, &my);
    BoundingRectangle mouse_pos{ GSvector2{(float)mx, (float)my}, GSvector2{(float)mx, (float)my} };

    if (mouse_pos.intersects(button_next)) UIManager::DrawRectangleBySize(0.2f, 0.8f, 400.0f, 80.0f, grey);
    if (mouse_pos.intersects(result_button_return)) UIManager::DrawRectangleBySize(0.6f, 0.8f, 400.0f, 80.0f, grey);
    fade_->draw();
}

bool ResultScene::is_end() const{
    return is_end_;
}

std::string ResultScene::next() const{
    return next_;
}

void ResultScene::end(){
    gsDeleteTexture(Image_StageOneEN);
    gsDeleteTexture(Image_StageOneJP);
    gsDeleteTexture(Image_StageTwoEN);
    gsDeleteTexture(Image_StageTwoJP);
    gsDeleteTexture(Image_StageThreeEN);
    gsDeleteTexture(Image_StageThreeJP);
    gsDeleteTexture(Image_ResultEN);
    gsDeleteTexture(Image_ResultJP);
    gsDeleteTexture(Image_TimeSpentEN);
    gsDeleteTexture(Image_TimeSpentJP);
    gsDeleteTexture(Image_TotalScoreEN);
    gsDeleteTexture(Image_TotalScoreJP);
    gsDeleteTexture(Image_ReturnTitleEN);
    gsDeleteTexture(Image_ReturnTitleJP);
    gsDeleteTexture(Image_NextEN);
    gsDeleteTexture(Image_NextJP);
    gsDeleteTexture(Image_TextureNumber);
    gsDeleteTexture(Image_Colon);
    gsDeleteBGM(BGM_GamePlay);
    gsDeleteSE(SE_Select);
    gsDeleteSE(SE_Confirm);
}
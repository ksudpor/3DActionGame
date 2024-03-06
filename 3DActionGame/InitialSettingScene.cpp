#include "InitialSettingScene.h"
#include "Language.h"
#include "GameProgress.h"
#include "UIManager.h"
#include "Assets.h"
#include <gslib.h>

BoundingRectangle select_english;
BoundingRectangle select_japanese;

void InitialSettingScene::start(){
    gsLoadTexture(Image_EnglishOption, "Assets/Image/english_option.png");
    gsLoadTexture(Image_JapaneseOption, "Assets/Image/japanese_option.png");
    gsLoadSE(SE_Select, "Assets/Sound/seSelect.wav", 1, GWAVE_DEFAULT);
    gsLoadSE(SE_Confirm, "Assets/Sound/seConfirm.wav", 1, GWAVE_DEFAULT);
    is_end_ = false;

    fade_ = new Fade();
}

void InitialSettingScene::update(float delta_time){
    fade_->update(delta_time);
    if (fade_->end_scene()) is_end_ = true;
    if (fade_->is_fading()) return;

    // キーボード操作
    if (gsGetKeyTrigger(GKEY_UP)) {
        gsPlaySE(SE_Select);
        UIManager::DecrementEnum(GameProgress::lang_menu_, 0, 1);
    }
    if (gsGetKeyTrigger(GKEY_DOWN)) {
        gsPlaySE(SE_Select);
        UIManager::IncrementEnum(GameProgress::lang_menu_, 0, 1);
    }

    switch (GameProgress::lang_menu_) {
    case GameProgress::LangMenu::Japanese:
        if (gsGetKeyTrigger(GKEY_RETURN)) {
            gsPlaySE(SE_Confirm);
            Language::change_language(Language::Select::Japanese); 
            fade_->start_fade_out(90.0f);
            //is_end_ = true;
        }
        break;
    case GameProgress::LangMenu::English:
        if (gsGetKeyTrigger(GKEY_RETURN)) {
            gsPlaySE(SE_Confirm);
            Language::change_language(Language::Select::English);
            fade_->start_fade_out(90.0f);
            //is_end_ = true;
        }
        break;
    }

    // マウス操作
    int mx, my;
    gsGetMouseCursorPosition(&mx, &my);
    BoundingRectangle mouse_pos{ GSvector2{(float)mx, (float)my}, GSvector2{(float)mx, (float)my} };

    select_english = BoundingRectangle{ UIManager::CreateButtonBySize(0.4f, 0.55f, 400.0f, 80.0f) };
    select_japanese = BoundingRectangle{ UIManager::CreateButtonBySize(0.4f, 0.42f, 400.0f, 80.0f) };

    if (mouse_pos.intersects(select_english) && gsGetMouseButtonDetach(GMOUSE_BUTTON_1)) {
        gsPlaySE(SE_Confirm);
        Language::change_language(Language::Select::English);
        fade_->start_fade_out(90.0f);
        //is_end_ = true;
    }
    if (mouse_pos.intersects(select_japanese) && gsGetMouseButtonDetach(GMOUSE_BUTTON_1)) {
        gsPlaySE(SE_Confirm);
        Language::change_language(Language::Select::Japanese);
        fade_->start_fade_out(90.0f);
        //is_end_ = true;
    }

    
}

void InitialSettingScene::draw() const{
    GScolor* grey = new GScolor{ 0.5f, 0.5f, 0.5f, 0.3f };
    GScolor transparent{ 0.9f, 0.6f, 0.9f, 0.4f };
    GScolor opaque{ 1.0f, 1.0f, 1.0f, 1.0f };

    UIManager::DrawSprite(Image_JapaneseOption, 0.4f, 0.42f, &UIManager::SwitchColor(GameProgress::lang_menu_, GameProgress::LangMenu::Japanese, transparent, opaque));
    UIManager::DrawSprite(Image_EnglishOption, 0.4f, 0.55f, &UIManager::SwitchColor(GameProgress::lang_menu_, GameProgress::LangMenu::English, transparent, opaque));

    int mx, my;
    gsGetMouseCursorPosition(&mx, &my);
    BoundingRectangle mouse_pos{ GSvector2{(float)mx, (float)my}, GSvector2{(float)mx, (float)my} };

    if (mouse_pos.intersects(select_english)) UIManager::DrawRectangleBySize(0.4f, 0.55f, 384.0f, 80.0f, grey);
    if (mouse_pos.intersects(select_japanese)) UIManager::DrawRectangleBySize(0.4f, 0.42f, 384.0f, 80.0f, grey);
    fade_->draw();
}

bool InitialSettingScene::is_end() const{
    return is_end_;
}

std::string InitialSettingScene::next() const{
    return "TitleScene";
}

void InitialSettingScene::end(){
    gsDeleteTexture(Image_EnglishOption);
    gsDeleteTexture(Image_JapaneseOption);
    gsDeleteSE(SE_Select);
    gsDeleteSE(SE_Confirm);
}

#include "TitleScene.h"
#include "Screen.h"
#include "BoundingRectangle.h"
#include "Assets.h";
#include "Language.h"
#include "UIManager.h"
#include <GLFW/glfw3.h>

void TitleScene::start(){
    // 画像の読み込み
    gsLoadTexture(Image_Background, "Assets/Image/background.png");
    gsLoadTexture(Image_Title, "Assets/Image/title.png");
    gsLoadTexture(Image_PlayIconEN, "Assets/Image/icon_play_en.png");
    gsLoadTexture(Image_OptionIconEN, "Assets/Image/icon_option_en.png");
    gsLoadTexture(Image_QuitIconEN, "Assets/Image/icon_quit_en.png");
    gsLoadTexture(Image_PlayIconJP, "Assets/Image/icon_play_jp.png");
    gsLoadTexture(Image_OptionIconJP, "Assets/Image/icon_option_jp.png");
    gsLoadTexture(Image_QuitIconJP, "Assets/Image/icon_quit_jp.png");
    gsLoadSE(SE_Select, "Assets/Sound/seSelect.wav", 1, GWAVE_DEFAULT);
    gsLoadSE(SE_Confirm, "Assets/Sound/seConfirm.wav", 1, GWAVE_DEFAULT);
    gsLoadBGM(BGM_GamePlay, "Assets/Sound/bgmGameplay.ogg", GS_TRUE);
    gsPlayBGM(BGM_GamePlay);

    menu_ = TitleMenu::Play;
    is_end_ = false;

    fade_ = new Fade();
    fade_->start_fade_in(60.0f);
}

void TitleScene::update(float delta_time){
    fade_->update(delta_time);
    if (fade_->end_scene()) {
        is_end_ = true;
    }
    if (fade_->is_fading()) return;

    // キーボード操作
    if (gsGetKeyTrigger(GKEY_UP)) {
        UIManager::DecrementEnum(menu_, 0, 2);
        gsPlaySE(SE_Select);
    }
    if (gsGetKeyTrigger(GKEY_DOWN)) {
        UIManager::IncrementEnum(menu_, 0, 2);
        gsPlaySE(SE_Select);
    }
    if (gsGetKeyTrigger(GKEY_RETURN)){
        if (menu_ == TitleMenu::Quit) {
            glfwTerminate();
            throw std::runtime_error("Force Close");
        }
        else {
            gsPlaySE(SE_Confirm);
            gsStopBGM();
            fade_->start_fade_out(90.0f);
            //is_end_ = true;
        }
    }

    // マウス操作
    int mx, my;
    gsGetMouseCursorPosition(&mx, &my);
    BoundingRectangle mouse_position{ GSvector2{(float)mx, (float)my}, GSvector2{(float)mx, (float)my} };

    BoundingRectangle play_button{ UIManager::CreateButtonBySize(0.72f, 0.42f, 384.0f, 128.0f) };
    BoundingRectangle option_button{ UIManager::CreateButtonBySize(0.72f, 0.58f, 384.0f, 128.0f) };
    BoundingRectangle quit_button{ UIManager::CreateButtonBySize(0.72f, 0.74f, 384.0f, 128.0f) };
    
    if (mouse_position.intersects(play_button)) {
        menu_ = TitleMenu::Play;
        if (gsGetMouseButtonDetach(GMOUSE_BUTTON_1)) {
            gsPlaySE(SE_Confirm);
            gsStopBGM();
            fade_->start_fade_out(90.0f);
            //is_end_ = true;
        }
    }
    if (mouse_position.intersects(option_button)) {
        menu_ = TitleMenu::Option;
        if (gsGetMouseButtonDetach(GMOUSE_BUTTON_1)) {
            gsPlaySE(SE_Confirm);
            gsStopBGM();
            fade_->start_fade_out(90.0f);
            //is_end_ = true;
        }
    }
    if (mouse_position.intersects(quit_button)) {
        menu_ = TitleMenu::Quit;;
        if (gsGetMouseButtonDetach(GMOUSE_BUTTON_1)) {
            glfwTerminate();
            throw std::runtime_error("Force Close");
        }
    }

    // メニューの処理
    switch (menu_) {
    case TitleMenu::Play:   next_ = "SelectWeaponScene";    break;
    case TitleMenu::Option: next_ = "OptionScene";          break;
    }

    scroll_ += 0.25f * delta_time;
}

void TitleScene::draw() const{
    //gsFontParameter(0, Screen::Height * 0.15, "ＭＳ ゴシック");
    //gsTextPos(Screen::Width * 0.08f, Screen::Height * 0.18f);
    //gsDrawText("Space Fighter");

    GScolor white{ 1.0f, 1.0f, 1.0f, 1.0f };
    GScolor grey{ 0.5f, 0.5f, 0.5f, 1.0 };

    GSrect src_rect{ scroll_, scroll_, 1980.0f + scroll_, 1080.0f + scroll_ };
    UIManager::DrawSpriteRect(Image_Background, 0.0f, 0.0f, &src_rect);
    UIManager::DrawSprite(Image_Title, 0.05f, 0.0f);
    UIManager::DrawSprite(Language::play_icon_image, 0.72f, 0.42f, &UIManager::SwitchColor(menu_, TitleMenu::Play, grey, white));
    UIManager::DrawSprite(Language::option_icon_image, 0.72f, 0.58f, &UIManager::SwitchColor(menu_, TitleMenu::Option, grey, white));
    UIManager::DrawSprite(Language::quit_icon_image, 0.72f, 0.74f, &UIManager::SwitchColor(menu_, TitleMenu::Quit, grey, white));

    //Mouse Position Debug
    //gsFontParameter(0, 16, "ＭＳ ゴシック");
    //gsTextPos(20, 20);
    //gsDrawText("%d , %d", (mx), (my));
    //float x = (float)mx / (float)Screen::Width;
    //float y = (float)my / (float)Screen::Height;
    //gsFontParameter(0, 16, "ＭＳ ゴシック");
    //gsTextPos(20, 40);
    //gsDrawText("%f , %f", x, y);
    fade_->draw();
}

bool TitleScene::is_end() const{
    return is_end_;
}

std::string TitleScene::next() const{
    return next_;
}

void TitleScene::end(){
    // 画像の削除
    gsDeleteTexture(Image_Background);
    gsDeleteTexture(Image_Title);
    gsDeleteTexture(Image_PlayIconEN);
    gsDeleteTexture(Image_OptionIconEN);
    gsDeleteTexture(Image_QuitIconEN);
    gsDeleteTexture(Image_PlayIconJP);
    gsDeleteTexture(Image_OptionIconJP);
    gsDeleteTexture(Image_QuitIconJP);
    gsDeleteSE(SE_Select);
    gsDeleteSE(SE_Confirm);
    gsDeleteBGM(BGM_GamePlay);
}
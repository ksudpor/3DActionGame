#include "PauseScreen.h"
#include "Screen.h"
#include <gslib.h>
#include "Language.h"
#include "UIManager.h"
#include "Assets.h"
#include "GameProgress.h"

BoundingRectangle button_640;
BoundingRectangle button_800;
BoundingRectangle button_1024;
BoundingRectangle button_1280;
BoundingRectangle button_1600;
BoundingRectangle button_1920;
BoundingRectangle button_window;
BoundingRectangle button_fullscreen;
BoundingRectangle button_bgm;
BoundingRectangle button_se;
BoundingRectangle button_english;
BoundingRectangle button_japanese;
BoundingRectangle button_apply;
BoundingRectangle option_button_return;
float bgm_volume = 1.0f;
float se_volume = 1.0f;

void PauseScreen::start(){
    gsLoadTexture(Image_OptionEN, "Assets/Image/option_en.png");
    gsLoadTexture(Image_OptionJP, "Assets/Image/option_jp.png");
    gsLoadTexture(Image_ResolutionEN, "Assets/Image/resolution_en.png");
    gsLoadTexture(Image_ResolutionJP, "Assets/Image/resolution_jp.png");
    gsLoadTexture(Image_ScreenModeEN, "Assets/Image/screen_mode_en.png");
    gsLoadTexture(Image_ScreenModeJP, "Assets/Image/screen_mode_jp.png");
    gsLoadTexture(Image_BgmEN, "Assets/Image/bgm_en.png");
    gsLoadTexture(Image_BgmJP, "Assets/Image/bgm_jp.png");
    gsLoadTexture(Image_SeEN, "Assets/Image/se_en.png");
    gsLoadTexture(Image_SeJP, "Assets/Image/se_jp.png");
    gsLoadTexture(Image_LanguageEN, "Assets/Image/language_en.png");
    gsLoadTexture(Image_LanguageJP, "Assets/Image/language_jp.png");
    gsLoadTexture(Image_ApplyEN, "Assets/Image/apply_en.png");
    gsLoadTexture(Image_ApplyJP, "Assets/Image/apply_jp.png");
    gsLoadTexture(Image_ReturnTitleEN, "Assets/Image/return_title_en.png");
    gsLoadTexture(Image_ReturnTitleJP, "Assets/Image/return_title_jp.png");
    gsLoadTexture(Image_CheckControlEN, "Assets/Image/check_control_en.png");
    gsLoadTexture(Image_CheckControlJP, "Assets/Image/check_control_jp.png");
    gsLoadTexture(Image_640x480, "Assets/Image/640x480.png");
    gsLoadTexture(Image_800x600, "Assets/Image/800x600.png");
    gsLoadTexture(Image_1024x768, "Assets/Image/1024x768.png");
    gsLoadTexture(Image_1280x720, "Assets/Image/1280x720.png");
    gsLoadTexture(Image_1600x900, "Assets/Image/1600x900.png");
    gsLoadTexture(Image_1920x1080, "Assets/Image/1920x1080.png");
    gsLoadTexture(Image_WindowEN, "Assets/Image/window_en.png");
    gsLoadTexture(Image_WindowJP, "Assets/Image/window_jp.png");
    gsLoadTexture(Image_FullscreenEN, "Assets/Image/fullscreen_en.png");
    gsLoadTexture(Image_FullscreenJP, "Assets/Image/fullscreen_jp.png");
    gsLoadTexture(Image_EnglishOption, "Assets/Image/english_option.png");
    gsLoadTexture(Image_JapaneseOption, "Assets/Image/japanese_option.png");
    gsLoadTexture(Image_SoundBar, "Assets/Image/sound_bar.png");
    gsLoadSE(SE_Select, "Assets/Sound/seSelect.wav", 1, GWAVE_DEFAULT);
    gsLoadSE(SE_Confirm, "Assets/Sound/seConfirm.wav", 1, GWAVE_DEFAULT);
}

void PauseScreen::update(float delta_time){
    if (is_end_) return;
    int res_width;
    int res_height;
    bool fullscreen_mode;

    // キーボード操作
    if (gsGetKeyTrigger(GKEY_UP)) {
        UIManager::DecrementEnum(menu_, 0, 5);
        gsPlaySE(SE_Select);
    }
    if (gsGetKeyTrigger(GKEY_DOWN)) {
        UIManager::IncrementEnum(menu_, 0, 5);
        gsPlaySE(SE_Select);
    }

    switch (menu_) {
    case OptionMenu::Resolution:
        if (gsGetKeyTrigger(GKEY_LEFT)) {
            UIManager::DecrementEnum(GameProgress::resolution_, 0, 5);
            gsPlaySE(SE_Select);
        }
        if (gsGetKeyTrigger(GKEY_RIGHT)) {
            UIManager::IncrementEnum(GameProgress::resolution_, 0, 5);
            gsPlaySE(SE_Select);
        }
        break;
    case OptionMenu::ScreenMode:
        if (gsGetKeyTrigger(GKEY_LEFT)) {
            UIManager::DecrementEnum(GameProgress::screen_mode_, 0, 1);
            gsPlaySE(SE_Select);
        }
        if (gsGetKeyTrigger(GKEY_RIGHT)) {
            UIManager::IncrementEnum(GameProgress::screen_mode_, 0, 1);
            gsPlaySE(SE_Select);
        }
        break;
    case OptionMenu::BGM:
        if (gsGetKeyState(GKEY_LEFT)) {
            bgm_volume -= 0.01f * delta_time; bgm_volume = CLAMP(bgm_volume, 0.0f, 1.0f); gsSetVolumeBGM(bgm_volume);
        }
        if (gsGetKeyState(GKEY_RIGHT)) {
            bgm_volume += 0.01f * delta_time; bgm_volume = CLAMP(bgm_volume, 0.0f, 1.0f); gsSetVolumeBGM(bgm_volume);
        }
        if (gsGetKeyDetach(GKEY_LEFT) || gsGetKeyDetach(GKEY_RIGHT)) gsPlaySE(SE_Select);
        break;
    case OptionMenu::SE:
        if (gsGetKeyState(GKEY_LEFT)) {
            se_volume -= 0.01f; se_volume = CLAMP(se_volume, 0.0f, 1.0f); gsSetMasterVolumeSE(se_volume); 
        }
        if (gsGetKeyState(GKEY_RIGHT)) {
            se_volume += 0.01f; se_volume = CLAMP(se_volume, 0.0f, 1.0f); gsSetMasterVolumeSE(se_volume);
        }
        if (gsGetKeyDetach(GKEY_LEFT) || gsGetKeyDetach(GKEY_RIGHT)) gsPlaySE(SE_Select);
        break;
    case OptionMenu::LangMenu:
        if (gsGetKeyTrigger(GKEY_LEFT)) {
            UIManager::DecrementEnum(GameProgress::lang_menu_, 0, 1);
            gsPlaySE(SE_Select);
        }
        if (gsGetKeyTrigger(GKEY_RIGHT)) {
            UIManager::IncrementEnum(GameProgress::lang_menu_, 0, 1);
            gsPlaySE(SE_Select);
        }
        break;
    case OptionMenu::EndMenu:
        if (gsGetKeyTrigger(GKEY_LEFT)) {
            UIManager::DecrementEnum(end_menu_, 0, 1);
            gsPlaySE(SE_Select);
        }
        if (gsGetKeyTrigger(GKEY_RIGHT)) {
            UIManager::IncrementEnum(end_menu_, 0, 1);
            gsPlaySE(SE_Select);
        }
        break;
    }

    // マウス操作
    int mx, my;
    gsGetMouseCursorPosition(&mx, &my);
    BoundingRectangle mouse_pos{ GSvector2{(float)mx, (float)my}, GSvector2{(float)mx, (float)my} };

    button_640 = UIManager::CreateButtonBySize(0.28f, 0.18f, 128.0f, 108.0f);
    button_800 = UIManager::CreateButtonBySize(0.351f, 0.18f, 128.0f, 108.0f);
    button_1024 = UIManager::CreateButtonBySize(0.422f, 0.18f, 128.0f, 108.0f);
    button_1280 = UIManager::CreateButtonBySize(0.493f, 0.18f, 128.0f, 108.0f);
    button_1600 = UIManager::CreateButtonBySize(0.564f, 0.18f, 128.0f, 108.0f);
    button_1920 = UIManager::CreateButtonBySize(0.635f, 0.18f, 128.0f, 108.0f);
    button_window = UIManager::CreateButtonBySize(0.28f, 0.3325f, 384.0f, 80.0f);
    button_fullscreen = UIManager::CreateButtonBySize(0.5f, 0.3325f, 384.0f, 80.0f);
    button_bgm = UIManager::CreateButtonBySize(0.28f, 0.485f, 800.0f, 60.0f);
    button_se = UIManager::CreateButtonBySize(0.28f, 0.625f, 800.0f, 60.0f);
    button_english = UIManager::CreateButtonBySize(0.5f, 0.7525f, 384.0f, 80.0f);
    button_japanese = UIManager::CreateButtonBySize(0.28f, 0.7525f, 384.0f, 80.0f);
    button_apply = UIManager::CreateButtonBySize(0.125f, 0.9f, 400.0f, 80.0f);
    option_button_return = UIManager::CreateButtonBySize(0.425f, 0.9f, 400.0f, 80.0f);

    if (mouse_pos.intersects(button_640) && gsGetMouseButtonDetach(GMOUSE_BUTTON_1)) {
        GameProgress::resolution_ = GameProgress::Resolution::Res640; gsPlaySE(SE_Select);
    }
    if (mouse_pos.intersects(button_800) && gsGetMouseButtonDetach(GMOUSE_BUTTON_1)) {
        GameProgress::resolution_ = GameProgress::Resolution::Res800; gsPlaySE(SE_Select);
    }
    if (mouse_pos.intersects(button_1024) && gsGetMouseButtonDetach(GMOUSE_BUTTON_1)) {
        GameProgress::resolution_ = GameProgress::Resolution::Res1024; gsPlaySE(SE_Select);
    }
    if (mouse_pos.intersects(button_1280) && gsGetMouseButtonDetach(GMOUSE_BUTTON_1)) {
        GameProgress::resolution_ = GameProgress::Resolution::Res1280; gsPlaySE(SE_Select);
    }
    if (mouse_pos.intersects(button_1600) && gsGetMouseButtonDetach(GMOUSE_BUTTON_1)) {
        GameProgress::resolution_ = GameProgress::Resolution::Res1600; gsPlaySE(SE_Select);
    }
    if (mouse_pos.intersects(button_1920) && gsGetMouseButtonDetach(GMOUSE_BUTTON_1)) {
        GameProgress::resolution_ = GameProgress::Resolution::Res1920; gsPlaySE(SE_Select);
    }

    if (mouse_pos.intersects(button_window) && gsGetMouseButtonDetach(GMOUSE_BUTTON_1)) {
        GameProgress::screen_mode_ = GameProgress::ScreenMode::Window; gsPlaySE(SE_Select);
    }
    if (mouse_pos.intersects(button_fullscreen) && gsGetMouseButtonDetach(GMOUSE_BUTTON_1)) {
        GameProgress::screen_mode_ = GameProgress::ScreenMode::Fullscreen; gsPlaySE(SE_Select);
    }

    if (mouse_pos.intersects(button_bgm) && gsGetMouseButtonDetach(GMOUSE_BUTTON_1)) {
        bgm_volume = ((float)mx - (0.28f * Screen::Width)) / (800.0f * Screen::Width / 1920.0f);
        bgm_volume = CLAMP(bgm_volume, 0.0f, 1.0f); gsSetVolumeBGM(bgm_volume);
        gsPlaySE(SE_Select);
    }
    if (mouse_pos.intersects(button_se) && gsGetMouseButtonDetach(GMOUSE_BUTTON_1)) {
        se_volume = ((float)mx - (0.28f * Screen::Width)) / (800.0f * Screen::Width / 1920.0f);
        se_volume = CLAMP(se_volume, 0.0f, 1.0f); gsSetMasterVolumeSE(se_volume);
        gsPlaySE(SE_Select);
    }

    if (mouse_pos.intersects(button_english) && gsGetMouseButtonDetach(GMOUSE_BUTTON_1)) {
        GameProgress::lang_menu_ = GameProgress::LangMenu::English; gsPlaySE(SE_Select);
    }
    if (mouse_pos.intersects(button_japanese) && gsGetMouseButtonDetach(GMOUSE_BUTTON_1)) {
        GameProgress::lang_menu_ = GameProgress::LangMenu::Japanese; gsPlaySE(SE_Select);
    }

    // メニューの処理
    switch (GameProgress::resolution_) {
    case GameProgress::Resolution::Res640:    res_width = 640;    res_height = 480;   break;
    case GameProgress::Resolution::Res800:    res_width = 800;    res_height = 600;   break;
    case GameProgress::Resolution::Res1024:   res_width = 1024;   res_height = 768;   break;
    case GameProgress::Resolution::Res1280:   res_width = 1280;   res_height = 720;   break;
    case GameProgress::Resolution::Res1600:   res_width = 1600;   res_height = 900;   break;
    case GameProgress::Resolution::Res1920:   res_width = 1920;   res_height = 1080;  break;
    }

    switch (GameProgress::screen_mode_) {
    case GameProgress::ScreenMode::Window:        fullscreen_mode = false;    break;
    case GameProgress::ScreenMode::Fullscreen:    fullscreen_mode = true;     break;
    }

    if (end_menu_ == EndMenu::ApplyOption && gsGetKeyTrigger(GKEY_RETURN) ||
        mouse_pos.intersects(button_apply) && gsGetMouseButtonDetach(GMOUSE_BUTTON_1)) {
        GLFWwindow* window = gslib::Window::instance().get_glfw_window();
        Screen::Is_Fullscreen = fullscreen_mode;
        Screen::Width = res_width;
        Screen::Height = res_height;
        float y_pos = Screen::Is_Fullscreen ? 0 : 23;
        GLFWmonitor* monitor = Screen::Is_Fullscreen ? glfwGetPrimaryMonitor() : NULL;
        Screen::Scale = GSvector2{ (float)Screen::Width / 1920.0f, (float)Screen::Height / 1080.0f };
        glfwSetWindowMonitor(window, monitor, 0, y_pos, Screen::Width, Screen::Height, Screen::Refresh_Rate);

        switch (GameProgress::lang_menu_) {
        case GameProgress::LangMenu::English:     Language::change_language(Language::Select::English);   break;
        case GameProgress::LangMenu::Japanese:    Language::change_language(Language::Select::Japanese);  break;
        }

        gsPlaySE(SE_Confirm);
    }
    else if (end_menu_ == EndMenu::ReturnTitle && gsGetKeyTrigger(GKEY_RETURN) ||
        mouse_pos.intersects(option_button_return) && gsGetMouseButtonDetach(GMOUSE_BUTTON_1)) {
        gsPlaySE(SE_Confirm);
        is_end_ = true;
    }
}

void PauseScreen::draw() const{
    GScolor* grey = new GScolor{ 0.5f, 0.5f, 0.5f, 0.3f };
    GScolor transparent{ 0.9f, 0.6f, 0.9f, 0.4f };
    GScolor opaque{ 1.0f, 1.0f, 1.0f, 1.0f };

    UIManager::DrawRectangleByPosition(0.0f, 0.0f, 1.0f, 1.0f, new GScolor(0.0f, 0.0f, 0.0f, 0.9f));

    UIManager::DrawSprite(Language::option_image, 0.08f, 0.04f);

    switch (menu_) {
    case OptionMenu::Resolution:    UIManager::DrawRectangleByPosition(0.0f, 0.16f, 0.75f, 0.3f, grey);    break;
    case OptionMenu::ScreenMode:    UIManager::DrawRectangleByPosition(0.0f, 0.3f, 0.75f, 0.44f, grey);    break;
    case OptionMenu::BGM:           UIManager::DrawRectangleByPosition(0.0f, 0.44f, 0.75f, 0.58f, grey);   break;
    case OptionMenu::SE:            UIManager::DrawRectangleByPosition(0.0f, 0.58f, 0.75f, 0.72f, grey);   break;
    case OptionMenu::LangMenu:      UIManager::DrawRectangleByPosition(0.0f, 0.72f, 0.75f, 0.86f, grey);   break;
    case OptionMenu::EndMenu:       UIManager::DrawRectangleByPosition(0.0f, 0.86f, 0.75f, 1.0f, grey);    break;
    }

    UIManager::DrawSprite(Language::resolution_image, 0.05f, 0.2f);
    UIManager::DrawSprite(Language::screenmode_image, 0.05f, 0.34f);
    UIManager::DrawSprite(Language::bgm_image, 0.05f, 0.48f);
    UIManager::DrawSprite(Language::se_image, 0.05f, 0.62f);
    UIManager::DrawSprite(Language::language_image, 0.05f, 0.76f);

    UIManager::DrawSprite(Image_640x480, 0.28f, 0.18f, new GScolor(UIManager::SwitchColor(GameProgress::resolution_, GameProgress::Resolution::Res640, transparent, opaque)));
    UIManager::DrawSprite(Image_800x600, 0.351f, 0.18f, new GScolor(UIManager::SwitchColor(GameProgress::resolution_, GameProgress::Resolution::Res800, transparent, opaque)));
    UIManager::DrawSprite(Image_1024x768, 0.422f, 0.18f, new GScolor(UIManager::SwitchColor(GameProgress::resolution_, GameProgress::Resolution::Res1024, transparent, opaque)));
    UIManager::DrawSprite(Image_1280x720, 0.493f, 0.18f, new GScolor(UIManager::SwitchColor(GameProgress::resolution_, GameProgress::Resolution::Res1280, transparent, opaque)));
    UIManager::DrawSprite(Image_1600x900, 0.564f, 0.18f, new GScolor(UIManager::SwitchColor(GameProgress::resolution_, GameProgress::Resolution::Res1600, transparent, opaque)));
    UIManager::DrawSprite(Image_1920x1080, 0.635f, 0.18f, new GScolor(UIManager::SwitchColor(GameProgress::resolution_, GameProgress::Resolution::Res1920, transparent, opaque)));

    UIManager::DrawSprite(Language::window_image, 0.28f, 0.3325f, new GScolor(UIManager::SwitchColor(GameProgress::screen_mode_, GameProgress::ScreenMode::Window, transparent, opaque)));
    UIManager::DrawSprite(Language::fullscreen_image, 0.5f, 0.3325f, new GScolor(UIManager::SwitchColor(GameProgress::screen_mode_, GameProgress::ScreenMode::Fullscreen, transparent, opaque)));

    UIManager::DrawSprite(Image_JapaneseOption, 0.28f, 0.7525f, new GScolor(UIManager::SwitchColor(GameProgress::lang_menu_, GameProgress::LangMenu::Japanese, transparent, opaque)));
    UIManager::DrawSprite(Image_EnglishOption, 0.5f, 0.7525f, new GScolor(UIManager::SwitchColor(GameProgress::lang_menu_, GameProgress::LangMenu::English, transparent, opaque)));

    GSrect bgm_rect{ 0.0f, 0.0f, bgm_volume * 800.0f, 60.0f};
    GSrect se_rect{ 0.0f, 0.0f, se_volume * 800.0f, 60.0f};
    UIManager::DrawSpriteRect(Image_SoundBar, 0.28f, 0.485f, &bgm_rect, NULL);
    UIManager::DrawSpriteRect(Image_SoundBar, 0.28f, 0.625f, &se_rect, NULL);

    UIManager::DrawSprite(Language::apply_image, 0.125f, 0.9f, &UIManager::SwitchColor(end_menu_, EndMenu::ApplyOption, transparent, opaque));
    UIManager::DrawSprite(Language::return_title_image, 0.425f, 0.9f, &UIManager::SwitchColor(end_menu_, EndMenu::ReturnTitle, transparent, opaque));

    UIManager::DrawSprite(Language::check_control_image, 0.75f, 0.0f);

    int mx, my;
    gsGetMouseCursorPosition(&mx, &my);
    BoundingRectangle mouse_pos{ GSvector2{(float)mx, (float)my}, GSvector2{(float)mx, (float)my} };

    if (mouse_pos.intersects(button_640)) UIManager::DrawRectangleBySize(0.28f, 0.18f, 128.0f, 108.0f, grey);
    if (mouse_pos.intersects(button_800)) UIManager::DrawRectangleBySize(0.351f, 0.18f, 128.0f, 108.0f, grey);
    if (mouse_pos.intersects(button_1024)) UIManager::DrawRectangleBySize(0.422f, 0.18f, 128.0f, 108.0f, grey);
    if (mouse_pos.intersects(button_1280)) UIManager::DrawRectangleBySize(0.493f, 0.18f, 128.0f, 108.0f, grey);
    if (mouse_pos.intersects(button_1600)) UIManager::DrawRectangleBySize(0.564f, 0.18f, 128.0f, 108.0f, grey);
    if (mouse_pos.intersects(button_1920)) UIManager::DrawRectangleBySize(0.635f, 0.18f, 128.0f, 108.0f, grey);

    if (mouse_pos.intersects(button_window)) UIManager::DrawRectangleBySize(0.28f, 0.3325f, 384.0f, 80.0f, grey);
    if (mouse_pos.intersects(button_fullscreen)) UIManager::DrawRectangleBySize(0.5f, 0.3325f, 384.0f, 80.0f, grey);

    if (mouse_pos.intersects(button_bgm)) UIManager::DrawRectangleByPosition(0.28f, 0.485f, (float)mx / Screen::Width, 0.485f + (60.0f / 1080.0f), grey);
    if (mouse_pos.intersects(button_se)) UIManager::DrawRectangleByPosition(0.28f, 0.625f, (float)mx / Screen::Width, 0.625f + (60.0f / 1080.0f), grey);

    if (mouse_pos.intersects(button_english)) UIManager::DrawRectangleBySize(0.5f, 0.7525f, 384.0f, 80.0f, grey);
    if (mouse_pos.intersects(button_japanese)) UIManager::DrawRectangleBySize(0.28f, 0.7525f, 384.0f, 80.0f, grey);

    if (mouse_pos.intersects(button_apply)) UIManager::DrawRectangleBySize(0.125f, 0.9f, 400.0f, 80.0f, grey);
    if (mouse_pos.intersects(option_button_return)) UIManager::DrawRectangleBySize(0.425f, 0.9f, 400.0f, 80.0f, grey);
}

void PauseScreen::end(){
    gsDeleteTexture(Image_OptionEN);
    gsDeleteTexture(Image_OptionJP);
    gsDeleteTexture(Image_ResolutionEN);
    gsDeleteTexture(Image_ResolutionJP);
    gsDeleteTexture(Image_ScreenModeEN);
    gsDeleteTexture(Image_ScreenModeJP);
    gsDeleteTexture(Image_BgmEN);
    gsDeleteTexture(Image_BgmJP);
    gsDeleteTexture(Image_SeEN);
    gsDeleteTexture(Image_SeJP);
    gsDeleteTexture(Image_LanguageEN);
    gsDeleteTexture(Image_LanguageJP);
    gsDeleteTexture(Image_ApplyEN);
    gsDeleteTexture(Image_ApplyJP);
    gsDeleteTexture(Image_ReturnTitleEN);
    gsDeleteTexture(Image_ReturnTitleJP);
    gsDeleteTexture(Image_CheckControlEN);
    gsDeleteTexture(Image_CheckControlJP);
    gsDeleteTexture(Image_640x480);
    gsDeleteTexture(Image_800x600);
    gsDeleteTexture(Image_1024x768);
    gsDeleteTexture(Image_1280x720);
    gsDeleteTexture(Image_1600x900);
    gsDeleteTexture(Image_1920x1080);
    gsDeleteTexture(Image_WindowEN);
    gsDeleteTexture(Image_WindowJP);
    gsDeleteTexture(Image_FullscreenEN);
    gsDeleteTexture(Image_FullscreenJP);
    gsDeleteTexture(Image_EnglishOption);
    gsDeleteTexture(Image_JapaneseOption);
    gsDeleteSE(SE_Select);
    gsDeleteSE(SE_Confirm);
}

bool PauseScreen::is_end() const{
    return is_end_;
}

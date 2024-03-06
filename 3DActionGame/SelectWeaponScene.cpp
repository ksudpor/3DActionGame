#include "SelectWeaponScene.h"
#include "Screen.h"
#include "Assets.h"
#include "Language.h"
#include "GameProgress.h"
#include "UIManager.h"
#include "Fade.h"

BoundingRectangle button_weapon1[6];
BoundingRectangle button_weapon2[6];
BoundingRectangle button_confirm_weapon;

void SelectWeaponScene::start(){
    // 画像の読み込み
    gsLoadTexture(Image_Axe, "Assets/Image/axe_icon.png");
    gsLoadTexture(Image_SwordShield, "Assets/Image/swordshield_icon.png");
    gsLoadTexture(Image_GreatSword, "Assets/Image/greatsword_icon.png");
    gsLoadTexture(Image_Pistol, "Assets/Image/pistol_icon.png");
    gsLoadTexture(Image_Rifle, "Assets/Image/rifle_icon.png");
    gsLoadTexture(Image_Shotgun, "Assets/Image/shotgun_icon.png");
    gsLoadTexture(Image_ConfirmEN, "Assets/Image/confirm_en.png");
    gsLoadTexture(Image_ConfirmJP, "Assets/Image/confirm_jp.png");
    gsLoadTexture(Image_WeaponOneEN, "Assets/Image/weapon_one_en.png");
    gsLoadTexture(Image_WeaponOneJP, "Assets/Image/weapon_one_jp.png");
    gsLoadTexture(Image_WeaponTwoEN, "Assets/Image/weapon_two_en.png");
    gsLoadTexture(Image_WeaponTwoJP, "Assets/Image/weapon_two_jp.png");
    gsLoadTexture(Image_StageOneEN, "Assets/Image/stage_one_en.png");
    gsLoadTexture(Image_StageOneJP, "Assets/Image/stage_one_jp.png");
    gsLoadTexture(Image_StageTwoEN, "Assets/Image/stage_two_en.png");
    gsLoadTexture(Image_StageTwoJP, "Assets/Image/stage_two_jp.png");
    gsLoadTexture(Image_StageThreeEN, "Assets/Image/stage_three_en.png");
    gsLoadTexture(Image_StageThreeJP, "Assets/Image/stage_three_jp.png");
    gsLoadTexture(Image_SelectWeaponEN, "Assets/Image/select_weapon_en.png");
    gsLoadTexture(Image_SelectWeaponJP, "Assets/Image/select_weapon_jp.png");
    gsLoadTexture(Image_SameWeaponEN, "Assets/Image/same_weapon_en.png");
    gsLoadTexture(Image_SameWeaponJP, "Assets/Image/same_weapon_jp.png");
    gsLoadSE(SE_Select, "Assets/Sound/seSelect.wav", 1, GWAVE_DEFAULT);
    gsLoadSE(SE_Confirm, "Assets/Sound/seConfirm.wav", 1, GWAVE_DEFAULT);

    is_end_ = false;
    menu_ = SelectMenu::Weapon1;
    same_weapon_ = false;

    fade_ = new Fade();
    fade_->start_fade_in(60.0f);
}

void SelectWeaponScene::update(float delta_time){
    fade_->update(delta_time);
    if (fade_->end_scene()) is_end_ = true;
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

    switch (menu_) {
    case SelectMenu::Weapon1:
        if (gsGetKeyTrigger(GKEY_LEFT)) {
            UIManager::DecrementEnum(weapon1_, 0, 5);
            gsPlaySE(SE_Select);
        }
        if (gsGetKeyTrigger(GKEY_RIGHT)) {
            UIManager::IncrementEnum(weapon1_, 0, 5);
            gsPlaySE(SE_Select);
        }
        break;
    case SelectMenu::Weapon2:
        if (gsGetKeyTrigger(GKEY_LEFT)) {
            UIManager::DecrementEnum(weapon2_, 0, 5);
            gsPlaySE(SE_Select);
        }
        if (gsGetKeyTrigger(GKEY_RIGHT)) {
            UIManager::IncrementEnum(weapon2_, 0, 5);
            gsPlaySE(SE_Select);
        }
        break;
    }

    // マウス操作
    int mx, my;
    gsGetMouseCursorPosition(&mx, &my);
    BoundingRectangle mouse_pos{ GSvector2{(float)mx, (float)my}, GSvector2{(float)mx, (float)my} };

    for (int i = 0; i < 6; ++i) {
        button_weapon1[i] = BoundingRectangle{ UIManager::CreateButtonBySize(0.32f + i * 0.1f, 0.31f, 128.0f, 128.0f) };
        button_weapon2[i] = BoundingRectangle{ UIManager::CreateButtonBySize(0.32f + i * 0.1f, 0.56f, 128.0f, 128.0f) };
        if (mouse_pos.intersects(button_weapon1[i]) && gsGetMouseButtonDetach(GMOUSE_BUTTON_1)) {
            menu_ = SelectMenu::Weapon1;
            weapon1_ = static_cast<WeaponMenu>(i);
            gsPlaySE(SE_Select);
        }
        if (mouse_pos.intersects(button_weapon2[i]) && gsGetMouseButtonDetach(GMOUSE_BUTTON_1)) {
            menu_ = SelectMenu::Weapon2;
            weapon2_ = static_cast<WeaponMenu>(i);
            gsPlaySE(SE_Select);
        }
    }

    button_confirm_weapon = BoundingRectangle{ UIManager::CreateButtonBySize(0.43f, 0.84f, 256.0f, 96.0f) };

    // メニューの処理
    if ((menu_ == SelectMenu::Confirm && gsGetKeyTrigger(GKEY_RETURN)) 
        || (mouse_pos.intersects(button_confirm_weapon) && gsGetMouseButtonDetach(GMOUSE_BUTTON_1))){
        if (weapon1_ == weapon2_) {
            same_weapon_ = true;
        }
        else{
            GameProgress::primary_weapon = confirm_weapon(weapon1_);
            GameProgress::secondary_weapon = confirm_weapon(weapon2_);
            gsPlaySE(SE_Confirm);
            fade_->start_fade_out(90.0f);
            //is_end_ = true;
        }
    }
}

void SelectWeaponScene::draw() const{
    GScolor* grey = new GScolor{ 0.5f, 0.5f, 0.5f, 0.3f };
    GScolor transparent{ 0.9f, 0.6f, 0.9f, 0.4f };
    GScolor opaque{ 1.0f, 1.0f, 1.0f, 1.0f };

    switch (menu_){
    case SelectMenu::Weapon1:   UIManager::DrawRectangleByPosition(0.0f, 0.25f, 1.0f, 0.5f, grey);    break;
    case SelectMenu::Weapon2:   UIManager::DrawRectangleByPosition(0.0f, 0.5f, 1.0f, 0.75f, grey);    break;
    }

    UIManager::DrawSprite(GameProgress::stage_image(GameProgress::stage_), 0.1f, 0.04f, new GScolor(opaque));
    UIManager::DrawSprite(Language::select_weapon_image, 0.15f, 0.12f, new GScolor(opaque));

    UIManager::DrawSprite(Language::weapon1_image, 0.1f, 0.33f, new GScolor(opaque));
    UIManager::DrawSprite(Language::weapon2_image, 0.1f, 0.58f, new GScolor(opaque));

    int mx, my;
    gsGetMouseCursorPosition(&mx, &my);
    BoundingRectangle mouse_pos{ GSvector2{(float)mx, (float)my}, GSvector2{(float)mx, (float)my} };

    for (int i = 0; i < 6; ++i) {
        GSuint image = Image_Axe + i;;
        WeaponMenu weapon_menu = static_cast<WeaponMenu>(i);
        UIManager::DrawSprite(image, 0.32f + i * 0.1f, 0.31f, new GScolor(UIManager::SwitchColor(weapon1_, weapon_menu, transparent, opaque)));
        UIManager::DrawSprite(image, 0.32f + i * 0.1f, 0.56f, new GScolor(UIManager::SwitchColor(weapon2_, weapon_menu, transparent, opaque)));

        if (mouse_pos.intersects(button_weapon1[i])) UIManager::DrawRectangleBySize(0.32f + i * 0.1f, 0.31f, 128.0f, 128.0f, grey);
        if (mouse_pos.intersects(button_weapon2[i])) UIManager::DrawRectangleBySize(0.32f + i * 0.1f, 0.56f, 128.0f, 128.0f, grey);
    }

    UIManager::DrawSprite(Language::confirm_image, 0.43f, 0.84f, new GScolor(UIManager::SwitchColor(menu_, SelectMenu::Confirm, transparent, opaque)));
    if (mouse_pos.intersects(button_confirm_weapon)) UIManager::DrawRectangleBySize(0.43f, 0.84f, 256.0f, 96.0f, grey);

    if (same_weapon_) {
        UIManager::DrawSprite(Language::same_weapon_image, 0.27f, 0.77f, new GScolor(opaque));
    }

    fade_->draw();
}

bool SelectWeaponScene::is_end() const{
    return is_end_;
}

std::string SelectWeaponScene::next() const{
    return "GamePlayScene";
}

void SelectWeaponScene::end(){
    gsDeleteTexture(Image_Axe);
    gsDeleteTexture(Image_SwordShield);
    gsDeleteTexture(Image_GreatSword);
    gsDeleteTexture(Image_Pistol);
    gsDeleteTexture(Image_Rifle);
    gsDeleteTexture(Image_Shotgun);
    gsDeleteTexture(Image_ConfirmEN);
    gsDeleteTexture(Image_ConfirmJP);
    gsDeleteTexture(Image_WeaponOneEN);
    gsDeleteTexture(Image_WeaponOneJP);
    gsDeleteTexture(Image_WeaponTwoEN);
    gsDeleteTexture(Image_WeaponTwoJP);
    gsDeleteTexture(Image_StageOneEN);
    gsDeleteTexture(Image_StageOneJP);
    gsDeleteTexture(Image_StageTwoEN);
    gsDeleteTexture(Image_StageTwoJP);
    gsDeleteTexture(Image_StageThreeEN);
    gsDeleteTexture(Image_StageThreeJP);
    gsDeleteTexture(Image_SelectWeaponEN);
    gsDeleteTexture(Image_SelectWeaponJP);
    gsDeleteTexture(Image_SameWeaponEN);
    gsDeleteTexture(Image_SameWeaponJP);
    gsDeleteSE(SE_Select);
    gsDeleteSE(SE_Confirm);
}

GameProgress::Weapon SelectWeaponScene::confirm_weapon(WeaponMenu weapon){
    switch (weapon) {
    case SelectWeaponScene::WeaponMenu::Axe:
        return GameProgress::Weapon::Axe;            break;
    case SelectWeaponScene::WeaponMenu::SwordShield:
        return GameProgress::Weapon::SwordShield;    break;
    case SelectWeaponScene::WeaponMenu::GreatSword:
        return GameProgress::Weapon::GreatSword;     break;
    case SelectWeaponScene::WeaponMenu::Pistol:
        return GameProgress::Weapon::Pistol;         break;
    case SelectWeaponScene::WeaponMenu::Rifle:
        return GameProgress::Weapon::Rifle;          break;
    case SelectWeaponScene::WeaponMenu::Shotgun:
        return GameProgress::Weapon::Shotgun;        break;
    }
}

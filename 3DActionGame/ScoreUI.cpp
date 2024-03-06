#include "ScoreUI.h"
#include "NumberTexture.h"
#include "Screen.h"
#include "Assets.h"
#include <gslib.h>
#include <algorithm>    //std::min


ScoreUI::ScoreUI(int score) : 
    score_{ score } {
}

void ScoreUI::initialize(int score){
    score_ = score;
}

void ScoreUI::add(int score){
    score_ = std::min(score_ + score, 99999);
}

void ScoreUI::draw() const{
    NumberTexture number{ Image_TextureNumber, 64, 64 };
    number.set_scale(0.75f);
    number.draw(GSvector2{ Screen::Width * 0.85f, Screen::Height * 0.05f }, score_, 5);
    GSvector2 ui_scale{ Screen::Scale * 0.75f };
    GSvector2 ui_pos{ Screen::Width * 0.725f, Screen::Height * 0.05f };
    gsDrawSprite2D( Image_TextureScore, &ui_pos, NULL, NULL, NULL, &ui_scale, 0.0f);
}

int ScoreUI::get() const{
    return score_;
}

void ScoreUI::clear(){
    score_ = 0;
}

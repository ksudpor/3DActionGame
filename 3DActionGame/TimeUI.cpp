#include "TimeUI.h"
#include "NumberTexture.h"
#include "Screen.h"
#include "Assets.h"
#include <gslib.h>
#include <algorithm>

TimeUI::TimeUI(float time) {
    initialize(time);
}

void TimeUI::initialize(float time){
    time_ = time;
    used_time_ = 0.0f;
    max_time_ = time;
}

void TimeUI::update(float delta_time){
    time_ -= delta_time;
    used_time_ += delta_time;

    if (time_ <= 0.0f) time_ = 0.0f;
    if (used_time_ >= max_time_) used_time_ = max_time_;
}

void TimeUI::draw() const{
    NumberTexture number{ Image_TextureNumber, 64, 64 };
    number.set_scale(0.75f);
    float sec = time_ / 60.0f;
    int second = (int)sec % 60;
    int minute = (int)sec / 60;

    GScolor color;
    if (time_ < 1800.0f) {
        color = GScolor(0.8f, 0.2f, 0.2f, 1.0f);
    }
    else {
        color = GScolor(1.0f, 1.0f, 1.0f, 1.0f);
    }

    number.draw(GSvector2{ Screen::Width * 0.5f, Screen::Height * 0.05f }, minute, 2, '0', color);
    number.draw(GSvector2{ Screen::Width * 0.56f, Screen::Height * 0.05f }, second, 2, '0', color);
    GSvector2 ui_scale{ Screen::Scale * 0.75f };
    GSvector2 ui_pos{ Screen::Width * 0.4f, Screen::Height * 0.05f };
    gsDrawSprite2D(Image_TextureTime, &ui_pos, NULL, NULL, &color, &ui_scale, 0.0f);
    GSvector2 colon_scale{ Screen::Scale * 0.75f };
    GSvector2 colon_pos{ Screen::Width * 0.54f, Screen::Height * 0.05f };
    gsDrawSprite2D(Image_Colon, &colon_pos, NULL, NULL, &color, &colon_scale, 0.0f);
}

float TimeUI::get() const{
    return time_;
}

float TimeUI::spent_time() const{
    return used_time_;
}

void TimeUI::clear(){
    time_ = max_time_;
    used_time_ = 0.0f;
}

#include "Screen.h"
#include "Fade.h"

Fade::Fade() {
    fade_time_ = 0.0f;
    timer_ = 0.0f;
    fade_in_color_ = GScolor{ 0.0f, 0.0f, 0.0f, 1.0f };
    fade_out_color_ = GScolor{ 0.0f, 0.0f, 0.0f, 0.0f };
    is_fade_in_ = false;
    is_fade_out_ = false;
    is_fade_out_end_ = false;
}

void Fade::update(float delta_time){
    if (is_fade_in_) {
        timer_ -= delta_time;
        fade_in_color_.a -= delta_time / fade_time_;
    }
    if (is_fade_out_) {
        timer_ -= delta_time;
        fade_out_color_.a += delta_time / fade_time_;
    }
    
    if (timer_ <= 0) {
        is_fade_in_ = false;
        is_fade_out_ = false;
    }
}

void Fade::draw() const{
    if (is_fade_in_) {
        gsDrawRectangle2D(new GSrect(0.0f, 0.0f, Screen::Width, Screen::Height), &fade_in_color_);
    }
    if (is_fade_out_) {
        gsDrawRectangle2D(new GSrect(0.0f, 0.0f, Screen::Width, Screen::Height), &fade_out_color_);
    }
    //if (is_fade_out_end_) {
    //    gsDrawRectangle2D(new GSrect(0.0f, 0.0f, Screen::Width, Screen::Height), &fade_out_color_);
    //}
}

bool Fade::is_fading(){
    return is_fade_in_ || is_fade_out_;
}

void Fade::start_fade_in(float fade_time){
    fade_time_ = fade_time;
    timer_ = fade_time;
    is_fade_in_ = true;
}

void Fade::start_fade_out(float fade_time){
    fade_time_ = fade_time;
    timer_ = fade_time;
    is_fade_out_ = true;
    is_fade_out_end_ = true;
}

bool Fade::end_scene(){
    return !is_fade_out_ && is_fade_out_end_;
}

#include "OptionScene.h"
#include "Assets.h"

void OptionScene::start(){
    gsLoadBGM(BGM_GamePlay, "Assets/Sound/bgmGameplay.ogg", GS_TRUE);
    gsPlayBGM(BGM_GamePlay);

    is_end_ = false;
    pause_ = new PauseScreen();
    pause_->start();

    fade_ = new Fade();
    fade_->start_fade_in(60.0f);
}

void OptionScene::update(float delta_time){
    fade_->update(delta_time);
    if (fade_->end_scene()) is_end_ = true;
    if (fade_->is_fading()) return;

    pause_->update(delta_time);

    if (pause_->is_end()){
        gsStopBGM();
        fade_->start_fade_out(90.0f);
        //is_end_ = true;
    }
}

void OptionScene::draw() const{
    pause_->draw();
    fade_->draw();
}

bool OptionScene::is_end() const{
    return is_end_;
}

std::string OptionScene::next() const{
    return "TitleScene";
}

void OptionScene::end(){
    gsDeleteBGM(BGM_GamePlay);
    pause_->end();
}

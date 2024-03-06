#ifndef GAME_PLAY_SCENE_H_
#define GAME_PLAY_SCENE_H_

#include "IScene.h"
#include "World.h"
#include "PauseScreen.h"
#include "Fade.h"
#include "Stage.h"

// ゲームプレイシーン
class GamePlayScene : public IScene {
public:
	virtual void start() override;
	virtual void update(float delta_time) override;
	virtual void draw() const override;
	virtual bool is_end() const override;
	virtual std::string next() const override;
	virtual void end() override;

private:
	World world_;
	bool is_end_{ false };
	std::string next_;
	Actor* player_;
	Actor* stage_manager_;
	bool is_paused_;
	PauseScreen* pause_;
	Fade* fade_;
	bool is_play_se_;
};
#endif

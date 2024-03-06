#ifndef GAME_OVER_SCENE_H_
#define GAME_OVER_SCENE_H_

#include "IScene.h"
#include <gslib.h>
#include "Fade.h"

// ゲームオーバーシーン
class GameOverScene : public IScene {
	enum class GameOverMenu {
		Retry,
		ReturnTitle,
		RetryReset
	};

public:
	virtual void start() override;
	virtual void update(float delta_time) override;
	virtual void draw() const override;
	virtual bool is_end() const override;
	virtual std::string next() const override;
	virtual void end() override;

private:
	bool is_end_{ false };
	std::string next_;
	GameOverMenu menu_;
	Fade* fade_;
};


#endif

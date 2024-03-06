#ifndef RESULT_SCENE_H_
#define RESULT_SCENE_H_

#include "IScene.h"
#include <gslib.h>
#include "Fade.h"

// ゲームオーバーシーン
class ResultScene : public IScene {
	enum class ResultMenu {
		NextStage, 
		ReturnTitle
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
	ResultMenu menu_;
	Fade* fade_;
};


#endif
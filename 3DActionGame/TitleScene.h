#ifndef TITLE_SCENE_H_
#define TITLE_SCENE_H_

#include "IScene.h"
#include <gslib.h>
#include <GLFW/glfw3.h>
#include "Fade.h"

// タイトルシーン
class TitleScene : public IScene {
enum class TitleMenu {
	Play,
	Option,
	Quit
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
	TitleMenu menu_;
	Fade* fade_;
	float scroll_;
};

#endif
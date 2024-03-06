#ifndef SCENE_NULL_H_
#define SCENE_NULL_H_

#include "IScene.h"

// ƒkƒ‹ƒV[ƒ“
class SceneNull : public IScene {
public:
	virtual void start() override {}
	virtual void update(float delta_time) override {}
	virtual void draw() const override {}
	virtual bool is_end() const override {
		return false;
	}
	virtual std::string next() const override {
		return "SceneNull";
	}
	virtual void end() override {}
};

#endif

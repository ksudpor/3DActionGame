#ifndef STAGE_MANAGER_H_
#define STAGE_MANAGER_H_

#include "Actor.h"
#include "Stage.h"
#include "GameProgress.h"

class StageManager : public Actor {
public:
	StageManager(IWorld* world);
	virtual void update(float delt_time) override;
	virtual void draw() const override;
	virtual bool game_clear() const override;
	virtual bool game_over() const override;

private:
	Stage* stage_{ nullptr };
};

#endif
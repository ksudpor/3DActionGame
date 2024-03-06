#ifndef STAGE_ONE_
#define STAGE_ONE_

#include "Stage.h"
#include "Actor.h"

class StageOne : public Stage {
public:
	StageOne(IWorld* world);
	virtual void update(float delta_time) override;
	virtual void draw() override;
	virtual bool game_clear() override;
	virtual bool game_over() override;

private:
	IWorld* world_;
	float respawn_timer_[10];
	Actor* spawn_enemies_[10];
	int dead_count_;
};

#endif

#ifndef STAGE_TWO_H_
#define STAGE_TWO_H_

#include "Stage.h"
#include "Actor.h"

class StageTwo : public Stage {
public:
	StageTwo(IWorld* world);
	virtual void update(float delta_time) override;
	virtual void draw() override;
	virtual bool game_clear() override;
	virtual bool game_over() override;

private:
	IWorld* world_;
	float respawn_timer_[10];
	Actor* spawn_enemies_[10];
	Actor* keys_[10];
	int key_count_;
};

#endif

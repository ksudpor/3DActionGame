#ifndef STAGE_THree_H_
#define STAGE_TWO_H_

#include "Stage.h"
#include "Actor.h"

class StageThree : public Stage {
public:
	StageThree(IWorld* world);
	virtual void update(float delta_time) override;
	virtual void draw() override;
	virtual bool game_clear() override;
	virtual bool game_over() override;

private:
	IWorld* world_;
	float respawn_timer_[15];
	Actor* spawn_enemies_[15];
	Actor* keys_[5];
	Actor* goal_;
	Actor* player_;
};

#endif

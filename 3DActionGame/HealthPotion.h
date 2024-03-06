#ifndef HEALTH_POTION_H_
#define	HEALTH_POTION_H_

#include "Actor.h"

class HealthPotion : public Actor {
public:
	HealthPotion(IWorld* world, const GSvector3& position);
	virtual void update(float delta_time) override;
	virtual void draw() const override;
	virtual void react(Actor& other) override;

private:
	float lifespan_timer_;
	GSint effect_handle_;
};

#endif
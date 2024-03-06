#ifndef ENEMY_BULLET_H_
#define	ENEMY_BULLET_H_

#include "Actor.h"

class EnemyBullet : public Actor {
public:
	EnemyBullet(IWorld* world, const GSvector3& position, const GSquaternion& rotation, const GSvector3& velocity, int damage = 1);
	virtual void update(float delta_time) override;
	virtual void draw() const override;
	virtual void react(Actor& other) override;

private:
	float lifespan_timer_;
	GSint effect_handle_;
};

#endif

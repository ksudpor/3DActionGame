#ifndef ATTACK_COLLIDER_H_
#define ATTACK_COLLIDER_H_

#include "Actor.h"

// çUåÇîªíË
class AttackCollider : public Actor {
public:
	AttackCollider(IWorld* world, const BoundingSphere& collider,const std::string& tag,
		const std::string& name,const std::string& owner_tag = "",int damage = 1,
		float lifespan = 1.0f, float delay = 0.0f, GSquaternion rotation = GSquaternion{0.0f, 0.0f, 0.0f, 1.0f});
	virtual void update(float delta_time) override;
	virtual void draw() const override;
	virtual void react(Actor& other) override;

private:
	std::string owner_tag_;
	float lifespan_timer_;
	float delay_timer_;
};
#endif
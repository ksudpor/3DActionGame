#include "AttackCollider.h"

AttackCollider::AttackCollider(IWorld* world, const BoundingSphere& collider, 
	const std::string& tag, const std::string& name, const std::string& owner_tag, 
	int damage, float lifespan, float delay, GSquaternion rotation) : 
	lifespan_timer_{ lifespan }, delay_timer_{ delay } {
	world_ = world;
	tag_ = tag;
	owner_tag_ = owner_tag;
	name_ = name;
	collider_ = BoundingSphere{ collider.radius };
	transform_.position(collider.center);
	transform_.rotation(rotation);
	enable_collider_ = false;
	damage_ = damage;
}

void AttackCollider::update(float delta_time){
	if (delay_timer_ <= 0.0f) {
		enable_collider_ = true;
		if (lifespan_timer_ <= 0.0f) {
			die();
		}
		lifespan_timer_ -= delta_time;
	}
	delay_timer_ -= delta_time;
}

void AttackCollider::draw() const{
	//if (enable_collider_) collider().draw();
}

void AttackCollider::react(Actor& other){
	if (other.tag() == tag() || other.tag() == owner_tag_) return;
	die();
}

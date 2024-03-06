#include "Goal.h"
#include "IWorld.h"
#include "Assets.h"
#include <GSeffect.h>

Goal::Goal(IWorld* world, const GSvector3& position){
	world_ = world;
	tag_ = "GoalTag";
	name_ = "Goal";
	collider_ = BoundingSphere{ 1.5f };
	transform_.position(position + GSvector3{ 0.0f, 0.875f, 0.0f });
	GSmatrix4 localMat = GSmatrix4::TRS(GSvector3{ 0.0f, -0.875f, 0.0f },
		GSquaternion::euler(0.0f, 0.0f, 0.0f), GSvector3{ 2.0f, 2.0f, 2.0f });
	GSmatrix4 worldMat = localMat * transform_.localToWorldMatrix();
	effect_handle_ = gsPlayEffectEx(Effect_Goal, new GSmatrix4(worldMat));
}

void Goal::draw() const{
	//collider_.draw();
}

void Goal::react(Actor& other){
	if (other.tag() == "PlayerTag") {
		gsPlaySE(SE_Key);
		world_->add_score(1500);
		die();
		return;
	}
}

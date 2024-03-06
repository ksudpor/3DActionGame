#include "HealthPotion.h"
#include "Assets.h"
#include <GSeffect.h>

HealthPotion::HealthPotion(IWorld* world, const GSvector3& position){
	world_ = world;
	tag_ = "HealthPotionTag";
	name_ = "HealthPotion";
	collider_ = BoundingSphere{ 0.2f };
	transform_.position(position + GSvector3{ 0.0f, 0.875f, 0.0f });
	lifespan_timer_ = 3600.0f;
	GSmatrix4 localMat = GSmatrix4::TRS(GSvector3{ 0.0f, -0.875f, 0.0f }, 
		GSquaternion::euler( 0.0f, 0.0f, 0.0f ), GSvector3{ 0.5f, 0.5f, 0.5f });
	GSmatrix4 worldMat = localMat * transform_.localToWorldMatrix();
	effect_handle_ = gsPlayEffectEx(Effect_LootBeamRed, new GSmatrix4(worldMat));
}

void HealthPotion::update(float delta_time){
	if (lifespan_timer_ <= 0.0f) {
		gsStopEffect(effect_handle_);
		die();
		return;
	}
	lifespan_timer_ -= delta_time;
}

void HealthPotion::draw() const{
	glPushMatrix();
	glMultMatrixf(transform_.localToWorldMatrix());
	gsDrawMesh(Mesh_HealthPotion);
	glPopMatrix();
	//collider().draw();
}

void HealthPotion::react(Actor& other){
	if (other.tag() == "PlayerTag") {
		gsStopEffect(effect_handle_);
		die();
		return;
	}
}

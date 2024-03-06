#include "KeyItem.h"
#include "IWorld.h"
#include "Assets.h"
#include <GSeffect.h>

KeyItem::KeyItem(IWorld* world, const GSvector3& position){
	world_ = world;
	tag_ = "KeyItemTag";
	name_ = "KeyItem";
	collider_ = BoundingSphere{ 0.2f };
	transform_.position(position + GSvector3{ 0.0f, 0.875f, 0.0f });
	GSmatrix4 localMat = GSmatrix4::TRS(GSvector3{ 0.0f, -0.875f, 0.0f },
		GSquaternion::euler(0.0f, 0.0f, 0.0f), GSvector3{ 0.5f, 0.5f, 0.5f });
	GSmatrix4 worldMat = localMat * transform_.localToWorldMatrix();
	effect_handle_ = gsPlayEffectEx(Effect_LootBeamYellow, new GSmatrix4(worldMat));
}

void KeyItem::draw() const{
	glPushMatrix();
	glMultMatrixf(transform_.localToWorldMatrix());
	gsDrawMesh(Mesh_KeyItem);
	glPopMatrix();
}

void KeyItem::react(Actor& other){
	if (other.tag() == "PlayerTag") {
		gsStopEffect(effect_handle_);
		gsPlaySE(SE_Key);
		world_->add_score(150);
		die();
		return;
	}
}

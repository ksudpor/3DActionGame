#include "PlayerBullet.h"
#include "IWorld.h"
#include "Field.h"
#include "Line.h"
#include "Assets.h"

PlayerBullet::PlayerBullet(IWorld* world, const GSvector3& position, const GSquaternion& rotation, 
	const GSvector3& velocity, float timer, const GSuint& mesh, int damage){
	world_ = world;
	tag_ = "PlayerBulletTag";
	name_ = "PlayerBullet";
	velocity_ = velocity;
	collider_ = BoundingSphere{ 0.2f };
	transform_.position(position);
	transform_.rotation(rotation);
	lifespan_timer_ = timer;
	mesh_ = mesh;
	damage_ = damage;
}

void PlayerBullet::update(float delta_time){
	// フィールドとの衝突判定
	Line line;
	line.start = transform_.position();
	line.end = transform_.position() + velocity_ * delta_time;
	GSvector3 intersect;
	if (world_->field()->collide(line, &intersect)) {
		transform_.position(intersect);
		die();
		return;
	}

	// 寿命が尽きたら死亡
	if (lifespan_timer_ <= 0.0f) {
		die();
		return;
	}
	lifespan_timer_ -= delta_time;

	transform_.translate(velocity_ * delta_time, GStransform::Space::World);
}

void PlayerBullet::draw() const{
	glPushMatrix();
	glMultMatrixf(transform_.localToWorldMatrix());
	gsDrawMesh(mesh_);
	glPopMatrix();
	//collider().draw();
}

void PlayerBullet::react(Actor& other){
	if (other.tag() == tag() || other.tag() == "PlayerTag" || other.tag() == "EnemyBulletTag") return;
	die();
}

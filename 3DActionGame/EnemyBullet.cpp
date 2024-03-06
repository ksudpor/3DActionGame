#include "EnemyBullet.h"
#include "IWorld.h"
#include "Field.h"
#include "Line.h"
#include "Assets.h"
#include <GSeffect.h>

// 寿命
const float LifeSpanTime{ 60.0f };

EnemyBullet::EnemyBullet(IWorld* world, const GSvector3& position, const GSquaternion& rotation, const GSvector3& velocity, int damage){
	// ワールドを設定
	world_ = world;
	// タグ名
	tag_ = "EnemyBulletTag";
	// アクター名
	name_ = "EnemyBullet";
	// 移動量の初期化
	velocity_ = velocity;
	// 衝突判定用の球体を設定
	collider_ = BoundingSphere{ 0.2f };
	// 座標の初期化
	transform_.position(position);
	transform_.rotation(rotation);
	// 寿命
	lifespan_timer_ = LifeSpanTime;
	// エフェクトを再生（生成）する
	effect_handle_ = gsPlayEffect(Effect_BigArrowDark, &position);
	damage_ = damage;
}

void EnemyBullet::update(float delta_time){
	// 寿命が尽きたら死亡
	if (lifespan_timer_ <= 0.0f) {
		die();
		// エフェクトの停止（削除）
		gsStopEffect(effect_handle_);
		return;
	}
	// 寿命の更新
	lifespan_timer_ -= delta_time;
	// フィールドとの衝突判定
	Line line;
	line.start = transform_.position();
	line.end = transform_.position() + velocity_ * delta_time;
	GSvector3 intersect;
	if (world_->field()->collide(line, &intersect)) {
		// 交点の座標に補正
		transform_.position(intersect);
		// フィールドに衝突したら死亡
		velocity_ = GSvector3{ 0.0f, 0.0f, 0.0f };
		return;
	}
	// 移動する（ワールド座標系基準）
	transform_.translate(velocity_ * delta_time, GStransform::Space::World);
}

void EnemyBullet::draw() const{
	// エフェクトに自身のワールド変換行列を設定
	GSmatrix4 localMat = GSmatrix4::TRS(GSvector3{0.0f,0.0f,0.0f}, GSquaternion::euler(GSvector3(0.0f, 0.0f, 0.0f)), GSvector3{ 0.5f, 0.5f, 0.5f });
	GSmatrix4 worldMat = localMat * transform_.localToWorldMatrix();
	gsSetEffectMatrix(effect_handle_, &worldMat); // ワールド変換行列を設定
	// 寿命によって、だんだん透明にしていく（アルファ値を変換させる）
	GScolor start_color{ 1.0f, 1.0f, 1.0f, 1.0f };
	GScolor end_color{ 1.0f, 1.0f, 1.0f, 0.0f };
	GScolor color = GScolor::lerp(start_color, end_color, lifespan_timer_ / LifeSpanTime);
	gsSetEffectColor(effect_handle_, &color); // カラーを設定

	// デバッグ表示
	//collider().draw();
}

void EnemyBullet::react(Actor& other){
	if (other.tag() == tag() || other.tag() == "EnemyTag" || other.tag() == "PlayerBulletTag") return;
	if (other.tag() == "PlayerTag") {
		die();
		gsStopEffect(effect_handle_);
		return;
	}
	// 衝突したら移動しないようにする
	velocity_ = GSvector3{ 0.0f, 0.0f, 0.0f };
	// 無敵状態にする
	enable_collider_ = false;
}

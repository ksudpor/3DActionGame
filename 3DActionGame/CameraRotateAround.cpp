#include "CameraRotateAround.h"
#include "IWorld.h"
#include "Field.h"
#include "Line.h"


// プレーヤーからの相対位置（z座標のみ）
const GSvector3 PlayerOffset{ 0.0f, 0.0f, -5.0f };
// カメラの注視点の補正値
const GSvector3 ReferencePointOffset{ 0.0f, 1.8f, 0.0f };

CameraRotateAround::CameraRotateAround(IWorld* world, const GSvector3& position, const GSvector3& at){
	world_ = world;
	tag_ = "CameraTag";
	name_ = "Camera";
	transform_.position(position);
	transform_.lookAt(at);

	// ｘ軸周りの回転角度の初期化
	pitch_ = (at - position).getPitch();
	// ｙ軸周りの回転角度の初期化
	yaw_ = (at - position).getYaw();
}

void CameraRotateAround::update(float delta_time){
	Actor* player = world_->find_actor("Player");
	if (player == nullptr) return;

	//// y軸まわりにカメラを回転させる
	//if (gsGetKeyState(GKEY_LEFT)) yaw_ += 1.0f * delta_time;
	//if (gsGetKeyState(GKEY_RIGHT)) yaw_ -= 1.0f * delta_time;
	//// x軸まわりにカメラを回転させる
	//if (gsGetKeyState(GKEY_UP)) pitch_ += 1.0f * delta_time;
	//if (gsGetKeyState(GKEY_DOWN)) pitch_ -= 1.0f * delta_time;
	int mx, my;
	gsGetMouseVelocity(&mx, &my, NULL);
	const float MouseSensitivity{ 0.2f };
	yaw_ -= mx * delta_time * MouseSensitivity;
	pitch_ += my * delta_time * MouseSensitivity;

	// x軸まわりの回転角度の制限をする
	pitch_ = CLAMP(pitch_, -10.0f, 30.0f);

	GSvector3 at = player->transform().position() + ReferencePointOffset;
	GSvector3 position = at + GSquaternion::euler(pitch_, yaw_, 0.0f) * PlayerOffset;
	
	// フィールドとの衝突判定
	Line line{ at, position };
	GSvector3 intersect;
	if (world_->field()->collide(line, &intersect)) {
		position = intersect;
	}

	// スムースダンプによる滑らかな補間
	const float SmoothTime{ 12.0f }; // 補間フレーム数
	const float MaxSpeed{ 1.0f }; // 移動スピードの最大値
	position = GSvector3::smoothDamp(transform_.position(), position, velocity_,
		SmoothTime, MaxSpeed, delta_time);

	transform_.position(position);
	transform_.lookAt(at);

	gsUpdateFrustum();
}

void CameraRotateAround::draw() const{
	GSvector3 eye = transform_.position();
	GSvector3 at = eye + transform_.forward();
	GSvector3 up = transform_.up();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		eye.x, eye.y, eye.z, // 視点の位置
		at.x, at.y, at.z, // 注視点の位置
		up.x, up.y, up.z // 視点の上方向
	);
}

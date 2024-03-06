#include "Camera.h"
#include "IWorld.h"

// プレーヤーからの相対座標
const GSvector3 PlayerOffset{ 0.0f, 3.0f, -5.0f };
// カメラの注視点の補正値
const GSvector3 ReferencePointOffset{ 0.0f, 1.7f, 0.0f };

Camera::Camera(IWorld* world){
	world_ = world;
	name_ = "Camera";
	tag_ = "CameraTag";
}

void Camera::draw() const{
	// プレーヤーを検索
	Actor* player = world_->find_actor("Player");
	if (player == nullptr) return;
	// カメラの位置を求める
	GSvector3 eye = PlayerOffset * player->transform().localToWorldMatrix();
	// 注視点の位置を求める
	GSvector3 at = player->transform().position() + ReferencePointOffset;
	// OpenGLのカメラの設定
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		eye.x, eye.y, eye.z, // 視点の位置
		at.x, at.y, at.z, // 注視点の位置
		0.0f, 1.0f, 0.0f // 視点の上方向
	);

}

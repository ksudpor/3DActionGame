#include "CameraFPS.h"
#include "IWorld.h"
#include "Field.h"
#include "Line.h"

const GSvector3 PlayerOffset{ 0.0f, 1.8f, -0.1f };

CameraFPS::CameraFPS(IWorld* world){
	world_ = world;
	tag_ = "CameraTag";
	name_ = "Camera";
}

void CameraFPS::draw() const{
	Actor* player = world_->find_actor("Player");
	if (player == nullptr) return;

	GSvector3 eye = PlayerOffset * player->transform().localToWorldMatrix();
	GSvector3 at = eye + player->transform().forward();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		eye.x, eye.y, eye.z, // 視点の位置
		at.x, at.y, at.z, // 注視点の位置
		0.0f, 1.0f, 0.0f // 視点の上方向
	);
}

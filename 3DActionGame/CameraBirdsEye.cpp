#include "CameraBirdsEye.h"
#include "IWorld.h"

const GSvector3 PlayerOffset{ 0.0f, 15.0f, -2.0f };

CameraBirdsEye::CameraBirdsEye(IWorld* world){
	world_ = world;
	tag_ = "CameraTag";
	name_ = "Camera";
}

void CameraBirdsEye::draw() const{
	Actor* player = world_->find_actor("Player");
	if (player == nullptr) return;

	GSvector3 eye = player->transform().position() + PlayerOffset;
	GSvector3 at = player->transform().position();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		eye.x, eye.y, eye.z, // 視点の位置
		at.x, at.y, at.z, // 注視点の位置
		0.0f, 0.0f, 1.0f // 視点の上方向
	);
}

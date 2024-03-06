#include "CameraFixedPoint.h"

CameraFixedPoint::CameraFixedPoint(IWorld* world){
	world_ = world;
	tag_ = "CameraTag";
	name_ = "Camera";
}

void CameraFixedPoint::draw() const{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		10.0f, 10.0f, 10.0f, // 視点の位置
		0.0f, 0.0f, 0.0f, // 注視点の位置
		0.0f, 1.0f, 0.0f // 視点の上方向
	);
}

#ifndef CAMERA_FPS_H_
#define CAMERA_FPS_H_

#include "Actor.h"

class CameraFPS : public Actor {
public:
	CameraFPS(IWorld* world);
	virtual void draw() const override;
};
#endif

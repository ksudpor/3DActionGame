#ifndef CAMERA_H_
#define CAMERA_H_

#include "Actor.h"

// ƒJƒƒ‰ƒNƒ‰ƒX
class Camera : public Actor {
public:
	Camera(IWorld* world);
	virtual void draw() const override;
};
#endif
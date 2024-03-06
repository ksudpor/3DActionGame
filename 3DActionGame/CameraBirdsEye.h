#ifndef CAMERA_BIRDS_EYE_H_
#define CAMERA_BIRDS_EYE_H_

#include "Actor.h"

class CameraBirdsEye : public Actor {
public:
	CameraBirdsEye(IWorld* world);
	virtual void draw() const override;
};
#endif

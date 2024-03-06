#ifndef CAMERA_FIXED_POINT_H_
#define CAMERA_FIXED_POINT_H_

#include "Actor.h"

class CameraFixedPoint : public Actor {
public:
	CameraFixedPoint(IWorld* world);
	virtual void draw() const override;
};
#endif

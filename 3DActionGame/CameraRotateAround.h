#ifndef CAMERA_ROTATE_AROUND_H_
#define CAMERA_ROTATE_AROUND_H_

#include "Actor.h"

class CameraRotateAround : public Actor {
public:
	CameraRotateAround(IWorld* world, const GSvector3& position, const GSvector3& at);
	virtual void update(float delta_time) override;
	virtual void draw() const override;
private:
	float yaw_{ 0.0f };
	float pitch_{ 0.0f };
};
#endif
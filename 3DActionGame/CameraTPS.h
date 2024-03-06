#ifndef CAMERA_TPS_H_
#define CAMERA_TPS_H_

#include "Actor.h"

class CameraTPS : public Actor {
public:
	CameraTPS(IWorld* world, const GSvector3& position, const GSvector3& at);
	virtual void update(float delta_time) override;
	virtual void draw() const override;
};
#endif

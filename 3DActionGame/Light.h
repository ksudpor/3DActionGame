#ifndef LIGHT_H_
#define LIGHT_H_

#include "Actor.h"

// ���C�g�N���X
class Light : public Actor {
public:
	Light(IWorld* world);
	virtual void draw() const override;
};
#endif


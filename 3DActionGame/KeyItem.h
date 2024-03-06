#ifndef KEY_ITEM
#define KEY_ITEM

#include "Actor.h"

class KeyItem : public Actor {
public:
	KeyItem(IWorld* world, const GSvector3& position);
	virtual void draw() const override;
	virtual void react(Actor& other) override;

private:
	GSint effect_handle_;
};

#endif

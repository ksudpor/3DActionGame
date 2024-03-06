#ifndef GOAL_H_
#define GOAL_H_

#include "Actor.h"

class Goal : public Actor {
public:
	Goal(IWorld* world, const GSvector3& position);
	virtual void draw() const override;
	virtual void react(Actor& other) override;

private:
	GSint effect_handle_;

};


#endif
#ifndef PLAYER_BULLET_H_
#define PLAYER_BULLET_H_

#include "Actor.h"

// プレーヤーの弾クラス
class PlayerBullet : public Actor {
public:
	PlayerBullet(IWorld* world, const GSvector3& position, const GSquaternion& rotation, 
		const GSvector3& velocity, float timer, const GSuint& mesh, int damage = 1);
	virtual void update(float delta_time) override;
	virtual void draw() const override;
	virtual void react(Actor& other) override;

private:
	float lifespan_timer_;
	GSuint mesh_;
};
#endif

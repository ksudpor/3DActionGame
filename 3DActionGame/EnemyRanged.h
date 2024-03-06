#ifndef ENEMY_RANGED_H_
#define ENEMY_RANGED_H_

#include "Actor.h"
#include "AnimatedMesh.h"

class EnemyRanged : public Actor {
public:
	// 状態を表す列挙型
	enum class State {
		Idle,
		Walk,
		Turn,
		Damage,
		Attack,
		Down
	};

public:
	EnemyRanged(IWorld* world, const GSvector3& position);
	virtual void update(float delta_time) override;
	virtual void draw() const override;
	virtual void react(Actor& other) override;
private:

	void update_state(float delta_time);
	void change_state(State state, GSuint motion, bool loop = true);
	void idle(float delta_time);
	void walk(float delta_time);
	void turn(float delta_time);
	void attack(float delta_time);
	void damage(float delta_time);
	void down(float delta_time);

	bool is_turn() const;
	bool is_attack() const;
	bool is_walk() const;

	float target_signed_angle() const;
	float target_angle() const;
	float target_distance() const;

	void collide_field();
	void collide_actor(Actor& other);
	void generate_bullet();
private:
	AnimatedMesh mesh_;		// アニメーションメッシュ
	GSuint upper_motion_;			// モーション番号
	bool upper_motion_loop_;		// モーションのループ指定
	State state_;			// 状態
	float state_timer_;		// 状態タイマ
	Actor* player_;			// プレーヤー
	int health_;			// 体力

	bool is_hit_;
	float hit_timer_;

	bool attacked_;
	float attack_timer_;
};

#endif

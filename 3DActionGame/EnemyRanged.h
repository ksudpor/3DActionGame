#ifndef ENEMY_RANGED_H_
#define ENEMY_RANGED_H_

#include "Actor.h"
#include "AnimatedMesh.h"

class EnemyRanged : public Actor {
public:
	// ��Ԃ�\���񋓌^
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
	AnimatedMesh mesh_;		// �A�j���[�V�������b�V��
	GSuint upper_motion_;			// ���[�V�����ԍ�
	bool upper_motion_loop_;		// ���[�V�����̃��[�v�w��
	State state_;			// ���
	float state_timer_;		// ��ԃ^�C�}
	Actor* player_;			// �v���[���[
	int health_;			// �̗�

	bool is_hit_;
	float hit_timer_;

	bool attacked_;
	float attack_timer_;
};

#endif

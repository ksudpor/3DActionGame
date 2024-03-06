#ifndef PLAYER_H_
#define PLAYER_H_

#include "Actor.h"
#include "AnimatedMesh.h"
#include "Weapon.h"
#include <vector>
#include "Line.h"
#include "GameProgress.h"

// �v���[���[�N���X
class Player : public Actor {
public:
	//���
	enum class State {
		Equip,
		Walk,
		Run,
		Jump,
		Evade,
		Damage,
		Fire,
		Attack1,
		Attack2,
		Attack3,
		Guard,
		Skill,
		Down
	};

public:
	Player(IWorld* world = nullptr, const GSvector3& position = GSvector3{ 0.0f, 0.0f, 0.0f });
	void update(float delta_time) override;
	void draw() const override;
	void draw_gui() const override;
	virtual void react(Actor& other) override;

private:
	// ��Ԃ̍X�V
	void update_state(float delta_time);
	// ��Ԃ̕ύX
	void change_state(State state, GSuint motion, bool loop = true);
	void change_state(State state, GSuint upper_motion, bool upper_loop, GSuint lower_motion, bool lower_loop);
	void change_weapon();
	Weapon* initialize_weapon(GameProgress::Weapon enum_weapon, IWorld* world);

	void equip(float delta_time);
	void walk(float delta_time);
	void run(float delta_time);
	void jump(float delta_time);
	void evade(float delta_time);
	void damage(float delta_time);
	void fire(float delta_time);
	void attack1(float delta_time);
	void attack2(float delta_time);
	void attack3(float delta_time);
	void guard(float delta_time);
	void skill(float delta_time);
	void down(float delta_time);

	void move(float delta_time, float speed, State state, GSint motion_default, GSint motion_move, 
		bool layer_motion, bool enable_target, bool can_evade, bool can_attack = false);
	void input_handle(float delta_time);
	void attack_handle(float delta_time);
	void event_handle(Weapon* weapon);

	// ����̕`��
	void draw_weapon() const;

	// �t�B�[���h�Ƃ̏Փˏ���
	void collide_field();
	// �A�N�^�[�Ƃ̏Փˏ���
	void collide_actor(Actor& other);

	void target_enemy(float delta_time);
	float target_signed_angle() const;

private:
	AnimatedMesh mesh_;			// �A�j���[�V�������b�V��
	GSuint upper_motion_;		// ���[�V�����ԍ�
	bool upper_motion_loop_;	// ���[�V�����̃��[�v�w��
	GSuint lower_motion_;		
	bool lower_motion_loop_;
	State state_;				// ���
	float state_timer_;			// ��ԃ^�C�}
	Weapon* weapon_;
	Weapon* weapon_primary_;
	Weapon* weapon_secondary_;
	bool is_evading_;
	

	bool is_targeting_;				//���b�N�I�����Ă��邩
	std::vector<Actor*> enemies_;	//�G�̃��X�g
	Line target_line_;				//�v���C���[�ƃ��b�N�Ώۂ̊Ԃ̐�
	Actor* targeted_enemy_;			//���݃��b�N�I������Ă���G
	float untarget_timer_;			//���b�N�����̂��߂ɃL�[�𒷉������鎞��
	//�ǂ����̓G�����b�N�I������Ă��邩�Ƃ킩��悤�ɁA�G�t�F�N�g������
	GSint target_effect_;			

	GSint hit_effect_;
	float combo_timer_;
	int combo_count_;

	int health_;
	float stamina_;
	bool sp_used_;
	float sp_regen_timer_;

	// Bone Debug
	//int bone = 16;
};

#endif

#ifndef WEAPON_H_
#define WEAPON_H_

#include "PlayerMotionID.h"
#include "GameProgress.h"
#include "Assets.h"
#include "IWorld.h"
#include <gslib.h>

class Weapon {
public:
	Weapon() = default;
	virtual ~Weapon() = default;
	virtual void start();
	virtual void update(float delta_time);
	virtual void guard();
	virtual void skill();
	virtual void skill_event1();
	virtual void skill_event2();
	virtual void skill_event3();
	virtual void skill_event4();
	virtual void event_combo_attack1();
	virtual void event_combo_attack2();
	virtual void event_combo_attack3();
	virtual void event_fire();

	const GSint& motion_walk_front() const;
	const GSint& motion_walk_back() const;
	const GSint& motion_run() const;
	const GSint& motion_jump() const;
	const GSint& motion_damage() const;
	const GSint& motion_die() const;
	const GSint& motion_equip() const;
	const GSint& motion_idle() const;
	const GSint& motion_guard() const;
	const GSint& motion_attack1() const;
	const GSint& motion_attack2() const;
	const GSint& motion_attack3() const;
	const GSint& motion_fire() const;
	const GSint& motion_move_fire() const;
	const GSint& motion_skill() const;
	const GSint& bone_position_right_hand() const;
	const GSint& bone_position_left_hand() const;
	const GSuint& mesh_number_right_hand() const;
	const GSuint& mesh_number_left_hand() const;
	const GSfloat& event_time_attack1() const;
	const GSfloat& event_time_attack2() const;
	const GSfloat& event_time_attack3() const;
	const GSfloat& event_time_fire() const;
	const float& attack_motion_rate() const;
	const float& damage_motion_rate() const;
	const float& guard_sp() const;
	const float& evade_sp() const;
	const int& defend() const;
	const int& damage() const;
	const bool& has_combo() const;

protected:
	GSint motion_walk_front_;
	GSint motion_walk_back_;
	GSint motion_run_;
	GSint motion_jump_;
	GSint motion_damage_;
	GSint motion_die_;
	GSint motion_equip_;
	GSint motion_idle_;
	GSint motion_guard_;
	GSint motion_attack1_;
	GSint motion_attack2_;
	GSint motion_attack3_;
	GSint motion_fire_;
	GSint motion_move_fire_;
	GSint motion_skill_;
	GSint bone_position_right_hand_;
	GSint bone_position_left_hand_;
	GSuint mesh_number_right_hand_;
	GSuint mesh_number_left_hand_;
	GSfloat event_time_attack1_{ 0.0f };
	GSfloat event_time_attack2_{ 0.0f };
	GSfloat event_time_attack3_{ 0.0f };
	GSfloat event_time_fire_{ 0.0f };
	IWorld* world_{ nullptr };
	float attack_motion_rate_{ 1.0f };
	float damage_motion_rate_{ 1.0f };
	bool has_combo_;

	float guard_sp_;
	float evade_sp_;
	int defend_;
	int damage_;
};


#endif

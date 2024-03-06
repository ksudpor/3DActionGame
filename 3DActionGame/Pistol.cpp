#include "Pistol.h"
#include "PlayerBullet.h"
#include <GSEffect.h>

const GSvector3 fire_position_{ 0.0f, 1.2f, 0.5f };

Pistol::Pistol(IWorld* world){
	world_ = world;
	motion_walk_front_ = MotionGunWalkF;
	motion_walk_back_ = MotionDefaultWalkB;
	motion_run_ = MotionGunRunF;
	motion_jump_ = MotionGunJump;
	motion_damage_ = MotionGunDamage;
	motion_die_ = MotionGunDie;
	motion_equip_ = MotionGunEquip;
	motion_idle_ = MotionGunIdle;
	motion_guard_ = MotionGunGuard;
	motion_fire_ = MotionPistolFire;
	motion_move_fire_ = MotionDefaultWalkF;
	motion_skill_ = MotionPistolSkill;
	bone_position_right_hand_ = 90;
	mesh_number_right_hand_ = Mesh_WeaponPistol;
	bone_position_left_hand_ = 30;
	mesh_number_left_hand_ = Mesh_None;
	event_time_fire_ = 5.0f;
	has_combo_ = false;
	guard_sp_ = 30.0f;
	evade_sp_ = 30.0f;
	defend_ = 25;
}

void Pistol::event_fire() {
	Actor* player = world_->find_actor("Player");
	player->play_effect(Effect_MuzzleFlash1, fire_position_, GSvector3(0.0f, 0.0f, 0.0f), GSvector3(0.5f, 0.5f, 0.5f));
	generate_bullet(fire_position_);
	gsPlaySE(SE_Pistol);
}

void Pistol::generate_bullet(const GSvector3& gen_pos){
	// �e�𐶐�����ꏊ�̋���
	const float GenerateDistance{ gen_pos.z };
	// ��������ʒu�̍����̕␳�l
	const float GenerateHeight{ gen_pos.y };
	// �e�̈ړ��X�s�[�h
	const float Speed{ 0.8f };
	int damage = gsRand(20, 30) * GameProgress::player_attack_rate_;
	//int damage = 1000;
	// �v���C���[��T��
	Actor* player = world_->find_actor("Player");
	// �����ʒu�̌v�Z
	GSvector3 position = player->transform().position() + player->transform().forward() * GenerateDistance;
	// �����ʒu�̍�����␳����
	position.y += GenerateHeight;
	GSquaternion rotation = player->transform().rotation();
	// �ړ��ʂ̌v�Z
	GSvector3 velocity = player->transform().forward() * Speed;
	// �e�̐���
	world_->add_actor(new PlayerBullet{ world_, position, rotation, velocity, 40.0f, Mesh_BulletPistol, damage });
}

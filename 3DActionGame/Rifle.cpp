#include "Rifle.h"
#include "PlayerBullet.h"
#include <GSEffect.h>

const GSvector3 fire_position_{ 0.0f, 1.25f, 0.4f };

Rifle::Rifle(IWorld* world){
	world_ = world;
	motion_walk_front_ = MotionGunWalkF;
	motion_walk_back_ = MotionAxeWalkB;
	motion_run_ = MotionGunRunF;
	motion_jump_ = MotionGunJump;
	motion_damage_ = MotionGunDamage;
	motion_die_ = MotionGunDie;
	motion_equip_ =MotionGunEquip;
	motion_idle_ = MotionGunIdle;
	motion_guard_ = MotionGunGuard;
	motion_fire_ = MotionRifleFire;
	motion_move_fire_ = MotionGreatSwordWalkF;
	motion_skill_ = MotionRifleSkill;
	bone_position_right_hand_ = 94;
	mesh_number_right_hand_ = Mesh_WeaponRifle;
	bone_position_left_hand_ = 30;
	mesh_number_left_hand_ = Mesh_None;
	event_time_fire_ = 5.0f;
	has_combo_ = false;
	guard_sp_ = 35.0f;
	evade_sp_ = 30.0f;
	defend_ = 25;
}

void Rifle::event_fire(){
	Actor* player = world_->find_actor("Player");
	player->play_effect(Effect_MuzzleFlash2, fire_position_, GSvector3(0.0f, 0.0f, 0.0f), GSvector3(1.0f, 1.0f, 1.0f));
	generate_bullet(fire_position_);
	gsPlaySE(SE_Rifle);
}

void Rifle::generate_bullet(const GSvector3& gen_pos) {
	// ’e‚ð¶¬‚·‚éêŠ‚Ì‹——£
	const float GenerateDistance{ gen_pos.z };
	// wwww¶¬‚·‚éˆÊ’u‚Ì‚‚³‚Ì•â³’l
	const float GenerateHeight{ gen_pos.y };
	// ’e‚ÌˆÚ“®ƒXƒs[ƒh
	const float Speed{ 1.0f };
	int damage = gsRand(15, 25) * GameProgress::player_attack_rate_;
	// ƒvƒŒƒCƒ„[‚ð’T‚·
	Actor* player = world_->find_actor("Player");
	// ¶¬ˆÊ’u‚ÌŒvŽZ
	GSvector3 position1 = player->transform().position() + player->transform().forward() * GenerateDistance;
	GSvector3 position2 = player->transform().position() + player->transform().forward() * (GenerateDistance + 0.6f);
	GSvector3 position3 = player->transform().position() + player->transform().forward() * (GenerateDistance + 1.2f);
	// ¶¬ˆÊ’u‚Ì‚‚³‚ð•â³‚·‚é
	position1.y += GenerateHeight;
	position2.y += GenerateHeight;
	position3.y += GenerateHeight;
	GSquaternion rotation = player->transform().rotation();
	// ˆÚ“®—Ê‚ÌŒvŽZ
	GSvector3 velocity = player->transform().forward() * Speed;
	// ’e‚Ì¶¬
	world_->add_actor(new PlayerBullet{ world_, position1, rotation, velocity, 60.0f, Mesh_BulletRifle, damage });
	world_->add_actor(new PlayerBullet{ world_, position2, rotation, velocity, 60.0f, Mesh_BulletRifle, damage });
	world_->add_actor(new PlayerBullet{ world_, position3, rotation, velocity, 60.0f, Mesh_BulletRifle, damage });
}
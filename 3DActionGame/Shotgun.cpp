#include "Shotgun.h"
#include "PlayerBullet.h"
#include <GSEffect.h>

const GSvector3 fire_position_ { 0.0f, 1.2f, 0.9f };

Shotgun::Shotgun(IWorld* world){
	world_ = world;
	motion_walk_front_ = MotionGunWalkF;
	motion_walk_back_ = MotionAxeWalkB;
	motion_run_ = MotionGunRunF;
	motion_jump_ = MotionGunJump;
	motion_damage_ = MotionGunDamage;
	motion_die_ = MotionGunDie;
	motion_equip_ = MotionGunEquip;
	motion_idle_ = MotionGunIdle;
	motion_guard_ = MotionGunGuard;
	motion_fire_ = MotionShotgunFire;
	motion_move_fire_ = MotionAxeWalkF;
	motion_skill_ = MotionShotgunSkill;
	bone_position_right_hand_ = 90;
	mesh_number_right_hand_ = Mesh_None;
	bone_position_left_hand_ = 30;
	mesh_number_left_hand_ = Mesh_WeaponShotgun;
	event_time_fire_ = 5.0f;
	has_combo_ = false;
	guard_sp_ = 25.0f;
	evade_sp_ = 30.0f;
	defend_ = 25;
}

void Shotgun::event_fire(){
	Actor* player = world_->find_actor("Player");
	player->play_effect(Effect_MuzzleFlash1, fire_position_, GSvector3(0.0f, 0.0f, 0.0f), GSvector3(1.0f, 1.0f, 1.0f));
	generate_bullet(fire_position_);
	gsPlaySE(SE_Shotgun);
}

void Shotgun::generate_bullet(const GSvector3& gen_pos){
	// ’e‚ð¶¬‚·‚éêŠ‚Ì‹——£
	const float GenerateDistance{ gen_pos.z };
	// ¶¬‚·‚éˆÊ’u‚Ì‚‚³‚Ì•â³’l
	const float GenerateHeight{ gen_pos.y };
	// ’e‚ÌˆÚ“®ƒXƒs[ƒh
	const float Speed{ 0.8f };
	int damage = gsRand(25, 40) * GameProgress::player_attack_rate_;
	// ƒvƒŒƒCƒ„[‚ð’T‚·
	Actor* player = world_->find_actor("Player");
	// ¶¬ˆÊ’u‚ÌŒvŽZ
	GSvector3 position1 = player->transform().position() + player->transform().forward() * GenerateDistance;
	GSvector3 position2 = player->transform().position() + 1.0f * player->transform().right() * GenerateDistance;
	GSvector3 position3 = player->transform().position() - 1.0f * player->transform().right() * GenerateDistance;
	// ¶¬ˆÊ’u‚Ì‚‚³‚ð•â³‚·‚é
	position1.y += GenerateHeight;
	position2.y += GenerateHeight;
	position3.y += GenerateHeight;
	GSquaternion rotation = player->transform().rotation();
	// ˆÚ“®—Ê‚ÌŒvŽZ
	GSvector3 velocity = player->transform().forward() * Speed;
	// ’e‚Ì¶¬ {
	world_->add_actor(new PlayerBullet{ world_, position1, rotation, velocity, 20.0f, Mesh_BulletShotgun, damage });
	world_->add_actor(new PlayerBullet{ world_, position2, rotation, velocity, 20.0f, Mesh_BulletShotgun, damage });
	world_->add_actor(new PlayerBullet{ world_, position3, rotation, velocity, 20.0f, Mesh_BulletShotgun, damage });
}

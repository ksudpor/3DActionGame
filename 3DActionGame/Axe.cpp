#include "Axe.h"
#include "AttackCollider.h"
#include <GSEffect.h>

Axe::Axe(IWorld* world){
	world_ = world;
	motion_walk_front_ = MotionAxeWalkF;
	motion_walk_back_ = MotionAxeWalkB;
	motion_run_ = MotionAxeRunF;
	motion_jump_ = MotionAxeJump;
	motion_damage_ = MotionAxeDamage;
	motion_die_ = MotionAxeDie;
	motion_equip_ = MotionAxeEquip;
	motion_idle_ = MotionAxeIdle;
	motion_guard_ = MotionAxeGuard;
	motion_attack1_ = MotionAxeAttack1;
	motion_attack2_ = MotionAxeAttack2;
	motion_attack3_ = MotionAxeAttack3;
	motion_move_fire_ = MotionAxeWalkF;
	motion_skill_ = MotionAxeSkill;
	bone_position_right_hand_ = 94;
	mesh_number_right_hand_ = Mesh_WeaponAxe;
	bone_position_left_hand_ = 30;
	mesh_number_left_hand_ = Mesh_None;
	event_time_attack1_ = 48.0f;
	event_time_attack2_ = 48.0f;
	event_time_attack3_ = 60.0f;
	attack_motion_rate_ = 0.75f;
	damage_motion_rate_ = 0.5f;
	has_combo_ = true;
	guard_sp_ = 20.0f;
	evade_sp_ = 20.0f;
	defend_ = 28;
}

void Axe::event_combo_attack1(){
	Actor* player = world_->find_actor("Player");
	player->play_effect(Effect_SlashRed, GSvector3(0.0f, 0.9f, 0.0f), GSvector3(0.0f, 0.0f, -180.0f), GSvector3(0.8f, 0.8f, 0.8f));
	generate_attack_collider(0.6f, 0.6f, 0.4f);
	generate_attack_collider(1.0f, 0.0f, 0.4f);
	generate_attack_collider(0.6f, -0.6f, 0.4f);
	gsPlaySE(SE_Axe1);
}

void Axe::event_combo_attack2(){
	Actor* player = world_->find_actor("Player");
	player->play_effect(Effect_SlashRed, GSvector3(0.2f, 0.3f, 0.2f), GSvector3(-30.0f, 0.0f, -210.0f), GSvector3(0.8f, 0.8f, 0.8f));
	generate_attack_collider(1.0f, 0.0f, 0.4f);
	generate_attack_collider(0.6f, 0.6f, 0.4f);
	generate_attack_collider(0.6f, -0.6f, 0.4f);
	gsPlaySE(SE_Axe2);
}

void Axe::event_combo_attack3(){
	Actor* player = world_->find_actor("Player");
	player->play_effect(Effect_SlashRed, GSvector3(0.0f, 1.0f, 0.6f), GSvector3(0.0f, 0.0f, -90.0f), GSvector3(0.8f, 0.8f, 0.8f));
	player->play_effect(Effect_SlashRed, GSvector3(0.6f, 1.0f, 0.6f), GSvector3(0.0f, 0.0f, -90.0f), GSvector3(0.8f, 0.8f, 0.8f));
	player->play_effect(Effect_SlashRed, GSvector3(-0.6f, 1.0f, 0.6f), GSvector3(0.0f, 0.0f, -90.0f), GSvector3(0.8f, 0.8f, 0.8f));
	generate_attack_collider(0.8f, 0.0f, 0.4f);
	generate_attack_collider(0.4f, 0.6f, 0.4f);
	generate_attack_collider(0.4f, -0.6f, 0.4f);
	generate_attack_collider(1.6f, 0.0f, 0.4f);
	generate_attack_collider(1.2f, 0.6f, 0.4f);
	generate_attack_collider(1.2f, -0.6f, 0.4f);
	gsPlaySE(SE_Axe3);
}

void Axe::generate_attack_collider(float d_forward, float d_side, float radius) {
	const float Height{ 1.0f };
	const float Lifespan{ 15.0f };
	const float Delay{ 0.0f };
	int damage = gsRand(96, 128) * GameProgress::player_attack_rate_;
	// プレイヤーを探す
	Actor* player = world_->find_actor("Player");
	// 衝突判定を出現させる座標を求める（前方の位置）
	GSvector3 position = player->transform().position() + player->transform().forward() * d_forward + player->transform().right() * d_side;
	// 高さの補正（足元からの高さ）
	position.y += Height;
	// 衝突判定用の球を作成
	BoundingSphere collider{ radius, position };
	// 衝突判定を出現させる
	world_->add_actor(new AttackCollider{ world_, collider,
	"PlayerAttackTag", "PlayerAttack", player->tag(), damage, Lifespan, Delay });
}

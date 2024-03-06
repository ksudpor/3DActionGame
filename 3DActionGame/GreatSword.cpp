#include "GreatSword.h"
#include "AttackCollider.h"
#include <GSEffect.h>

GreatSword::GreatSword(IWorld* world){
	world_ = world;
	motion_walk_front_ = MotionGreatSwordWalkF;
	motion_walk_back_ = MotionGreatSwordWalkB;
	motion_run_ = MotionGreatSwordRunF;
	motion_jump_ = MotionGreatSwordJump;
	motion_damage_ = MotionGreatSwordDamage;
	motion_die_ = MotionGreatSwordDie;
	motion_equip_ = MotionGreatSwordEquip;
	motion_idle_ = MotionGreatSwordIdle;
	motion_guard_ = MotionGreatSwordGuard;
	motion_attack1_ = MotionGreatSwordAttack1;
	motion_attack2_ = MotionGreatSwordAttack2;
	motion_attack3_ = MotionGreatSwordAttack3;
	motion_move_fire_ = MotionGreatSwordWalkF;
	motion_skill_ = MotionGreatSwordSkill;
	bone_position_right_hand_ = 90;
	mesh_number_right_hand_ = Mesh_None;
	bone_position_left_hand_ = 16;
	mesh_number_left_hand_ = Mesh_WeaponGreatsword;
	event_time_attack1_ = 30.0f;
	event_time_attack2_ = 45.0f;
	event_time_attack3_ = 45.0f;
	damage_motion_rate_ = 0.625f;
	has_combo_ = true;
	guard_sp_ = 25.0f;
	evade_sp_ = 20.0f;
	defend_ = 32;
}

void GreatSword::event_combo_attack1(){
	Actor* player = world_->find_actor("Player");
	player->play_effect(Effect_SlashDark, GSvector3(0.0f, 1.1f, 0.5f), GSvector3(0.0f, 0.0f, -180.0f), GSvector3(1.0f, 1.0f, 1.0f));
	generate_attack_collider(1.55f, 0.0f, 0.45f);
	generate_attack_collider(1.05f, 0.75f, 0.45f);
	generate_attack_collider(1.05f, -0.75f, 0.45f);
	gsPlaySE(SE_Greatsword1);
}

void GreatSword::event_combo_attack2(){
	Actor* player = world_->find_actor("Player");
	player->play_effect(Effect_SlashDark, GSvector3(0.0f, 0.9f, 0.5f), GSvector3(0.0f, 0.0f, 0.0f), GSvector3(1.0f, 1.0f, 1.0f));
	generate_attack_collider(1.55f, 0.0f, 0.45f);
	generate_attack_collider(1.05f, 0.75f, 0.45f);
	generate_attack_collider(1.05f, -0.75f, 0.45f);
	gsPlaySE(SE_Greatsword2);
}

void GreatSword::event_combo_attack3(){
	Actor* player = world_->find_actor("Player");
	player->play_effect(Effect_SlashDark, GSvector3(0.0f, 1.0f, 0.6f), GSvector3(0.0f, 0.0f, -30.0f), GSvector3(1.0f, 1.0f, 1.0f));
	player->play_effect(Effect_SlashDark, GSvector3(0.0f, 1.0f, 0.6f), GSvector3(0.0f, 0.0f, -150.0f), GSvector3(1.0f, 1.0f, 1.0f));
	generate_attack_collider(1.8f, 0.0f, 0.45f);
	generate_attack_collider(1.4f, 0.9f, 0.45f);
	generate_attack_collider(1.4f, -0.9f, 0.45f);
	generate_attack_collider(0.9f, 0.0f, 0.45f);
	generate_attack_collider(0.5f, 0.9f, 0.45f);
	generate_attack_collider(0.5f, -0.9f, 0.45f);
	gsPlaySE(SE_Greatsword3);
}

void GreatSword::generate_attack_collider(float d_forward, float d_side, float radius){
	const float Height{ 1.0f };
	const float Lifespan{ 15.0f };
	const float Delay{ 0.0f };
	int damage = gsRand(112, 144) * GameProgress::player_attack_rate_;
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
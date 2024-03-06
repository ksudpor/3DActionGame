#include "SwordShield.h"
#include "AttackCollider.h"
#include <GSEffect.h>

SwordShield::SwordShield(IWorld* world){
	world_ = world;
	motion_walk_front_ = MotionSwordShieldWalkF;
	motion_walk_back_ = MotionSwordShieldWalkB;
	motion_run_ = MotionSwordShieldRunF;
	motion_jump_ = MotionSwordShieldJump;
	motion_damage_ = MotionSwordShieldDamage;
	motion_die_ = MotionSwordShieldDie;
	motion_equip_ = MotionSwordShieldEquip;
	motion_idle_ = MotionSwordShieldIdle;
	motion_guard_ = MotionSwordShieldGuard;
	motion_attack1_ = MotionSwordShieldAttack1;
	motion_attack2_ = MotionSwordShieldAttack2;
	motion_attack3_ = MotionSwordShieldAttack3;
	motion_move_fire_ = MotionSwordShieldWalkF;
	motion_skill_ = MotionSwordShieldSkill;
	bone_position_right_hand_ = 94;
	mesh_number_right_hand_ = Mesh_WeaponSword;
	bone_position_left_hand_ = 30;
	mesh_number_left_hand_ = Mesh_WeaponShield;
	event_time_attack1_ = 30.0f;
	event_time_attack2_ = 45.0f;
	event_time_attack3_ = 75.0f;
	has_combo_ = true;
	guard_sp_ = 15.0f;
	evade_sp_ = 20.0f;
	defend_ = 36;
}

void SwordShield::event_combo_attack1(){
	Actor* player = world_->find_actor("Player");
	player->play_effect(Effect_SlashBlue, GSvector3(-0.1f, 0.8f, 0.3f), GSvector3(0.0f, 0.0f, -210.0f), GSvector3(0.6f, 0.6f, 0.6f));
	generate_attack_collider(0.95f, 0.35f, 0.35f);
	generate_attack_collider(0.95f, -0.35f, 0.35f);
	gsPlaySE(SE_Sword1);
}

void SwordShield::event_combo_attack2(){
	Actor* player = world_->find_actor("Player");
	player->play_effect(Effect_SlashBlue, GSvector3(0.1f, 1.2f, 0.3f), GSvector3(0.0f, 0.0f, -30.0f), GSvector3(0.6f, 0.6f, 0.6f));
	generate_attack_collider(0.95f, 0.35f, 0.35f);
	generate_attack_collider(0.95f, -0.35f, 0.35f);
	gsPlaySE(SE_Sword2);
}

void SwordShield::event_combo_attack3(){
	Actor* player = world_->find_actor("Player");
	player->play_effect(Effect_SlashBlue, GSvector3(0.0f, 1.0f, 0.4f), GSvector3(0.0f, 0.0f, 0.0f), GSvector3(0.6f, 0.6f, 0.6f));
	player->play_effect(Effect_SlashBlue, GSvector3(0.0f, 1.0f, -0.4f), GSvector3(0.0f, 180.0f, 0.0f), GSvector3(0.6f, 0.6f, 0.6f));
	player->play_effect(Effect_SlashBlue, GSvector3(0.4f, 1.0f, 0.0f), GSvector3(0.0f, 90.0f, 0.0f), GSvector3(0.6f, 0.6f, 0.6f));
	player->play_effect(Effect_SlashBlue, GSvector3(-0.4f, 1.0f, 0.0f), GSvector3(0.0f, -90.0f, 0.0f), GSvector3(0.6f, 0.6f, 0.6f));
	generate_attack_collider(1.05f, 0.0f, 0.35f);
	generate_attack_collider(-1.05f, 0.0f, 0.35f);
	generate_attack_collider(0.0f, 1.05f, 0.35f);
	generate_attack_collider(0.0f, -1.05f, 0.35f);
	generate_attack_collider(0.55f, 0.55f, 0.35f);
	generate_attack_collider(-0.55f, -0.55f, 0.35f);
	generate_attack_collider(-0.55f, 0.55f, 0.35f);
	generate_attack_collider(0.55f, -0.55f, 0.35f);
	gsPlaySE(SE_Sword3);
}

void SwordShield::generate_attack_collider(float d_forward, float d_side, float radius) {
	const float Height{ 1.0f };
	const float Lifespan{ 15.0f };
	const float Delay{ 0.0f };
	int damage = gsRand(80, 112) * GameProgress::player_attack_rate_;
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

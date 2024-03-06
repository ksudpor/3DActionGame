#include "Player.h"
#include "IWorld.h"
#include "Field.h"
#include "Assets.h"
#include "PlayerMotionID.h"
#include "Screen.h"


#include "Rifle.h"
#include "Pistol.h"
#include "Shotgun.h"
#include "Axe.h"
#include "SwordShield.h"
#include "Greatsword.h"

#include <gslib.h>
#include <GSEffect.h>
#include <imgui/imgui.h>

const float WalkSpeed{ 0.03f };		// �������̈ړ����x
const float RunSpeed{ 0.075f };		// ���鎞�̈ړ����x
const float PlayerHeight{ 1.0f };	// �����̍���
const float PlayerRadius{ 0.45f };	// �Փ˔���p�̔��a
const float FootOffset{ 0.1f };		// �����̃I�t�Z�b�g
const float Gravity{ -0.008f };		// �d�͒l
const float JumpPower{ 0.12 };		// ���˂��
const int BaseDefend{ 20 };
int* MaxHP{ &GameProgress::player_health_ };
float* MaxSP{ &GameProgress::player_stamina_ };

// ImGui Effect Debug
//static GSvector3 pos;
//static GSvector3 rot;
//static GSvector3 scale{ 1.0f, 1.0f, 1.0f };

Player::Player(IWorld* world, const GSvector3& position) :
	mesh_{ Mesh_Player, Skel_Player, Anim_Player, MotionDefaultWait00, true, 2},
	upper_motion_{ MotionDefaultWait00 },
	upper_motion_loop_{ true },
	lower_motion_{ MotionDefaultWait00 },
	lower_motion_loop_{ true },
	state_{ State::Walk },
	state_timer_{ 0.0f },
	health_{ *MaxHP },
	stamina_{ *MaxSP } {
	// ���[���h��ݒ�
	world_ = world;
	// �^�O���̐ݒ�
	tag_ = "PlayerTag";
	// ���O�̐ݒ�
	name_ = "Player";
	// �Փ˔��苅�̐ݒ�
	collider_ = BoundingSphere{ PlayerRadius, GSvector3{0.0f, PlayerHeight, 0.0f} };
	// ���W�̏�����
	transform_.position(position);
	// ���b�V���̕ϊ��s���������
	mesh_.transform(transform_.localToWorldMatrix());
	is_evading_ = false;
	is_targeting_ = false;
	weapon_primary_ = initialize_weapon(GameProgress::primary_weapon, world);
	weapon_secondary_ = initialize_weapon(GameProgress::secondary_weapon, world);
	weapon_ = weapon_primary_;
	combo_timer_ = 0;
	combo_count_ = 0;
	targeted_enemy_ = nullptr;
	untarget_timer_ = 30.0f;
	event_handle(weapon_primary_);
	event_handle(weapon_secondary_);
	sp_used_ = false;
	sp_regen_timer_ = 0.0f;

	// �����g�̃{�[���h�c
	static const GSuint lower_body_bone_id[] =
	{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	// �����g�{�[�������C���[�P�ɐݒ�i����ȊO�̏㔼�g�{�[���̓��C���[�O�j
	mesh_.set_layer_indices(1, lower_body_bone_id, 11);
}

void Player::update(float delta_time){
	update_state(delta_time);
	velocity_.y += Gravity * delta_time;
	transform_.translate(0.0f, velocity_.y, 0.0f);
	collide_field();

	mesh_.change_motion(0, upper_motion_, upper_motion_loop_);
	mesh_.change_motion(1, lower_motion_, lower_motion_loop_);
	mesh_.update(delta_time);
	mesh_.transform(transform_.localToWorldMatrix());

	weapon_->update(delta_time);
	if (!is_evading_) combo_timer_--;
	if (combo_timer_ < 0) combo_count_ = 0;

	if (sp_used_) sp_regen_timer_ += delta_time;
	if (sp_regen_timer_ > 60.0f) sp_used_ = false;
	if (!sp_used_) stamina_ += *MaxSP * delta_time / 400.0f;

	health_ = CLAMP(health_, 0, *MaxHP);
	stamina_ = CLAMP(stamina_, 0, *MaxSP);

	target_enemy(delta_time);

	// bone Debug
	//if (gsGetKeyTrigger(GKEY_UP)) ++bone;
	//if (gsGetKeyTrigger(GKEY_DOWN)) --bone;

	// ImGui Effect Debug
	//ImGui::Begin("Effect Adjust");
	//ImGui::DragFloat3("Position", pos);
	//ImGui::DragFloat3("Rotation", rot);
	//ImGui::DragFloat3("Scale", scale);
	//ImGui::End();
}

void Player::draw() const{
	GScolor color{ 0.8f, 0.2f, 0.2f, 1.0f };
	gsSetEffectColor(hit_effect_, &color);
	
	mesh_.draw();
	draw_weapon();

	// �Փ˔��苅�̃f�o�b�O�\��
	//collider().draw();
}

void Player::draw_gui() const{
	GSvector2 hp_frame_pos{ Screen::Width * 0.03f, Screen::Height * 0.04f };
	GSvector2 hp_bar_pos{ Screen::Width * 0.055f, Screen::Height * 0.0844f };
	GSvector2 sp_bar_pos{ Screen::Width * 0.03f, Screen::Height * 0.13f };
	GSvector2 hp_bar_scale = Screen::Scale * GSvector2{ (float)health_ / (float)*MaxHP, 1.0f };
	GSvector2 sp_bar_scale = Screen::Scale * GSvector2{ stamina_ / *MaxSP, 1.0f };
	GScolor transparent = GScolor{ 1.0f, 1.0f, 1.0f, 0.5f };
	gsDrawSprite2D(Image_HpBarGrey, &hp_bar_pos, NULL, NULL, &transparent, &Screen::Scale, 0.0f);
	gsDrawSprite2D(Image_HpBarRed, &hp_bar_pos, NULL, NULL, NULL, &hp_bar_scale, 0.0f);
	gsDrawSprite2D(Image_HpFrame, &hp_frame_pos, NULL, NULL, NULL, &Screen::Scale, 0.0f);
	gsDrawSprite2D(Image_SpBarGrey, &sp_bar_pos, NULL, NULL, &transparent, &Screen::Scale, 0.0f);
	gsDrawSprite2D(Image_SpBarOrange, &sp_bar_pos, NULL, NULL, NULL, &sp_bar_scale, 0.0f);

	//position debug
	//gsFontParameter(0, Screen::Height * 0.025, "�l�r �S�V�b�N");
	//gsTextPos(Screen::Width * 0.08f, Screen::Height * 0.5f);
	//gsDrawText("%f, %f, %f", transform_.position().x, transform_.position().y, transform_.position().z);
}

void Player::react(Actor& other){
	// �_���[�W���̏ꍇ�͉������Ȃ�
	if (state_ == State::Damage || state_ == State::Down) return;
	int damage = other.damage() - (BaseDefend + (float)weapon_->defend() / (float)other.damage());
	int guard_damage = other.damage() - (BaseDefend + (weapon_->defend() * 2));
	damage = CLAMP(damage, 0, damage);
	guard_damage = CLAMP(guard_damage, 0, guard_damage);

	// �G�̍U������ƏՓ˂������H
	if ((other.tag() == "EnemyAttackTag" || other.tag() == "EnemyBulletTag") && !is_evading_) {
		if (state_ == State::Guard && stamina_ >= weapon_->guard_sp()) {
			health_ -= guard_damage;
			stamina_ -= weapon_->guard_sp();
			other.play_effect(Effect_Guard, GSvector3{ 0.0f,0.0f,0.0f}, GSvector3{ 0.0f,180.0f,0.0f }, GSvector3{ 1.0f,1.0f,1.0f });
			gsPlaySE(SE_Guard);
			return;
		}

		// �̗͂����炷
		health_ -= damage;
		sp_used_ = true;
		sp_regen_timer_ = 0;

		GSmatrix4 localMat = GSmatrix4::TRS(GSvector3{ 0.0f,0.8f,0.0f }, GSquaternion::euler(GSvector3(0.0f, 0.0f, 0.0f)), GSvector3{ 0.5f, 0.5f, 0.5f });
		GSmatrix4 worldMat = localMat * transform_.localToWorldMatrix();
		hit_effect_ = gsPlayEffectEx(Effect_Hit, &worldMat);

		if (health_ <= 0) {
			// �c��̗̑͂��Ȃ���΃_�E����ԂɑJ��
			gsPlaySE(SE_PlayerDie);
			change_state(State::Down, weapon_->motion_die(), false);
		}
		else {
			if (state_ == State::Attack1 || state_ == State::Attack2 
				|| state_ == State::Attack3 && gsRand(1,100) < 50) {
				return;
			}
			if (other.tag() == "EnemyAttackTag") {
				// �^�[�Q�b�g�����̃x�N�g�������߂�
				GSvector3 to_target = other.transform().position() - transform().position();
				// �������͖����ɂ���
				to_target.y = 0.0f;
				// �^�[�Q�b�g�����Ƌt�����Ƀm�b�N�o�b�N����ړ��ʂ����߂�
				velocity_ = -to_target.getNormalized() * 0.4f;
			}
			// �_���[�W��ԂɑJ�ڂ���
			gsPlaySE(SE_PlayerDamage);
			change_state(State::Damage, weapon_->motion_damage(), false);
			return;
		}
	}
	// �G�ƏՓ˂������H
	if (other.tag() == "EnemyTag") {
		// �A�N�^�[���m���d�Ȃ�Ȃ��悤�ɕ␳����
		collide_actor(other);
	}
	if (other.tag() == "HealthPotionTag") {
		if (*MaxHP - health_ <= 300){
			health_ = *MaxHP;
		}
		else {
			health_ += 300;
		}
	}
}

void Player::update_state(float delta_time){
	switch (state_) {
	case State::Equip:		equip(delta_time);		break;
	case State::Walk:		walk(delta_time);		break;
	case State::Run:		run(delta_time);		break;
	case State::Jump:		jump(delta_time);		break;
	case State::Evade:		evade(delta_time);		break;
	case State::Damage:		damage(delta_time);		break;
	case State::Fire:		fire(delta_time);		break;
	case State::Attack1:	attack1(delta_time);	break;
	case State::Attack2:	attack2(delta_time);	break;
	case State::Attack3:	attack3(delta_time);	break;
	case State::Guard:		guard(delta_time);		break;
	case State::Skill:		skill(delta_time);		break;
	case State::Down:		down(delta_time);		break;
	}
	state_timer_ += delta_time;
}

void Player::change_state(State state, GSuint motion, bool loop){
	upper_motion_ = motion;
	upper_motion_loop_ = loop;
	lower_motion_ = motion;
	lower_motion_loop_ = loop;
	state_ = state;
	state_timer_ = 0.0f;
}

void Player::change_state(State state, GSuint upper_motion, bool upper_loop, GSuint lower_motion, bool lower_loop){
	upper_motion_ = upper_motion;
	upper_motion_loop_ = upper_loop;
	lower_motion_ = lower_motion;
	lower_motion_loop_ = lower_loop;
	state_ = state;
	state_timer_ = 0.0f;
}

void Player::change_weapon(){
	if (weapon_ == weapon_primary_) weapon_ = weapon_secondary_;
	else if (weapon_ == weapon_secondary_) weapon_ = weapon_primary_;
	combo_timer_ = 0;
	combo_count_ = 0;
	
	change_state(State::Equip, weapon_->motion_equip());
}

Weapon* Player::initialize_weapon(GameProgress::Weapon enum_weapon, IWorld* world)
{
	switch (enum_weapon) {
	case GameProgress::Weapon::Axe:
		return new Axe{ world };			break;
	case GameProgress::Weapon::SwordShield:
		return new SwordShield{ world };	break;
	case GameProgress::Weapon::GreatSword:
		return new GreatSword{ world };		break;
	case GameProgress::Weapon::Pistol:
		return new Pistol{ world };			break;
	case GameProgress::Weapon::Rifle:
		return new Rifle{ world };			break;
	case GameProgress::Weapon::Shotgun:
		return new Shotgun{ world };		break;
	}
	return nullptr;
}

void Player::equip(float delta_time){
	if (state_timer_ >= mesh_.motion_end_time()) {
		walk(delta_time);
	}
}

void Player::walk(float delta_time){
	if (gsGetKeyState(GKEY_LSHIFT) && stamina_ > 0.0f) {
		change_state(State::Run, weapon_->motion_run());
		return;
	}

	if (gsGetMouseButtonState(GMOUSE_BUTTON_2)) {
		change_state(State::Guard, weapon_->motion_guard(), true, weapon_->motion_guard(), true);
		return;
	}

	move(delta_time, WalkSpeed * GameProgress::player_speed_rate_, State::Walk, weapon_->motion_idle(), weapon_->motion_walk_front(), false, true, true);
	input_handle(delta_time);
	attack_handle(delta_time);
}

void Player::run(float delta_time){
	stamina_ -= delta_time * 0.2f;
	sp_used_ = true;
	sp_regen_timer_ = 0;

	if (!gsGetKeyState(GKEY_LSHIFT) || stamina_ <= 0.0f) {
		change_state(State::Walk, weapon_->motion_walk_front());
		return;
	}

	if (gsGetMouseButtonState(GMOUSE_BUTTON_2)) {
		change_state(State::Guard, weapon_->motion_guard());
		return;
	}

	move(delta_time, RunSpeed * GameProgress::player_speed_rate_, State::Run, weapon_->motion_idle(), weapon_->motion_run(), false, false, true);
	input_handle(delta_time);
	attack_handle(delta_time);
}

void Player::jump(float delta_time){
	transform_.translate(velocity_, GStransform::Space::World);
	attack_handle(delta_time);

	GSvector3 position = transform_.position();
	Line line;
	line.start = position + collider_.center;
	line.end = position + GSvector3{ 0.0f, -FootOffset, 0.0f };
	GSvector3 intersect;
	if (state_timer_ >= mesh_.motion_end_time() || world_->field()->collide(line, &intersect)) {
		walk(delta_time);
	}
}

void Player::evade(float delta_time){
	transform_.translate(velocity_, GStransform::Space::World);
	sp_used_ = true;
	sp_regen_timer_ = 0;

	if (state_timer_ >= mesh_.motion_end_time() * 0.8f) {
		is_evading_ = false;
		walk(delta_time);
	}
}

void Player::attack1(float delta_time){
	combo_timer_ = 60.0f;
	combo_count_ = 1;
	if (state_timer_ >= weapon_->event_time_attack1() + 12.0f) {
		attack_handle(delta_time);
	}
	if (state_timer_ >= mesh_.motion_end_time() * weapon_->attack_motion_rate()) {
		walk(delta_time);
	}
}

void Player::attack2(float delta_time){
	combo_timer_ = 60.0f;
	combo_count_ = 2;
	if (state_timer_ >= weapon_->event_time_attack2() + 12.0f) {
		attack_handle(delta_time);
	}
	if (state_timer_ >= mesh_.motion_end_time() * weapon_->attack_motion_rate()) {
		walk(delta_time);
	}
}

void Player::attack3(float delta_time){
	combo_count_ = 0;
	if (state_timer_ >= mesh_.motion_end_time() * weapon_->attack_motion_rate()) {
		walk(delta_time);
	}
}

void Player::guard(float delta_time){
	move(delta_time, WalkSpeed * GameProgress::player_speed_rate_, State::Guard, weapon_->motion_guard(), weapon_->motion_move_fire(), true, true, true);
	input_handle(delta_time);
	attack_handle(delta_time);
	sp_used_ = true;
	sp_regen_timer_ = 0;
	if (!gsGetMouseButtonState(GMOUSE_BUTTON_2)) {
		change_state(State::Walk, weapon_->motion_walk_front());
		return;
	}
}

void Player::skill(float delta_time){
	if (state_timer_ >= mesh_.motion_end_time()) {
		walk(delta_time);
	}
}

void Player::down(float delta_time){
	if (state_timer_ >= 300.0f) {
		// �_�E�����[�V�������I�������玀�S
		die();
	}
}

void Player::damage(float delta_time) {
	// �m�b�N�o�b�N����i���[���h���W�n�j
	transform_.translate(velocity_ * delta_time, GStransform::Space::World);
	// ����������
	velocity_ -= GSvector3{ velocity_.x, 0.0f, velocity_.z } *0.5f * delta_time;
	// �_���[�W���[�V�����̏I����҂�
	if (state_timer_ >= mesh_.motion_end_time() * weapon_->damage_motion_rate()) {
		gsStopEffect(Effect_Hit);
		walk(delta_time);
	}
}

void Player::fire(float delta_time){
	move(delta_time, WalkSpeed * GameProgress::player_speed_rate_, State::Fire, weapon_->motion_fire(), weapon_->motion_move_fire(), true, true, true, true);
	
	if (!gsGetMouseButtonState(GMOUSE_BUTTON_1) && state_timer_ >= mesh_.motion_end_time()) {
		change_state(State::Walk, weapon_->motion_walk_front());
		return;
	}
}

void Player::move(float delta_time, float speed, State state, GSint motion_default, GSint motion_move, 
	bool layer_motion, bool enable_target, bool can_evade, bool can_attack){
	GSint motion{ motion_default };
	GSvector3 forward{ 0.0f, 0.0f, 0.0f };
	GSvector3 right{ 0.0f, 0.0f, 0.0f };
	GSvector3 velocity{ 0.0f, 0.0f, 0.0f };
	// �^�[�Q�b�g�Ɍ�����
	if (enable_target && is_targeting_ && targeted_enemy_ != nullptr) {
		forward = transform_.forward();
		forward.y = 0.0f;
		right = transform_.right();
		right.y = 0.0f;
		// �L�[�̓��͒l����ړ��x�N�g�����v�Z
		if (gsGetKeyState(GKEY_W)) velocity += forward;
		if (gsGetKeyState(GKEY_S)) velocity -= forward;
		if (gsGetKeyState(GKEY_A)) velocity -= right;
		if (gsGetKeyState(GKEY_D)) velocity += right;
		velocity = velocity.normalized() * speed * delta_time;
		float angle = target_signed_angle() * delta_time;
		transform_.rotate(0.0f, angle, 0.0f);
		if (velocity.length() != 0 && gsGetKeyState(GKEY_W)) motion = motion_move;
		else if (velocity.length() != 0.0f && !gsGetKeyState(GKEY_W)) motion = weapon_->motion_walk_back();
	}
	else {
		// �J�����̑O�����x�N�g�����擾
		forward = world_->camera()->transform().forward();
		forward.y = 0.0f;
		// �J�����̉E�����x�N�g�����擾
		right = world_->camera()->transform().right();
		right.y = 0.0f;
		// �L�[�̓��͒l����ړ��x�N�g�����v�Z
		if (gsGetKeyState(GKEY_W)) velocity += forward;
		if (gsGetKeyState(GKEY_S)) velocity -= forward;
		if (gsGetKeyState(GKEY_A)) velocity -= right;
		if (gsGetKeyState(GKEY_D)) velocity += right;
		velocity = velocity.normalized() * speed * delta_time;
		// �ړ����Ă��邩�H
		if (velocity.length() != 0.0f) {
			// �����̕��
			GSquaternion rotation = GSquaternion::rotateTowards(transform_.rotation(),
				GSquaternion::lookRotation(velocity), 12.0f * delta_time);
			transform_.rotation(rotation);
			motion = motion_move;
		}
	}
	
	// ���[�V�����̕ύX
	if (layer_motion) {
		if (can_attack) {
			if (state_timer_ >= mesh_.motion_end_time() * weapon_->attack_motion_rate()
				|| state_timer_ >= gsGetEndAnimationTime(1, motion)) {
				if (!gsGetKeyState(GKEY_W) && !gsGetKeyState(GKEY_S)
					&& !gsGetKeyState(GKEY_A) && !gsGetKeyState(GKEY_D)) {
					mesh_.change_motion(1, motion_default, lower_motion_loop_);	
				}
				else if (!gsGetMouseButtonState(GMOUSE_BUTTON_1)) {
					walk(delta_time);
				}
				else change_state(state, motion_default, true, motion, true);
			}
		}
		else change_state(state, motion_default, true, motion, true);
	}
	else {
		change_state(state, motion);
	}

	// �ړ��ʂ�xz���������X�V
	velocity_.x = velocity.x;
	velocity_.z = velocity.z;
	// ���s�ړ�����i���[���h��j
	transform_.translate(velocity_, GStransform::Space::World);

	if (can_evade) {
		if ((gsGetKeyState(GKEY_W) || gsGetKeyState(GKEY_S)
			|| gsGetKeyState(GKEY_A) || gsGetKeyState(GKEY_D))
			&& gsGetKeyTrigger(GKEY_LCONTROL) && stamina_ >= weapon_->evade_sp()) {
			velocity = velocity.normalized() * RunSpeed * GameProgress::player_speed_rate_ * delta_time;
			velocity_.x = velocity.x;
			velocity_.z = velocity.z;
			stamina_ -= weapon_->evade_sp();
			gsPlaySE(SE_Evade);
			is_evading_ = true;
			change_state(State::Evade, MotionGunEvadeF, false);
			return;
		}
	}
}

void Player::input_handle(float delta_time){
	if (gsGetKeyTrigger(GKEY_Q)) {
		gsPlaySE(SE_ChangeWeapon);
		change_weapon();
		return;
	}

	if (gsGetKeyTrigger(GKEY_SPACE)) {
		gsPlaySE(SE_Jump);
		velocity_.y += JumpPower * delta_time;
		transform_.translate(velocity_, GStransform::Space::World);
		change_state(State::Jump, weapon_->motion_jump(), false);
		return;
	}

	if (gsGetKeyTrigger(GKEY_E)) {
		change_state(State::Skill, weapon_->motion_skill());
		return;
	}
}

void Player::attack_handle(float delta_time){
	if (weapon_->has_combo()) {
		if (gsGetMouseButtonTrigger(GMOUSE_BUTTON_1) && combo_count_ == 0) {
			change_state(State::Attack1, weapon_->motion_attack1());
			return;
		}
		else if (gsGetMouseButtonTrigger(GMOUSE_BUTTON_1) && combo_count_ == 1) {
			change_state(State::Attack2, weapon_->motion_attack2());
			return;
		}
		else if (gsGetMouseButtonTrigger(GMOUSE_BUTTON_1) && combo_count_ == 2) {
			change_state(State::Attack3, weapon_->motion_attack3());
			return;
		}
	}
	else{
		if (gsGetMouseButtonState(GMOUSE_BUTTON_1)){
			change_state(State::Fire, weapon_->motion_fire());
			weapon_->start();
			return;
		}
	}
}

void Player::event_handle(Weapon* weapon){
	if (weapon->has_combo()) {
		mesh_.add_event(0, weapon->motion_attack1(), weapon->event_time_attack1(), [=] { weapon->event_combo_attack1(); });
		mesh_.add_event(0, weapon->motion_attack2(), weapon->event_time_attack2(), [=] { weapon->event_combo_attack2(); });
		mesh_.add_event(0, weapon->motion_attack3(), weapon->event_time_attack3(), [=] { weapon->event_combo_attack3(); });
	}
	else{
		mesh_.add_event(0, weapon->motion_fire(), weapon->event_time_fire(), [=] { weapon->event_fire(); });
	}

	mesh_.add_event(1, weapon->motion_walk_front(), 5.0f, [=] { gsPlaySE(SE_Walk); });
	mesh_.add_event(1, weapon->motion_walk_back(), 5.0f, [=] { gsPlaySE(SE_Walk); });
	mesh_.add_event(1, weapon->motion_walk_front(), 35.0f, [=] { gsPlaySE(SE_Walk); });
	mesh_.add_event(1, weapon->motion_walk_back(), 35.0f, [=] { gsPlaySE(SE_Walk); });
	mesh_.add_event(1, weapon->motion_run(), 1.0f, [=] { gsPlaySE(SE_Walk); });
	mesh_.add_event(1, weapon->motion_run(), 21.0f, [=] { gsPlaySE(SE_Walk); });

	// ImGui Debug
	//mesh_.add_event(weapon_->motion_attack1(), weapon_->event_time_attack1(), [=] { play_effect(Effect_MuzzleFlash, pos, rot, scale); });
	//mesh_.add_event(weapon_->motion_attack2(), weapon_->event_time_attack2(), [=] { play_effect(Effect_SlashDark, pos, rot, scale); });
	//mesh_.add_event(weapon_->motion_attack3(), weapon_->event_time_attack3(), [=] { play_effect(Effect_SlashDark, pos, rot, scale); });
}

void Player::draw_weapon() const{
	glPushMatrix();
	glMultMatrixf(mesh_.bone_matrices(weapon_->bone_position_right_hand()));
	gsDrawMesh(weapon_->mesh_number_right_hand());
	glPopMatrix();

	glPushMatrix();
	glMultMatrixf(mesh_.bone_matrices(weapon_->bone_position_left_hand()));
	gsDrawMesh(weapon_->mesh_number_left_hand());
	glPopMatrix();

	// bone Debug
	//glPushMatrix();
	//glMultMatrixf(mesh_.bone_matrices(bone));
	//gsDrawMesh(weapon_->mesh_number_right_hand());
	//glPopMatrix();
	//gsDrawText("�{�[���ԍ� = %d", bone);
}

void Player::collide_field(){
	// �ǂƂ̏Փ˔���i���̂Ƃ̔���j
	GSvector3 center; // �����߂���̋��̂̒��S���W
	if (world_->field()->collide(collider(), &center)) {
		// y���W�͕ύX���Ȃ�
		center.y = transform_.position().y;
		// �␳��̍��W�ɕύX����
		transform_.position(center);
	}
	// �n�ʂƂ̏Փ˔���i�����Ƃ̌�������j
	GSvector3 position = transform_.position();
	Line line;
	line.start = position + collider_.center;
	line.end = position + GSvector3{ 0.0f, -FootOffset, 0.0f };
	GSvector3 intersect; // �n�ʂƂ̌�_
	if (world_->field()->collide(line, &intersect)) {
		// ��_�̈ʒu����y���W�̂ݕ␳����
		position.y = intersect.y;
		// ���W��ύX����
		transform_.position(position);
		// �d�͂�����������
		velocity_.y = 0.0f;
	}
}

void Player::collide_actor(Actor& other){
	// �����W���������W�����߂�
	GSvector3 position = transform_.position();
	position.y = 0.0f;
	GSvector3 target = other.transform().position();
	target.y = 0.0f;
	// ����Ƃ̋���
	float distance = GSvector3::distance(position, target);
	// �Փ˔��苅�̔��a���m�����������������߂�
	float length = collider_.radius + other.collider().radius;
	// �Փ˔��苅�̏d�Ȃ��Ă��钷�������߂�
	float overlap = length - distance;
	// �d�Ȃ��Ă��镔���̔����̋������������ړ��ʂ����߂�
	GSvector3 v = (position - target).getNormalized() * overlap * 0.5f;
	transform_.translate(v, GStransform::Space::World);
	// �t�B�[���h�Ƃ̏Փ˔���
	collide_field();
}

//���b�N�I���@�\
void Player::target_enemy(float delta_time){
	//�S�Ă̓G���������A���X�g�Ɋi�[����
	enemies_ = world_->find_actor_with_tag("EnemyTag");
	//������J�����O���擾����
	GSfrustum* frustum = gsGetFrustum();
	//�v���C���[�̈ʒu����̎n�_�ɂ���
	target_line_.start = transform_.position() + transform_.position().up();
	//�S�Ă̓G�ɐ���������
	for (Actor* en : enemies_) {
		GSvector3 center = en->collider().center;
		float radius = en->collider().radius;
		//�G��������J�����O�̒��ɂ�����
		if (gsFrustumIsSphereInside(frustum, &center, radius)) {
			GSvector3 intersect;
			//�G�̈ʒu����̏I�_�ɂ���
			target_line_.end = en->transform().position() + en->transform().position().up();
			//�����ǂɂԂ�������A�v���C���[�̎��_���炻�̓G�������Ȃ��ƃt���O�𗧂Ă�
			if (world_->field()->collide(target_line_, &intersect)) en->set_visible(false);
			//�����ǂɂԂ���Ȃ������猩����
			else en->set_visible(true);
		}
		//������J�����O�̊O�ɂ����猩���Ȃ�
		else {
			en->set_visible(false);
		}
	}

	//�v���C���[���G�����b�N�I�����Ă��Ȃ����ATAB�L�[����������
	if (!is_targeting_ && gsGetKeyTrigger(GKEY_TAB)) {
		for (Actor* en : enemies_) {
			//�����Ȃ�������X�L�b�v
			if (!en->get_visible()) continue;
			//��������
			else if (en->get_visible()) {
				//���݃��b�N�I�����ꂽ�G�ɂ���
				targeted_enemy_ = en;
				//���b�N�I�����ꂽ���Ƃ�����ƃt���O�𗧂Ă�
				en->set_targeted(true);
				//�v���C���[�������b�N�I�����Ă���
				is_targeting_ = true;
				//���b�N�I�����ꂽ�G�̈ʒu�ŃG�t�F�N�g���Đ�����
				gsStopEffect(target_effect_);
				target_effect_ = gsPlayEffectEx(Effect_AuraYellow, 
					new GSmatrix4(targeted_enemy_->get_world_matrix
					(GSvector3{ 0.0f, 1.0f, 0.0f }, 
						GSvector3(0.0f, 0.0f, 0.0f), 
						GSvector3(1.2f, 1.2f, 1.2f))));
				break;
			}
		}
	}
	//�v���C���[���G�����b�N�I�����Ă��鎞
	else if (is_targeting_) {
		//TAB�L�[����������
		if (gsGetKeyTrigger(GKEY_TAB)) {
			for (auto en = enemies_.begin(); en != enemies_.end();) {
				//�����Ȃ�������X�L�b�v
				if (!(*en)->get_visible()) ++en;
				//��������A���A���b�N�I�����ꂽ���Ƃ��Ȃ�������
				else if ((*en)->get_visible() && !(*en)->get_targeted()) {
					//���݃��b�N�I�����ꂽ�G�ɂ���i�Ώۂ�ύX����j
					targeted_enemy_ = *en;
					//���b�N�I�����ꂽ���Ƃ�����ƃt���O�𗧂Ă�
					(*en)->set_targeted(true);
					//�v���C���[�������b�N�I�����Ă���
					is_targeting_ = true;
					//���b�N�I�����ꂽ�G�̈ʒu�ŃG�t�F�N�g���Đ�����
					gsStopEffect(target_effect_);
					target_effect_ = gsPlayEffectEx(Effect_AuraYellow,
						new GSmatrix4(targeted_enemy_->get_world_matrix
						(GSvector3{ 0.0f, 1.0f, 0.0f },
							GSvector3(0.0f, 0.0f, 0.0f),
							GSvector3(1.2f, 1.2f, 1.2f))));
					break;
				}
				//���̓X�L�b�v
				else {
					++en;
				}
				//���X�g���̍Ō�̏��Ԃ��߂�����
				if (en == enemies_.end()) {
					//�S�Ẵ��b�N�I���̃t���O�����Z�b�g����
					for (Actor* e : enemies_) {
						e->set_targeted(false);
					}
					//���b�N�I���Ώۂ����Z�b�g����
					targeted_enemy_ = nullptr;
					//���b�N��������
					is_targeting_ = false;
					//�G�t�F�N�g���~�߂�
					gsStopEffect(target_effect_);
				}
			}
		}
		//TAB�L�[�𒷉���������A
		else if (gsGetKeyState(GKEY_TAB)) {
			//���b�N�����̃^�C�}�[���J�E���g�_�E������
			untarget_timer_ -= delta_time;
		}
		//TAB�L�[�����������
		else if (gsGetKeyDetach(GKEY_TAB)) {
			//���b�N�����̃^�C�}�[���ď���������
			untarget_timer_ = 30.0f;
		}
		//���b�N�I���Ώۂ����S������
		else if (targeted_enemy_->is_dead()) {
			//�S�Ẵ��b�N�I���̃t���O�����Z�b�g����
			for (Actor* e : enemies_) {
				e->set_targeted(false);
			}
			//���b�N��������
			is_targeting_ = false;
			gsStopEffect(target_effect_);
		}
		//���b�N�����̃^�C�}�[��0�ɂȂ�����
		if (untarget_timer_ <= 0) {
			//�S�Ẵ��b�N�I���̃t���O�����Z�b�g����
			for (Actor* e : enemies_) {
				e->set_targeted(false);
			}
			//���b�N�����̃^�C�}�[���ď���������
			untarget_timer_ = 30.0f;
			//���b�N�I���Ώۂ����Z�b�g����
			targeted_enemy_ = nullptr;
			//���b�N��������
			is_targeting_ = false;
			//�G�t�F�N�g���~�߂�
			gsStopEffect(target_effect_);
		}
	}

	//���b�N�I���Ώۂ̍s����擾���A�G�t�F�N�g�̍s����X�V����
	if (is_targeting_ && targeted_enemy_ != nullptr) {
		gsSetEffectMatrix(target_effect_, 
			new GSmatrix4(targeted_enemy_->get_world_matrix
			(GSvector3{ 0.0f, 1.0f, 0.0f }, 
				GSvector3(0.0f, 0.0f, 0.0f), 
				GSvector3(1.2f, 1.2f, 1.2f))));
	}
}

float Player::target_signed_angle() const {
	// �^�[�Q�b�g�����Ȃ����0��Ԃ�
	if (targeted_enemy_ == nullptr) return 0.0f;
	// �^�[�Q�b�g�����̃x�N�g�������߂�
	GSvector3 to_target = targeted_enemy_->transform().position() - transform_.position();
	// �O���������̃x�N�g�����擾
	GSvector3 forward = transform_.forward();
	// �x�N�g����y�����𖳌��ɂ���
	forward.y = 0.0f;
	to_target.y = 0.0f;
	// �O���������̃x�N�g���ƃ^�[�Q�b�g�����̃x�N�g���̊p�x�������߂�
	return GSvector3::signedAngle(forward, to_target);
}
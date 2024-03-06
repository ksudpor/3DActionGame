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

const float WalkSpeed{ 0.03f };		// 歩く時の移動速度
const float RunSpeed{ 0.075f };		// 走る時の移動速度
const float PlayerHeight{ 1.0f };	// 自分の高さ
const float PlayerRadius{ 0.45f };	// 衝突判定用の半径
const float FootOffset{ 0.1f };		// 足元のオフセット
const float Gravity{ -0.008f };		// 重力値
const float JumpPower{ 0.12 };		// 跳ねる力
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
	// ワールドを設定
	world_ = world;
	// タグ名の設定
	tag_ = "PlayerTag";
	// 名前の設定
	name_ = "Player";
	// 衝突判定球の設定
	collider_ = BoundingSphere{ PlayerRadius, GSvector3{0.0f, PlayerHeight, 0.0f} };
	// 座標の初期化
	transform_.position(position);
	// メッシュの変換行列を初期化
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

	// 下半身のボーンＩＤ
	static const GSuint lower_body_bone_id[] =
	{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	// 下半身ボーンをレイヤー１に設定（それ以外の上半身ボーンはレイヤー０）
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

	// 衝突判定球のデバッグ表示
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
	//gsFontParameter(0, Screen::Height * 0.025, "ＭＳ ゴシック");
	//gsTextPos(Screen::Width * 0.08f, Screen::Height * 0.5f);
	//gsDrawText("%f, %f, %f", transform_.position().x, transform_.position().y, transform_.position().z);
}

void Player::react(Actor& other){
	// ダメージ中の場合は何もしない
	if (state_ == State::Damage || state_ == State::Down) return;
	int damage = other.damage() - (BaseDefend + (float)weapon_->defend() / (float)other.damage());
	int guard_damage = other.damage() - (BaseDefend + (weapon_->defend() * 2));
	damage = CLAMP(damage, 0, damage);
	guard_damage = CLAMP(guard_damage, 0, guard_damage);

	// 敵の攻撃判定と衝突したか？
	if ((other.tag() == "EnemyAttackTag" || other.tag() == "EnemyBulletTag") && !is_evading_) {
		if (state_ == State::Guard && stamina_ >= weapon_->guard_sp()) {
			health_ -= guard_damage;
			stamina_ -= weapon_->guard_sp();
			other.play_effect(Effect_Guard, GSvector3{ 0.0f,0.0f,0.0f}, GSvector3{ 0.0f,180.0f,0.0f }, GSvector3{ 1.0f,1.0f,1.0f });
			gsPlaySE(SE_Guard);
			return;
		}

		// 体力を減らす
		health_ -= damage;
		sp_used_ = true;
		sp_regen_timer_ = 0;

		GSmatrix4 localMat = GSmatrix4::TRS(GSvector3{ 0.0f,0.8f,0.0f }, GSquaternion::euler(GSvector3(0.0f, 0.0f, 0.0f)), GSvector3{ 0.5f, 0.5f, 0.5f });
		GSmatrix4 worldMat = localMat * transform_.localToWorldMatrix();
		hit_effect_ = gsPlayEffectEx(Effect_Hit, &worldMat);

		if (health_ <= 0) {
			// 残りの体力がなければダウン状態に遷移
			gsPlaySE(SE_PlayerDie);
			change_state(State::Down, weapon_->motion_die(), false);
		}
		else {
			if (state_ == State::Attack1 || state_ == State::Attack2 
				|| state_ == State::Attack3 && gsRand(1,100) < 50) {
				return;
			}
			if (other.tag() == "EnemyAttackTag") {
				// ターゲット方向のベクトルを求める
				GSvector3 to_target = other.transform().position() - transform().position();
				// ｙ成分は無効にする
				to_target.y = 0.0f;
				// ターゲット方向と逆方向にノックバックする移動量を求める
				velocity_ = -to_target.getNormalized() * 0.4f;
			}
			// ダメージ状態に遷移する
			gsPlaySE(SE_PlayerDamage);
			change_state(State::Damage, weapon_->motion_damage(), false);
			return;
		}
	}
	// 敵と衝突したか？
	if (other.tag() == "EnemyTag") {
		// アクター同士が重ならないように補正する
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
		// ダウンモーションが終了したら死亡
		die();
	}
}

void Player::damage(float delta_time) {
	// ノックバックする（ワールド座標系）
	transform_.translate(velocity_ * delta_time, GStransform::Space::World);
	// 減速させる
	velocity_ -= GSvector3{ velocity_.x, 0.0f, velocity_.z } *0.5f * delta_time;
	// ダメージモーションの終了を待つ
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
	// ターゲットに向かう
	if (enable_target && is_targeting_ && targeted_enemy_ != nullptr) {
		forward = transform_.forward();
		forward.y = 0.0f;
		right = transform_.right();
		right.y = 0.0f;
		// キーの入力値から移動ベクトルを計算
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
		// カメラの前方向ベクトルを取得
		forward = world_->camera()->transform().forward();
		forward.y = 0.0f;
		// カメラの右方向ベクトルを取得
		right = world_->camera()->transform().right();
		right.y = 0.0f;
		// キーの入力値から移動ベクトルを計算
		if (gsGetKeyState(GKEY_W)) velocity += forward;
		if (gsGetKeyState(GKEY_S)) velocity -= forward;
		if (gsGetKeyState(GKEY_A)) velocity -= right;
		if (gsGetKeyState(GKEY_D)) velocity += right;
		velocity = velocity.normalized() * speed * delta_time;
		// 移動しているか？
		if (velocity.length() != 0.0f) {
			// 向きの補間
			GSquaternion rotation = GSquaternion::rotateTowards(transform_.rotation(),
				GSquaternion::lookRotation(velocity), 12.0f * delta_time);
			transform_.rotation(rotation);
			motion = motion_move;
		}
	}
	
	// モーションの変更
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

	// 移動量のxz成分だけ更新
	velocity_.x = velocity.x;
	velocity_.z = velocity.z;
	// 平行移動する（ワールド基準）
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
	//gsDrawText("ボーン番号 = %d", bone);
}

void Player::collide_field(){
	// 壁との衝突判定（球体との判定）
	GSvector3 center; // 押し戻し後の球体の中心座標
	if (world_->field()->collide(collider(), &center)) {
		// y座標は変更しない
		center.y = transform_.position().y;
		// 補正後の座標に変更する
		transform_.position(center);
	}
	// 地面との衝突判定（線分との交差判定）
	GSvector3 position = transform_.position();
	Line line;
	line.start = position + collider_.center;
	line.end = position + GSvector3{ 0.0f, -FootOffset, 0.0f };
	GSvector3 intersect; // 地面との交点
	if (world_->field()->collide(line, &intersect)) {
		// 交点の位置からy座標のみ補正する
		position.y = intersect.y;
		// 座標を変更する
		transform_.position(position);
		// 重力を初期化する
		velocity_.y = 0.0f;
	}
}

void Player::collide_actor(Actor& other){
	// ｙ座標を除く座標を求める
	GSvector3 position = transform_.position();
	position.y = 0.0f;
	GSvector3 target = other.transform().position();
	target.y = 0.0f;
	// 相手との距離
	float distance = GSvector3::distance(position, target);
	// 衝突判定球の半径同士を加えた長さを求める
	float length = collider_.radius + other.collider().radius;
	// 衝突判定球の重なっている長さを求める
	float overlap = length - distance;
	// 重なっている部分の半分の距離だけ離れる移動量を求める
	GSvector3 v = (position - target).getNormalized() * overlap * 0.5f;
	transform_.translate(v, GStransform::Space::World);
	// フィールドとの衝突判定
	collide_field();
}

//ロックオン機能
void Player::target_enemy(float delta_time){
	//全ての敵を検索し、リストに格納する
	enemies_ = world_->find_actor_with_tag("EnemyTag");
	//視錐台カリングを取得する
	GSfrustum* frustum = gsGetFrustum();
	//プレイヤーの位置を線の始点にする
	target_line_.start = transform_.position() + transform_.position().up();
	//全ての敵に線を引いて
	for (Actor* en : enemies_) {
		GSvector3 center = en->collider().center;
		float radius = en->collider().radius;
		//敵が視錐台カリングの中にいたら
		if (gsFrustumIsSphereInside(frustum, &center, radius)) {
			GSvector3 intersect;
			//敵の位置を線の終点にする
			target_line_.end = en->transform().position() + en->transform().position().up();
			//線が壁にぶつかったら、プレイヤーの視点からその敵が見えないとフラグを立てる
			if (world_->field()->collide(target_line_, &intersect)) en->set_visible(false);
			//線が壁にぶつからなかったら見える
			else en->set_visible(true);
		}
		//視錐台カリングの外にいたら見えない
		else {
			en->set_visible(false);
		}
	}

	//プレイヤーが敵をロックオンしていない時、TABキーを押したら
	if (!is_targeting_ && gsGetKeyTrigger(GKEY_TAB)) {
		for (Actor* en : enemies_) {
			//見えなかったらスキップ
			if (!en->get_visible()) continue;
			//見えたら
			else if (en->get_visible()) {
				//現在ロックオンされた敵にする
				targeted_enemy_ = en;
				//ロックオンされたことがあるとフラグを立てる
				en->set_targeted(true);
				//プレイヤーがをロックオンしている
				is_targeting_ = true;
				//ロックオンされた敵の位置でエフェクトを再生する
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
	//プレイヤーが敵をロックオンしている時
	else if (is_targeting_) {
		//TABキーを押したら
		if (gsGetKeyTrigger(GKEY_TAB)) {
			for (auto en = enemies_.begin(); en != enemies_.end();) {
				//見えなかったらスキップ
				if (!(*en)->get_visible()) ++en;
				//見えたら、かつ、ロックオンされたことがなかったら
				else if ((*en)->get_visible() && !(*en)->get_targeted()) {
					//現在ロックオンされた敵にする（対象を変更する）
					targeted_enemy_ = *en;
					//ロックオンされたことがあるとフラグを立てる
					(*en)->set_targeted(true);
					//プレイヤーがをロックオンしている
					is_targeting_ = true;
					//ロックオンされた敵の位置でエフェクトを再生する
					gsStopEffect(target_effect_);
					target_effect_ = gsPlayEffectEx(Effect_AuraYellow,
						new GSmatrix4(targeted_enemy_->get_world_matrix
						(GSvector3{ 0.0f, 1.0f, 0.0f },
							GSvector3(0.0f, 0.0f, 0.0f),
							GSvector3(1.2f, 1.2f, 1.2f))));
					break;
				}
				//他はスキップ
				else {
					++en;
				}
				//リスト内の最後の順番を過ぎたら
				if (en == enemies_.end()) {
					//全てのロックオンのフラグをリセットする
					for (Actor* e : enemies_) {
						e->set_targeted(false);
					}
					//ロックオン対象をリセットする
					targeted_enemy_ = nullptr;
					//ロック解除する
					is_targeting_ = false;
					//エフェクトを止める
					gsStopEffect(target_effect_);
				}
			}
		}
		//TABキーを長押ししたら、
		else if (gsGetKeyState(GKEY_TAB)) {
			//ロック解除のタイマーをカウントダウンする
			untarget_timer_ -= delta_time;
		}
		//TABキーを解放したら
		else if (gsGetKeyDetach(GKEY_TAB)) {
			//ロック解除のタイマーを再初期化する
			untarget_timer_ = 30.0f;
		}
		//ロックオン対象が死亡したら
		else if (targeted_enemy_->is_dead()) {
			//全てのロックオンのフラグをリセットする
			for (Actor* e : enemies_) {
				e->set_targeted(false);
			}
			//ロック解除する
			is_targeting_ = false;
			gsStopEffect(target_effect_);
		}
		//ロック解除のタイマーが0になったら
		if (untarget_timer_ <= 0) {
			//全てのロックオンのフラグをリセットする
			for (Actor* e : enemies_) {
				e->set_targeted(false);
			}
			//ロック解除のタイマーを再初期化する
			untarget_timer_ = 30.0f;
			//ロックオン対象をリセットする
			targeted_enemy_ = nullptr;
			//ロック解除する
			is_targeting_ = false;
			//エフェクトを止める
			gsStopEffect(target_effect_);
		}
	}

	//ロックオン対象の行列を取得し、エフェクトの行列を更新する
	if (is_targeting_ && targeted_enemy_ != nullptr) {
		gsSetEffectMatrix(target_effect_, 
			new GSmatrix4(targeted_enemy_->get_world_matrix
			(GSvector3{ 0.0f, 1.0f, 0.0f }, 
				GSvector3(0.0f, 0.0f, 0.0f), 
				GSvector3(1.2f, 1.2f, 1.2f))));
	}
}

float Player::target_signed_angle() const {
	// ターゲットがいなければ0を返す
	if (targeted_enemy_ == nullptr) return 0.0f;
	// ターゲット方向のベクトルを求める
	GSvector3 to_target = targeted_enemy_->transform().position() - transform_.position();
	// 前向き方向のベクトルを取得
	GSvector3 forward = transform_.forward();
	// ベクトルのy成分を無効にする
	forward.y = 0.0f;
	to_target.y = 0.0f;
	// 前向き方向のベクトルとターゲット方向のベクトルの角度差を求める
	return GSvector3::signedAngle(forward, to_target);
}
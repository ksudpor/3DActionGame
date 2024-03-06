#include "EnemyRanged.h"
#include "IWorld.h"
#include "Field.h"
#include "Line.h"
#include "Assets.h"
#include "EnemyBullet.h"
#include "HealthPotion.h"

enum {
    MotionIdle = 3,
    MotionWalk = 1,
    MotionTurnLeft = 5,
    MotionTurnRight = 6,
    MotionDamage = 2,
    MotionAttack = 4,
    MotionDown = 0
};

// 振り向く距離
const float TurnDistance{ 3.0f };
// 攻撃範囲
const float AttackDistance{ 15.0f }; 
const float AttackCooldown{ 120.0f };
// 距離
const float WalkDistance{ 20.0f };
// 移動スピード
const float WalkSpeed{ 0.025f };
// 振り向く角度
const float TurnAngle{ 2.5f };
// 自分の高さ
const float EnemyHeight{ 1.0f };
// 衝突判定用の半径
const float EnemyRadius{ 0.6f };
// 足元のオフセット
const float FootOffset{ 0.1f };
// 重力
const float Gravity{ -0.016f };
const int MaxHP{ 300 };

EnemyRanged::EnemyRanged(IWorld* world, const GSvector3& position) :
    mesh_{ Mesh_EnemyRanged, Skel_EnemyRanged, Anim_EnemyRanged, MotionIdle, true },
    upper_motion_{ MotionIdle },
    upper_motion_loop_{ true },
    state_{ State::Idle },
    state_timer_{ 0.0f },
    player_{ nullptr },
    health_{ MaxHP },
    is_hit_{ false },
    hit_timer_{ 0.0f },
    attacked_{ false },
    attack_timer_{ 0.0f } {
    world_ = world;
    tag_ = "EnemyTag";
    name_ = "EnemyRanged";
    collider_ = BoundingSphere{ EnemyRadius, GSvector3{0.0f, EnemyHeight, 0.0f} };
    transform_.position(position);
    mesh_.transform(transform_.localToWorldMatrix());
    mesh_.add_event(0, MotionAttack, 15.0f, [=] {generate_bullet(); });
}

void EnemyRanged::update(float delta_time) {
    // プレーヤーを検索する
    player_ = world_->find_actor("Player");
    // 状態の更新
    update_state(delta_time);
    // 重力を更新
    velocity_.y += Gravity * delta_time;
    // 重力を加える
    transform_.translate(0.0f, velocity_.y, 0.0f);
    // フィールドとの衝突判定
    collide_field();

    mesh_.change_motion(upper_motion_, upper_motion_loop_);
    mesh_.update(delta_time);
    mesh_.transform(transform_.localToWorldMatrix());

    if (is_hit_) hit_timer_ += delta_time;
    if (hit_timer_ > 25.0f) is_hit_ = false;

    if (attacked_) {
        attack_timer_ += delta_time;
        if (is_attack()){
            float angle = (target_signed_angle() >= 0.0f) ? TurnAngle : -TurnAngle;
            transform_.rotate(0.0f, angle * delta_time, 0.0f);
        }
    }
    if (attack_timer_ > AttackCooldown) attacked_ = false;
}

void EnemyRanged::draw() const {
    mesh_.draw();
    //collider().draw();
}

void EnemyRanged::react(Actor& other) {
    // ダウン中の場合は何もしない
    if (state_ == State::Down) return;

    // プレーヤーの弾に衝突したら
    if (other.tag() == "PlayerBulletTag") {
        // 体力を減らす
        health_ -= other.damage();
        play_effect(Effect_Hit, GSvector3{ 0.0f,1.0f,0.0f }, GSvector3{ 0.0f,0.0f,0.0f }, GSvector3{ 0.7f,0.7f,0.7f });
        if (health_ <= 0) {
            gsPlaySE(SE_EnemyDie);
            // 残りの体力がなければダウン状態に遷移
            change_state(State::Down, MotionDown, false);
        }
        else {
            if (state_ != State::Damage) {
                // 弾の進行方向にノックバックする移動量を求める
                //velocity_ = other.velocity().getNormalized() * 0.5f;
                // ダメージ状態に遷移
                gsPlaySE(SE_EnemyDamage);
                if (gsRand(1, 100) < 25) change_state(State::Damage, MotionDamage, false);
                return;
            }
        }
    }

    if (other.tag() == "PlayerAttackTag" && !is_hit_) {
        // 体力を減らす
        health_ -= other.damage();
        play_effect(Effect_Hit, GSvector3{ 0.0f,1.0f,0.0f }, GSvector3{ 0.0f,0.0f,0.0f }, GSvector3{ 0.7f,0.7f,0.7f });
        is_hit_ = true;
        hit_timer_ = 0;
        if (health_ <= 0) {
            // 残りの体力がなければダウン状態に遷移
            gsPlaySE(SE_EnemyDie);
            change_state(State::Down, MotionDown, false);
        }
        else {
            if (state_ != State::Damage) {
                // ターゲット方向のベクトルを求める
                GSvector3 to_target = other.transform().position() - transform().position();
                // ｙ成分は無効にする
                to_target.y = 0.0f;
                // ターゲット方向と逆方向にノックバックする移動量を求める
                velocity_ = -to_target.getNormalized() * 0.4f;
                // ダメージ状態に遷移する
                gsPlaySE(SE_EnemyDamage);
                change_state(State::Damage, MotionDamage, false);
                return;
            }
        }
    }

    // プレーヤーまたは敵に衝突したら
    if (other.tag() == "PlayerTag" || other.tag() == "EnemyTag") {
        collide_actor(other);
    }
}

void EnemyRanged::update_state(float delta_time) {
    switch (state_) {
    case State::Idle: idle(delta_time); break;
    case State::Attack: attack(delta_time); break;
    case State::Walk: walk(delta_time); break;
    case State::Damage: damage(delta_time); break;
    case State::Turn: turn(delta_time); break;
    case State::Down: down(delta_time); break;
    }
    state_timer_ += delta_time;
}

void EnemyRanged::change_state(State state, GSuint motion, bool loop) {
    upper_motion_ = motion;
    upper_motion_loop_ = loop;
    state_ = state;
    state_timer_ = 0.0f;
}

void EnemyRanged::idle(float delta_time) {
    if (player_ != nullptr) {
        Line line;
        line.start = transform_.position() + transform_.position().up();
        line.end = player_->transform().position() + player_->transform().position().up();

        GSvector3 intersect;
        if (world_->field()->collide(line, &intersect)) {
            change_state(State::Idle, MotionIdle);
            return;
        }
    }

    if (is_attack() && !attacked_) {
        change_state(State::Attack, MotionAttack);
        return;
    }
    if (is_walk() && !attacked_) {
        change_state(State::Walk, MotionWalk);
        return;
    }
    if (is_turn()) {
        GSuint motion = (target_signed_angle() >= 0.0f) ? MotionTurnLeft : MotionTurnRight;
        change_state(State::Turn, motion);
        return;
    }
    change_state(State::Idle, MotionIdle);
}

void EnemyRanged::walk(float delta_time) {
    if (player_ != nullptr) {
        Line line;
        line.start = transform_.position() + transform_.position().up();
        line.end = player_->transform().position() + player_->transform().position().up();

        GSvector3 intersect;
        if (world_->field()->collide(line, &intersect)) {
            change_state(State::Idle, MotionIdle);
            return;
        }
    }

    float angle = target_signed_angle();
    angle = CLAMP(angle, -TurnAngle, TurnAngle) * delta_time;
    transform_.rotate(0.0f, angle, 0.0f);
    transform_.translate(0.0f, 0.0f, WalkSpeed * delta_time);
    if (is_attack() && !attacked_) {
        change_state(State::Attack, MotionAttack);
    }
}

void EnemyRanged::turn(float delta_time) {
    if (state_timer_ >= mesh_.motion_end_time() * 0.8f) {
        idle(delta_time);
    }
    else {
        float angle = (target_signed_angle() >= 0.0f) ? TurnAngle : -TurnAngle;
        transform_.rotate(0.0f, angle * delta_time, 0.0f);
    }
}

void EnemyRanged::attack(float delta_time) {
    attack_timer_ = 0;

    if (state_timer_ >= mesh_.motion_end_time()) {
        attacked_ = true;
        attack_timer_ = 0;
        idle(delta_time);
    }
}

void EnemyRanged::damage(float delta_time) {
    if (state_timer_ < mesh_.motion_end_time()) {
        // ノックバックする
        transform_.translate(velocity_ * delta_time, GStransform::Space::World);
        velocity_ -= GSvector3{ velocity_.x, 0.0f, velocity_.z } *0.5f * delta_time;
        return;
    }
    if (target_angle() >= 90.0f) {
        GSuint motion = (target_signed_angle() >= 0.0f) ? MotionTurnLeft : MotionTurnRight;
        change_state(State::Turn, motion);
    }
    else {
        idle(delta_time);
    }
}

void EnemyRanged::down(float delta_time) {
    enable_collider_ = false;

    if (state_timer_ >= mesh_.motion_end_time()) {
        if (gsRand(1, 100) < 20) world_->add_actor(new HealthPotion(world_, transform_.position()));
        world_->add_score(100);
        // ダウンモーションが終了したら死亡
        die();
    }
}

bool EnemyRanged::is_turn() const {
    // 振り向き距離内かつ前向き方向のベクトルとターゲット方向のベクトルの角度差が90.0度以上か？
    return (target_distance() <= TurnDistance) && (target_angle() >= 90.0f);
}

bool EnemyRanged::is_attack() const {
    // 攻撃距離内かつ前向き方向のベクトルとターゲット方向のベクトルの角度差が20.0度以下か？
    return (target_distance() <= AttackDistance) && (target_angle() <= 20.0f);
}

bool EnemyRanged::is_walk() const {
    // 移動距離内かつ前方向と前向き方向のベクトルとターゲット方向のベクトルの角度差が100.0度以下か？
    return (target_distance() <= WalkDistance) && (target_angle() <= 100.0f);
}

float EnemyRanged::target_signed_angle() const {
    // ターゲットがいなければ0を返す
    if (player_ == nullptr) return 0.0f;
    // ターゲット方向のベクトルを求める
    GSvector3 to_target = player_->transform().position() - transform_.position();
    // 前向き方向のベクトルを取得
    GSvector3 forward = transform_.forward();
    // ベクトルのy成分を無効にする
    forward.y = 0.0f;
    to_target.y = 0.0f;
    // 前向き方向のベクトルとターゲット方向のベクトルの角度差を求める
    return GSvector3::signedAngle(forward, to_target);
}

float EnemyRanged::target_angle() const {
    return std::abs(target_signed_angle());
}

float EnemyRanged::target_distance() const {
    // ターゲットがいなければ最大距離を返す
    if (player_ == nullptr) return FLT_MAX; // float型の最大値
    // ターゲットとの距離を計算する
    return GSvector3::distance(player_->transform().position(), transform_.position());
}

void EnemyRanged::collide_field() {
    // 壁との衝突判定（球体との判定）
    GSvector3 center; // 衝突後の球体の中心座標
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
        // 交差した点からy座標のみ補正する
        position.y = intersect.y;
        // 座標を変更する
        transform_.position(position);
        // 重力を初期化する
        velocity_.y = 0.0f;
    }
}

void EnemyRanged::collide_actor(Actor& other) {
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

void EnemyRanged::generate_bullet() {
    gsPlaySE(SE_EnemyRanged);
    // 弾を生成する場所の距離
    const float GenerateDistance{ 0.5f };
    // wwww生成する位置の高さの補正値
    const float GenerateHeight{ 1.2f };
    // 弾の移動スピード
    const float Speed{ 0.5f };
    const int damage = 100;
    // 生成位置の計算
    GSvector3 position = transform_.position() + transform_.forward() * GenerateDistance;
    // 生成位置の高さを補正する
    position.y += GenerateHeight;
    GSquaternion rotation = transform_.rotation();
    // 移動量の計算
    GSvector3 velocity = transform_.forward() * Speed;
    // 弾の生成
    world_->add_actor(new EnemyBullet{ world_, position, rotation, velocity, damage });   
}

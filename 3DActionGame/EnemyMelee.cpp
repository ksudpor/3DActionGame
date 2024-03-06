#include "EnemyMelee.h"
#include "IWorld.h"
#include "Field.h"
#include "Line.h"
#include "Assets.h"
#include "AttackCollider.h"
#include "HealthPotion.h"

enum {
    MotionIdle = 3,
    MotionWalk = 2,
    MotionTurnLeft = 5,
    MotionTurnRight = 6,
    MotionDamage = 1,
    MotionAttack = 0,
    MotionDown = 4
};

// �U���������
const float TurnDistance{ 3.0f };
// �U���͈�
const float AttackDistance{ 1.2f };
const float AttackCooldown{ 90.0f };
// ����
const float WalkDistance{ 20.0f };
// �ړ��X�s�[�h
const float WalkSpeed{ 0.025f };
// �U������p�x
const float TurnAngle{ 2.5f };
// �����̍���
const float EnemyHeight{ 1.0f };
// �Փ˔���p�̔��a
const float EnemyRadius{ 0.6f };
// �����̃I�t�Z�b�g
const float FootOffset{ 0.1f };
// �d��
const float Gravity{ -0.016f };
const int MaxHP{ 300 };

EnemyMelee::EnemyMelee(IWorld* world, const GSvector3& position) :
    mesh_{ Mesh_EnemyMelee, Skel_EnemyMelee, Anim_EnemyMelee, MotionIdle, true },
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
    name_ = "EnemyMelee";
    collider_ = BoundingSphere{ EnemyRadius, GSvector3{0.0f, EnemyHeight, 0.0f} };
    transform_.position(position);
    mesh_.transform(transform_.localToWorldMatrix());
    mesh_.add_event(0, MotionAttack, 30.0f, [=] {generate_attack_collider(); });
}

void EnemyMelee::update(float delta_time){
    // �v���[���[����������
    player_ = world_->find_actor("Player");
    // ��Ԃ̍X�V
    update_state(delta_time);
    // �d�͂��X�V
    velocity_.y += Gravity * delta_time;
    // �d�͂�������
    transform_.translate(0.0f, velocity_.y, 0.0f);
    // �t�B�[���h�Ƃ̏Փ˔���
    collide_field();

    mesh_.change_motion(upper_motion_, upper_motion_loop_);
    mesh_.update(delta_time);
    mesh_.transform(transform_.localToWorldMatrix());
    
    if (is_hit_) hit_timer_ += delta_time;
    if (hit_timer_ > 25.0f) is_hit_ = false;

    if (attacked_) {
        attack_timer_ += delta_time;
        if (is_attack()) {
            float angle = (target_signed_angle() >= 0.0f) ? TurnAngle : -TurnAngle;
            transform_.rotate(0.0f, angle * delta_time, 0.0f);
        }
    }
    if (attack_timer_ > AttackCooldown) attacked_ = false;
}

void EnemyMelee::draw() const{
    mesh_.draw();
    //collider().draw();
}

void EnemyMelee::react(Actor& other){
    // �_�E�����̏ꍇ�͉������Ȃ�
    if (state_ == State::Down) return;
    
    // �v���[���[�̒e�ɏՓ˂�����
    if (other.tag() == "PlayerBulletTag") {
        // �̗͂����炷
        health_ -= other.damage();
        play_effect(Effect_Hit, GSvector3{ 0.0f,1.0f,0.0f }, GSvector3{ 0.0f,0.0f,0.0f }, GSvector3{ 0.7f,0.7f,0.7f });
        if (health_ <= 0) {
            // �c��̗̑͂��Ȃ���΃_�E����ԂɑJ��
            gsPlaySE(SE_EnemyDie);
            change_state(State::Down, MotionDown, false);
        }
        else {
            if (state_ != State::Damage) {
                // �e�̐i�s�����Ƀm�b�N�o�b�N����ړ��ʂ����߂�
                //velocity_ = other.velocity().getNormalized() * 0.5f;
                // �_���[�W��ԂɑJ��
                gsPlaySE(SE_EnemyDamage);
                if (gsRand(1, 100) < 25) change_state(State::Damage, MotionDamage, false);
                return;
            }
        }
    }

    if (other.tag() == "PlayerAttackTag" && !is_hit_) {
        // �̗͂����炷
        health_ -= other.damage();
        play_effect(Effect_Hit, GSvector3{ 0.0f,1.0f,0.0f }, GSvector3{ 0.0f,0.0f,0.0f }, GSvector3{ 0.7f,0.7f,0.7f });
        is_hit_ = true;
        hit_timer_ = 0;
        if (health_ <= 0) {
            // �c��̗̑͂��Ȃ���΃_�E����ԂɑJ��
            gsPlaySE(SE_EnemyDie);
            change_state(State::Down, MotionDown, false);
        }
        else{
            if (state_ != State::Damage) {
                // �^�[�Q�b�g�����̃x�N�g�������߂�
                GSvector3 to_target = other.transform().position() - transform().position();
                // �������͖����ɂ���
                to_target.y = 0.0f;
                // �^�[�Q�b�g�����Ƌt�����Ƀm�b�N�o�b�N����ړ��ʂ����߂�
                velocity_ = -to_target.getNormalized() * 0.4f;
                // �_���[�W��ԂɑJ�ڂ���
                gsPlaySE(SE_EnemyDamage);
                change_state(State::Damage, MotionDamage, false);
                return;
            }
        }
    }

    // �v���[���[�܂��͓G�ɏՓ˂�����
    if (other.tag() == "PlayerTag" || other.tag() == "EnemyTag") {
        collide_actor(other);
    }
}

void EnemyMelee::update_state(float delta_time){
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

void EnemyMelee::change_state(State state, GSuint motion, bool loop){
    upper_motion_ = motion;
    upper_motion_loop_ = loop;
    state_ = state;
    state_timer_ = 0.0f;
}

void EnemyMelee::idle(float delta_time){
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
    if (is_walk()) {
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

void EnemyMelee::walk(float delta_time){
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

void EnemyMelee::turn(float delta_time){
    if (state_timer_ >= mesh_.motion_end_time() * 0.8f) {
        idle(delta_time);
    }
    else {
        float angle = (target_signed_angle() >= 0.0f) ? TurnAngle : -TurnAngle;
        transform_.rotate(0.0f, angle * delta_time, 0.0f);
    }
}

void EnemyMelee::attack(float delta_time){
    attack_timer_ = 0;

    if (state_timer_ >= mesh_.motion_end_time()) {
        attacked_ = true;
        attack_timer_ = 0;
        idle(delta_time);
    }
}

void EnemyMelee::damage(float delta_time){
    if (state_timer_ < mesh_.motion_end_time()) {
        // �m�b�N�o�b�N����
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

void EnemyMelee::down(float delta_time) {
    enable_collider_ = false;

    if (state_timer_ >= mesh_.motion_end_time()) {
        if (gsRand(1, 100) < 20) world_->add_actor(new HealthPotion(world_, transform_.position()));
        world_->add_score(100);
        // �_�E�����[�V�������I�������玀�S
        die();
    }
}

bool EnemyMelee::is_turn() const{
    // �U��������������O���������̃x�N�g���ƃ^�[�Q�b�g�����̃x�N�g���̊p�x����90.0�x�ȏォ�H
    return (target_distance() <= TurnDistance) && (target_angle() >= 90.0f);
}

bool EnemyMelee::is_attack() const{
    // �U�����������O���������̃x�N�g���ƃ^�[�Q�b�g�����̃x�N�g���̊p�x����20.0�x�ȉ����H
    return (target_distance() <= AttackDistance) && (target_angle() <= 20.0f);
}

bool EnemyMelee::is_walk() const{
    // �ړ����������O�����ƑO���������̃x�N�g���ƃ^�[�Q�b�g�����̃x�N�g���̊p�x����100.0�x�ȉ����H
    return (target_distance() <= WalkDistance) && (target_angle() <= 100.0f);
}

float EnemyMelee::target_signed_angle() const{
    // �^�[�Q�b�g�����Ȃ����0��Ԃ�
    if (player_ == nullptr) return 0.0f;
    // �^�[�Q�b�g�����̃x�N�g�������߂�
    GSvector3 to_target = player_->transform().position() - transform_.position();
    // �O���������̃x�N�g�����擾
    GSvector3 forward = transform_.forward();
    // �x�N�g����y�����𖳌��ɂ���
    forward.y = 0.0f;
    to_target.y = 0.0f;
    // �O���������̃x�N�g���ƃ^�[�Q�b�g�����̃x�N�g���̊p�x�������߂�
    return GSvector3::signedAngle(forward, to_target);
}

float EnemyMelee::target_angle() const{
    return std::abs(target_signed_angle());
}

float EnemyMelee::target_distance() const{
    // �^�[�Q�b�g�����Ȃ���΍ő勗����Ԃ�
        if (player_ == nullptr) return FLT_MAX; // float�^�̍ő�l
    // �^�[�Q�b�g�Ƃ̋������v�Z����
    return GSvector3::distance(player_->transform().position(), transform_.position());
}

void EnemyMelee::collide_field(){
    // �ǂƂ̏Փ˔���i���̂Ƃ̔���j
    GSvector3 center; // �Փˌ�̋��̂̒��S���W
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
        // ���������_����y���W�̂ݕ␳����
        position.y = intersect.y;
        // ���W��ύX����
        transform_.position(position);
        // �d�͂�����������
        velocity_.y = 0.0f;
    }
}

void EnemyMelee::collide_actor(Actor& other){
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

void EnemyMelee::generate_attack_collider(){
    gsPlaySE(SE_EnemyMelee);
    // �U��������o��������ꏊ�̋���
    const float AttackColliderDistance{ 1.0f };
    // �U������̔��a
    const float AttackColliderRadius{ 0.4f };
    // �U��������o���ꏊ�̍���
    const float AttackColliderHeight{ 1.0f };
    // �U�����肪�L���ɂȂ�܂ł̒x������
    const float AttackColliderDelay{ 0.0f };
    // �U������̎���
    const float AttackColliderLifeSpan{ 15.0f };
    const int AttackColliderDamage{ 150 };
    // �Փ˔�����o����������W�����߂�i�O���̈ʒu�j
    GSvector3 position = transform_.position() + transform_.forward() * AttackColliderDistance;
    // �����̕␳�i��������̍����j
    position.y += AttackColliderHeight;
    // �Փ˔���p�̋����쐬
    BoundingSphere collider{ AttackColliderRadius, position };
    // �Փ˔�����o��������
    world_->add_actor(new AttackCollider{ world_, collider,
    "EnemyAttackTag", "EnemyAttack", tag_, AttackColliderDamage, AttackColliderLifeSpan, AttackColliderDelay, transform_.rotation()});
}

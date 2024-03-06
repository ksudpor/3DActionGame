#include "Actor.h"
#include <GSEffect.h>

void Actor::update(float delta_time){
}

void Actor::late_update(float delta_time){
}

void Actor::draw() const{
}

void Actor::draw_transparent() const{
}

void Actor::draw_gui() const{
}

void Actor::react(Actor& other){
}

void Actor::handle_message(const std::string& message, void* param){
}

void Actor::collide(Actor& other){
    // �ǂ���̃A�N�^�[���Փ˔��肪�L�����H
    if (enable_collider_ && other.enable_collider_) {
        // �Փ˔��������
        if (is_collide(other)) {
            // �Փ˂����ꍇ�́A���݂��ɏՓ˃��A�N�V����������
            react(other);
            other.react(*this);
        }
    }
}

void Actor::die(){
    dead_ = true;
}

bool Actor::is_collide(const Actor& other) const{
    return collider().intersects(other.collider());
}

bool Actor::is_dead() const{
    return dead_;
}

bool Actor::game_clear() const{
    return false;
}

bool Actor::game_over() const {
    return false;
}

const std::string& Actor::name() const{
    return name_;
}

const std::string& Actor::tag() const{
    return tag_;
}

const GStransform& Actor::transform() const{
    return transform_;
}

GStransform& Actor::transform(){
    return transform_;
}

GSvector3 Actor::velocity() const{
    return velocity_;
}

BoundingSphere Actor::collider() const{
    return collider_.transform(transform_.localToWorldMatrix());
}

void Actor::set_visible(bool visible){
    visible_ = visible;
}

bool Actor::get_visible() const{
    return visible_;
}

void Actor::set_targeted(bool targeted){
    targeted_ = targeted;
}

bool Actor::get_targeted() const{
    return targeted_;
}

const int& Actor::damage() const{
    return damage_;
}

void Actor::play_effect(GSuint id, const GSvector3& localPosition, const GSvector3& localRotation, const GSvector3& localScale){
    // �w�肳�ꂽTranslate, Rotation, Scale�̍s����쐬����
    GSmatrix4 localMat = GSmatrix4::TRS(localPosition, GSquaternion::euler(localRotation), localScale);
    // ���[���h��Ԃɕϊ�����
    GSmatrix4 worldMat = localMat * transform_.localToWorldMatrix();
    // �G�t�F�N�g���Đ�����
    gsPlayEffectEx(id, &worldMat);
}

const GSmatrix4& Actor::get_world_matrix(const GSvector3& localPosition, const GSvector3& localRotation, const GSvector3& localScale) const{
    GSmatrix4 localMat = GSmatrix4::TRS(localPosition, GSquaternion::euler(localRotation), localScale);
    GSmatrix4 worldMat = localMat * transform_.localToWorldMatrix();
    return worldMat;
}

#include "Weapon.h"

void Weapon::start() {
}

void Weapon::update(float delta_time){
}

void Weapon::guard(){
}

void Weapon::skill(){
}

void Weapon::skill_event1(){
}

void Weapon::skill_event2(){
}

void Weapon::skill_event3(){
}

void Weapon::skill_event4(){
}

void Weapon::event_combo_attack1(){
}

void Weapon::event_combo_attack2(){
}

void Weapon::event_combo_attack3(){
}

void Weapon::event_fire(){
}

const GSint& Weapon::motion_walk_front() const{
    return motion_walk_front_;
}

const GSint& Weapon::motion_walk_back() const{
    return motion_walk_back_;
}

const GSint& Weapon::motion_run() const{
    return motion_run_;
}

const GSint& Weapon::motion_jump() const{
    return motion_jump_;
}

const GSint& Weapon::motion_damage() const{
    return motion_damage_;
}

const GSint& Weapon::motion_die() const{
    return motion_die_;
}

const GSint& Weapon::motion_equip() const{
    return motion_equip_;
}

const GSint& Weapon::motion_idle() const{
    return motion_idle_;
}

const GSint& Weapon::motion_guard() const{
    return motion_guard_;
}

const GSint& Weapon::motion_attack1() const{
    return motion_attack1_;
}

const GSint& Weapon::motion_attack2() const{
    return motion_attack2_;
}

const GSint& Weapon::motion_attack3() const{
    return motion_attack3_;
}

const GSint& Weapon::motion_fire() const{
    return motion_fire_;
}

const GSint& Weapon::motion_move_fire() const{
    return motion_move_fire_;
}

const GSint& Weapon::motion_skill() const{
    return motion_skill_;
}

const GSint& Weapon::bone_position_right_hand() const{
    return bone_position_right_hand_;
}

const GSint& Weapon::bone_position_left_hand() const{
    return bone_position_left_hand_;
}

const GSuint& Weapon::mesh_number_right_hand() const{
    return mesh_number_right_hand_;
}

const GSuint& Weapon::mesh_number_left_hand() const{
    return mesh_number_left_hand_;
}

const GSfloat& Weapon::event_time_attack1() const{
    return event_time_attack1_;
}

const GSfloat& Weapon::event_time_attack2() const{
    return event_time_attack2_;
}

const GSfloat& Weapon::event_time_attack3() const {
    return event_time_attack3_;
}

const GSfloat& Weapon::event_time_fire() const{
    return event_time_fire_;
}

const float& Weapon::attack_motion_rate() const{
    return attack_motion_rate_;
}

const float& Weapon::damage_motion_rate() const{
    return damage_motion_rate_;
}

const float& Weapon::guard_sp() const{
    return guard_sp_;
}

const float& Weapon::evade_sp() const{
    return evade_sp_;
}

const int& Weapon::defend() const{
    return defend_;
}

const int& Weapon::damage() const {
    return damage_;
}

const bool& Weapon::has_combo() const{
    return has_combo_;
}

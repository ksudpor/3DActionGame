#include "AnimatedMesh.h"
#include <cmath>
#include <algorithm>

const GSfloat LerpTime{ 10.0f };    // 補間フレーム数

AnimatedMesh::AnimatedMesh(GSuint mesh, GSuint skeleton, GSuint animation, GSuint motion, bool loop, GSuint layer_count, GSuint bone_count) :
    mesh_{ mesh },
    skeleton_{ skeleton },
    animation_layers_(layer_count, Animation{ animation, motion, loop }),
    animation_layer_indices_(bone_count, 0),
    local_bone_matrices_(bone_count, GS_MATRIX4_IDENTITY),
    bone_matrices_(bone_count, GS_MATRIX4_IDENTITY),
    transform_{ GS_MATRIX4_IDENTITY } {
}

void AnimatedMesh::update(float delta_time){
    for (int i = 0; i < animation_layers_.size(); ++i) {
        // 更新前のタイマー値を取っておく
        GSfloat prev_timer = animation_layers_[i].current_motion_time();
        // アニメーションレイヤーを更新
        animation_layers_[i].update(delta_time);
        // モーションがループしたかどうか。
        // 現在の時間が1フレーム前の時間より小さい場合は、ループしたと判断できる。
        bool looped = animation_layers_[i].current_motion_time() < prev_timer;
        // 全イベントをチェックし、必要であればイベントを発行する
        for (const auto& event : events_) {
            if (i == event->layer_) {
                // 現在のモーションがイベント対象のモーションでなければ、何もしない
                if (event->motion_ != animation_layers_[i].motion()) continue;

                if (looped) {
                    if (prev_timer < event->time_ || event->time_ <= animation_layers_[i].current_motion_time()) {
                        event->callback_(); // コールバック処理を呼び出す
                    }
                }
                else {
                    if (prev_timer < event->time_ && event->time_ <= animation_layers_[i].current_motion_time()) {
                        event->callback_(); // コールバック処理を呼び出す
                    }
                }
            }
        }
    }
}

void AnimatedMesh::draw() const{
    // スケルトンの自動計算を無効にする
    gsDisable(GS_CALC_SKELETON);
    // スケルトンのバインド
    gsBindSkeleton(skeleton_);
    // ボーンの変換行列を設定
    gsSetMatrixSkeleton(bone_matrices_.data());
    // メッシュの描画
    glPushMatrix();
    glMultMatrixf(transform_);
    gsDrawMesh(mesh_);
    glPopMatrix();
    // スケルトンの自動計算を有効にする
    gsEnable(GS_CALC_SKELETON);
}

void AnimatedMesh::change_motion(GSuint motion, bool loop){
    for (auto& layer : animation_layers_) {
        layer.change_motion(motion, loop);
    }
}

void AnimatedMesh::change_motion(GSuint layer, GSuint motion, bool loop){
    animation_layers_[layer].change_motion(motion, loop);
}

void AnimatedMesh::set_layer_indices(GSuint layer_index, const GSuint layer_bones[], GSuint layer_bones_count){
    for (GSuint i = 0; i < layer_bones_count; ++i) {
        animation_layer_indices_[layer_bones[i]] = layer_index;
    }
}

void AnimatedMesh::transform(const GSmatrix4& matrix){
    // 座標変換行列を更新
    transform_ = matrix;
    // 各レイヤーのからローカル変換行列を取得
    for (GSuint i = 0; i < gsGetSkeletonNumBones(skeleton_); ++i) {
        local_bone_matrices_[i] = animation_layers_[animation_layer_indices_[i]].local_bone_matrix(i);
    }
    // ボーンの変換行列を計算
    gsBindSkeleton(skeleton_);
    gsCalculateSkeleton(NULL, local_bone_matrices_.data(), bone_matrices_.data());
}

bool AnimatedMesh::is_end_motion(GSuint layer) const{
    return animation_layers_[layer].is_motion_end();
}

float AnimatedMesh::current_motion_time(GSuint layer) const{
    return animation_layers_[layer].current_motion_time();
}

void AnimatedMesh::current_motion_time(float time, GSuint layer){
    animation_layers_[layer].current_motion_time(time);
}

float AnimatedMesh::motion_end_time(GSuint layer) const{
    return animation_layers_[layer].motion_end_time();
}

GSmatrix4 AnimatedMesh::bone_matrices(int bone_no) const{
    return bone_matrices_[bone_no] * transform_;
}

void AnimatedMesh::add_event(GSuint layer, GSuint motion, GSfloat time, std::function<void()> callback) {
    events_.push_back(std::make_unique<AnimationEvent>(layer, motion, time, callback));
}

AnimatedMesh::Animation::Animation(GSuint animation, GSuint motion, bool loop) :
    animation_{ animation },
    motion_{ motion },
    motion_timer_{ 0.0f },
    motion_loop_{ loop },
    prev_motion_{ motion },
    prev_motion_timer_{ 0.0f },
    lerp_timer_{ 0.0f } {
}

void AnimatedMesh::Animation::update(float delta_time) {
    // アニメーションタイマの更新
    motion_timer_ += delta_time;
    if (motion_loop_) {
        // モーションタイマをループさせる
        motion_timer_ = std::fmod(motion_timer_, motion_end_time());
    }
    else {
        // モーションタイマをクランプする
        motion_timer_ = std::min(motion_timer_, motion_end_time() - 1.0f);
    }
    // 補間中タイマの更新
    lerp_timer_ = std::min(lerp_timer_ + delta_time, LerpTime);
}

void AnimatedMesh::Animation::change_motion(GSuint motion, bool loop) {
    // 現在と同じモーションの場合は何もしない
    if (motion_ == motion) return;
    // 補間中の前半は、前回のモーションを更新をしないようにする
    if (lerp_timer_ > (LerpTime * 0.5f)) {
        // 前回のモーション番号とモーション時間を保存
        prev_motion_ = motion_;
        prev_motion_timer_ = motion_timer_;
        // 補間中タイマの初期化 
        lerp_timer_ = 0.0f;
    }
    // モーションの更新
    motion_ = motion;
    // モーションタイマの初期化
    motion_timer_ = 0.0f;
    // ループフラグの設定
    motion_loop_ = loop;
}

float AnimatedMesh::Animation::motion_end_time() const {
    return gsGetEndAnimationTime(animation_, motion_);
}

float AnimatedMesh::Animation::current_motion_time() const {
    return motion_timer_;
}

void AnimatedMesh::Animation::current_motion_time(float time) {
    motion_timer_ = time;
}

bool AnimatedMesh::Animation::is_motion_end() const {
    // ループモーションは終了しない
    if (motion_loop_) return false;
    // 終了しているか？
    return motion_timer_ >= (motion_end_time() - 1.0f);
}

GSmatrix4 AnimatedMesh::Animation::local_bone_matrix(GSuint bone_no) const {
    GSmatrix4 result;
    // 補間付きアニメーション変換行列を計算
    gsCalculateBoneAnimationLerp(
        animation_, prev_motion_, prev_motion_timer_,
        animation_, motion_, motion_timer_,
        bone_no,
        lerp_timer_ / LerpTime,
        &result
    );
    return result;
}

GSuint AnimatedMesh::Animation::bone_count() const {
    return gsGetAnimationNumBones(animation_, motion_);
}

const GSuint& AnimatedMesh::Animation::animation() const
{
    return animation_;
}

const GSuint& AnimatedMesh::Animation::motion() const{
    return motion_;
}

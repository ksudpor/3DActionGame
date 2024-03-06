#ifndef ACTOR_H_
#define ACTOR_H_

#include <gslib.h>
#include <GStransform.h>
#include <string>
#include "BoundingSphere.h"

class IWorld;

// アクタークラス
class Actor {
public:
	Actor() = default;
	virtual ~Actor() = default;
	virtual void update(float delta_time);
	virtual void late_update(float delta_time);
	virtual void draw() const;
	virtual void draw_transparent() const;
	virtual void draw_gui() const;

	// 衝突リアクション
	virtual void react(Actor& other);
	// メッセージ処理
	virtual void handle_message(const std::string& message, void* param);	
	// 衝突判定
	void collide(Actor& other);	
	// 死亡する
	void die();	
	
	//　フラグ関数
	bool is_collide(const Actor& other) const;
	bool is_dead() const;
	virtual bool game_clear() const;
	virtual bool game_over() const;

	//　取得関数
	const std::string& name() const;
	const std::string& tag() const;
	const GStransform& transform() const;
	GStransform& transform();
	GSvector3 velocity() const;
	BoundingSphere collider() const;

	void set_visible(bool visible);
	bool get_visible() const;
	void set_targeted(bool targeted);
	bool get_targeted() const;

	const int& damage() const;

	// エフェクシアのエフェクトを再生する
	void play_effect(GSuint id, const GSvector3& localPosition, const GSvector3& localRotation, const GSvector3& localScale);
	const GSmatrix4& get_world_matrix(const GSvector3& localPosition, const GSvector3& localRotation, const GSvector3& localScale) const;

	// コピー禁止
	Actor(const Actor& other) = delete;
	Actor& operator = (const Actor& other) = delete;
protected:
	IWorld* world_{ nullptr };					// ワールド
	std::string tag_;							// タグ名
	std::string name_;							// 名前
	GStransform transform_;						// トランスフォーム
	GSvector3 velocity_{ 0.0f, 0.0f, 0.0f };	// 移動量
	bool enable_collider_{ true };				// 衝突判定が有効か？
	BoundingSphere collider_;					// 衝突判定
	bool dead_{ false };						// 死亡フラグ
	int damage_;

	bool visible_{ false };
	bool targeted_{ false };
};


#endif

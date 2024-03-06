#ifndef PLAYER_H_
#define PLAYER_H_

#include "Actor.h"
#include "AnimatedMesh.h"
#include "Weapon.h"
#include <vector>
#include "Line.h"
#include "GameProgress.h"

// プレーヤークラス
class Player : public Actor {
public:
	//状態
	enum class State {
		Equip,
		Walk,
		Run,
		Jump,
		Evade,
		Damage,
		Fire,
		Attack1,
		Attack2,
		Attack3,
		Guard,
		Skill,
		Down
	};

public:
	Player(IWorld* world = nullptr, const GSvector3& position = GSvector3{ 0.0f, 0.0f, 0.0f });
	void update(float delta_time) override;
	void draw() const override;
	void draw_gui() const override;
	virtual void react(Actor& other) override;

private:
	// 状態の更新
	void update_state(float delta_time);
	// 状態の変更
	void change_state(State state, GSuint motion, bool loop = true);
	void change_state(State state, GSuint upper_motion, bool upper_loop, GSuint lower_motion, bool lower_loop);
	void change_weapon();
	Weapon* initialize_weapon(GameProgress::Weapon enum_weapon, IWorld* world);

	void equip(float delta_time);
	void walk(float delta_time);
	void run(float delta_time);
	void jump(float delta_time);
	void evade(float delta_time);
	void damage(float delta_time);
	void fire(float delta_time);
	void attack1(float delta_time);
	void attack2(float delta_time);
	void attack3(float delta_time);
	void guard(float delta_time);
	void skill(float delta_time);
	void down(float delta_time);

	void move(float delta_time, float speed, State state, GSint motion_default, GSint motion_move, 
		bool layer_motion, bool enable_target, bool can_evade, bool can_attack = false);
	void input_handle(float delta_time);
	void attack_handle(float delta_time);
	void event_handle(Weapon* weapon);

	// 武器の描画
	void draw_weapon() const;

	// フィールドとの衝突処理
	void collide_field();
	// アクターとの衝突処理
	void collide_actor(Actor& other);

	void target_enemy(float delta_time);
	float target_signed_angle() const;

private:
	AnimatedMesh mesh_;			// アニメーションメッシュ
	GSuint upper_motion_;		// モーション番号
	bool upper_motion_loop_;	// モーションのループ指定
	GSuint lower_motion_;		
	bool lower_motion_loop_;
	State state_;				// 状態
	float state_timer_;			// 状態タイマ
	Weapon* weapon_;
	Weapon* weapon_primary_;
	Weapon* weapon_secondary_;
	bool is_evading_;
	

	bool is_targeting_;				//ロックオンしているか
	std::vector<Actor*> enemies_;	//敵のリスト
	Line target_line_;				//プレイヤーとロック対象の間の線
	Actor* targeted_enemy_;			//現在ロックオンされている敵
	float untarget_timer_;			//ロック解除のためにキーを長押しする時間
	//どっちの敵がロックオンされているかとわかるように、エフェクトをつける
	GSint target_effect_;			

	GSint hit_effect_;
	float combo_timer_;
	int combo_count_;

	int health_;
	float stamina_;
	bool sp_used_;
	float sp_regen_timer_;

	// Bone Debug
	//int bone = 16;
};

#endif

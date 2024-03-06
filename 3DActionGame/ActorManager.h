#ifndef ACTOR_MANAGER_H_
#define ACTOR_MANAGER_H_

#include <list>
#include <vector>
#include <string>

class Actor;

class ActorManager {
public:
	ActorManager() = default;
	~ActorManager();
	void add(Actor* actor);
	void update(float delta_time);
	void late_update(float delta_time);
	void draw() const;
	void draw_transparent() const;
	void draw_gui() const;

	// アクターの衝突判定
	void collide();
	// 死亡しているアクターの削除
	void remove();
	// アクターの検索
	Actor* find(const std::string& name) const;
	// 指定したタグ名を持つアクターの検索
	std::vector<Actor*> find_with_tag(const std::string& tag) const;
	// アクター数を返す
	int count() const;
	// 指定したタグ名を持つアクター数を返す
	int count_with_tag(const std::string& tag) const;
	// メッセージの送信
	void send_message(const std::string& message, void* param);
	// 消去
	void clear();

	// コピー禁止
	ActorManager(const ActorManager& other) = delete;
	ActorManager& operator = (const ActorManager& other) = delete;

private:
	std::list<Actor*> actors_;	// アクターリスト
};

#endif

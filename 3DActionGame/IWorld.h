#ifndef IWORLD_H_
#define IWORLD_H_

#include <string>
#include <vector>

class Actor;
class Field;

// ワールド抽象インターフェース
class IWorld {
public:
	virtual ~IWorld() = default;
	// 追加
	virtual void add_actor(Actor* actor) = 0;
	virtual void add_score(int score) = 0;

	// 検索
	virtual Actor* find_actor(const std::string& name) const = 0;
	virtual std::vector<Actor*> find_actor_with_tag(const std::string& tag) const = 0;
	
	// 数える
	virtual int count_actor() const = 0;
	virtual int count_actor_with_tag(const std::string& tag) const = 0;
	
	// メッセージの送信
	virtual void send_message(const std::string& message, void* param = nullptr) = 0;
	
	//取得
	virtual Field* field() = 0;
	virtual Actor* camera() = 0;
	virtual Actor* light() = 0;
	virtual float time() = 0;
	virtual float spent_time() = 0;
	virtual int score() = 0;
};

#endif

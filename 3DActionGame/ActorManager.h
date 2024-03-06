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

	// �A�N�^�[�̏Փ˔���
	void collide();
	// ���S���Ă���A�N�^�[�̍폜
	void remove();
	// �A�N�^�[�̌���
	Actor* find(const std::string& name) const;
	// �w�肵���^�O�������A�N�^�[�̌���
	std::vector<Actor*> find_with_tag(const std::string& tag) const;
	// �A�N�^�[����Ԃ�
	int count() const;
	// �w�肵���^�O�������A�N�^�[����Ԃ�
	int count_with_tag(const std::string& tag) const;
	// ���b�Z�[�W�̑��M
	void send_message(const std::string& message, void* param);
	// ����
	void clear();

	// �R�s�[�֎~
	ActorManager(const ActorManager& other) = delete;
	ActorManager& operator = (const ActorManager& other) = delete;

private:
	std::list<Actor*> actors_;	// �A�N�^�[���X�g
};

#endif

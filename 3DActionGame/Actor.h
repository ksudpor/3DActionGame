#ifndef ACTOR_H_
#define ACTOR_H_

#include <gslib.h>
#include <GStransform.h>
#include <string>
#include "BoundingSphere.h"

class IWorld;

// �A�N�^�[�N���X
class Actor {
public:
	Actor() = default;
	virtual ~Actor() = default;
	virtual void update(float delta_time);
	virtual void late_update(float delta_time);
	virtual void draw() const;
	virtual void draw_transparent() const;
	virtual void draw_gui() const;

	// �Փ˃��A�N�V����
	virtual void react(Actor& other);
	// ���b�Z�[�W����
	virtual void handle_message(const std::string& message, void* param);	
	// �Փ˔���
	void collide(Actor& other);	
	// ���S����
	void die();	
	
	//�@�t���O�֐�
	bool is_collide(const Actor& other) const;
	bool is_dead() const;
	virtual bool game_clear() const;
	virtual bool game_over() const;

	//�@�擾�֐�
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

	// �G�t�F�N�V�A�̃G�t�F�N�g���Đ�����
	void play_effect(GSuint id, const GSvector3& localPosition, const GSvector3& localRotation, const GSvector3& localScale);
	const GSmatrix4& get_world_matrix(const GSvector3& localPosition, const GSvector3& localRotation, const GSvector3& localScale) const;

	// �R�s�[�֎~
	Actor(const Actor& other) = delete;
	Actor& operator = (const Actor& other) = delete;
protected:
	IWorld* world_{ nullptr };					// ���[���h
	std::string tag_;							// �^�O��
	std::string name_;							// ���O
	GStransform transform_;						// �g�����X�t�H�[��
	GSvector3 velocity_{ 0.0f, 0.0f, 0.0f };	// �ړ���
	bool enable_collider_{ true };				// �Փ˔��肪�L�����H
	BoundingSphere collider_;					// �Փ˔���
	bool dead_{ false };						// ���S�t���O
	int damage_;

	bool visible_{ false };
	bool targeted_{ false };
};


#endif

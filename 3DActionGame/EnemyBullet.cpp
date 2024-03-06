#include "EnemyBullet.h"
#include "IWorld.h"
#include "Field.h"
#include "Line.h"
#include "Assets.h"
#include <GSeffect.h>

// ����
const float LifeSpanTime{ 60.0f };

EnemyBullet::EnemyBullet(IWorld* world, const GSvector3& position, const GSquaternion& rotation, const GSvector3& velocity, int damage){
	// ���[���h��ݒ�
	world_ = world;
	// �^�O��
	tag_ = "EnemyBulletTag";
	// �A�N�^�[��
	name_ = "EnemyBullet";
	// �ړ��ʂ̏�����
	velocity_ = velocity;
	// �Փ˔���p�̋��̂�ݒ�
	collider_ = BoundingSphere{ 0.2f };
	// ���W�̏�����
	transform_.position(position);
	transform_.rotation(rotation);
	// ����
	lifespan_timer_ = LifeSpanTime;
	// �G�t�F�N�g���Đ��i�����j����
	effect_handle_ = gsPlayEffect(Effect_BigArrowDark, &position);
	damage_ = damage;
}

void EnemyBullet::update(float delta_time){
	// �������s�����玀�S
	if (lifespan_timer_ <= 0.0f) {
		die();
		// �G�t�F�N�g�̒�~�i�폜�j
		gsStopEffect(effect_handle_);
		return;
	}
	// �����̍X�V
	lifespan_timer_ -= delta_time;
	// �t�B�[���h�Ƃ̏Փ˔���
	Line line;
	line.start = transform_.position();
	line.end = transform_.position() + velocity_ * delta_time;
	GSvector3 intersect;
	if (world_->field()->collide(line, &intersect)) {
		// ��_�̍��W�ɕ␳
		transform_.position(intersect);
		// �t�B�[���h�ɏՓ˂����玀�S
		velocity_ = GSvector3{ 0.0f, 0.0f, 0.0f };
		return;
	}
	// �ړ�����i���[���h���W�n��j
	transform_.translate(velocity_ * delta_time, GStransform::Space::World);
}

void EnemyBullet::draw() const{
	// �G�t�F�N�g�Ɏ��g�̃��[���h�ϊ��s���ݒ�
	GSmatrix4 localMat = GSmatrix4::TRS(GSvector3{0.0f,0.0f,0.0f}, GSquaternion::euler(GSvector3(0.0f, 0.0f, 0.0f)), GSvector3{ 0.5f, 0.5f, 0.5f });
	GSmatrix4 worldMat = localMat * transform_.localToWorldMatrix();
	gsSetEffectMatrix(effect_handle_, &worldMat); // ���[���h�ϊ��s���ݒ�
	// �����ɂ���āA���񂾂񓧖��ɂ��Ă����i�A���t�@�l��ϊ�������j
	GScolor start_color{ 1.0f, 1.0f, 1.0f, 1.0f };
	GScolor end_color{ 1.0f, 1.0f, 1.0f, 0.0f };
	GScolor color = GScolor::lerp(start_color, end_color, lifespan_timer_ / LifeSpanTime);
	gsSetEffectColor(effect_handle_, &color); // �J���[��ݒ�

	// �f�o�b�O�\��
	//collider().draw();
}

void EnemyBullet::react(Actor& other){
	if (other.tag() == tag() || other.tag() == "EnemyTag" || other.tag() == "PlayerBulletTag") return;
	if (other.tag() == "PlayerTag") {
		die();
		gsStopEffect(effect_handle_);
		return;
	}
	// �Փ˂�����ړ����Ȃ��悤�ɂ���
	velocity_ = GSvector3{ 0.0f, 0.0f, 0.0f };
	// ���G��Ԃɂ���
	enable_collider_ = false;
}

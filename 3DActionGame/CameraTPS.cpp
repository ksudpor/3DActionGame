#include "CameraTPS.h"
#include "IWorld.h"
#include "Field.h"
#include "Line.h"

const GSvector3 PlayerOffset{ 0.0f, 3.0f, -5.0f };
const GSvector3 ReferencePointOffset{ 0.0f, 1.7f, 0.0f };

CameraTPS::CameraTPS(IWorld* world, const GSvector3& position, const GSvector3& at){
	world_ = world;
	tag_ = "CameraTag";
	name_ = "Camera";
	transform_.position(position);
	transform_.lookAt(at);
}

void CameraTPS::update(float delta_time){
	Actor* player = world_->find_actor("Player");
	if (player == nullptr) return;

	GSvector3 position = PlayerOffset * player->transform().localToWorldMatrix();
	GSvector3 at = player->transform().position() + ReferencePointOffset;
	
	// �t�B�[���h�Ƃ̏Փ˔���
	Line line{ at, position };
	GSvector3 intersect;
	if (world_->field()->collide(line, &intersect)) {
		position = intersect;
	}

	// �X���[�X�_���v�ɂ�銊�炩�ȕ��
	const float SmoothTime{ 12.0f }; // ��ԃt���[����
	const float MaxSpeed{ 1.0f }; // �ړ��X�s�[�h�̍ő�l
	position = GSvector3::smoothDamp(transform_.position(), position, velocity_,
		SmoothTime, MaxSpeed, delta_time);
	
	transform_.position(position);
	transform_.lookAt(at);

}

void CameraTPS::draw() const{
	// ���_�̈ʒu
	GSvector3 eye = transform_.position();
	// �����_�̈ʒu
	GSvector3 at = eye + transform_.forward();
	// �J�����̐ݒ�
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		eye.x, eye.y, eye.z, // ���_�̈ʒu
		at.x, at.y, at.z, // �����_�̈ʒu
		0.0f, 1.0f, 0.0f // ���_�̏����
	);
}

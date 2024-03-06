#include "CameraRotateAround.h"
#include "IWorld.h"
#include "Field.h"
#include "Line.h"


// �v���[���[����̑��Έʒu�iz���W�̂݁j
const GSvector3 PlayerOffset{ 0.0f, 0.0f, -5.0f };
// �J�����̒����_�̕␳�l
const GSvector3 ReferencePointOffset{ 0.0f, 1.8f, 0.0f };

CameraRotateAround::CameraRotateAround(IWorld* world, const GSvector3& position, const GSvector3& at){
	world_ = world;
	tag_ = "CameraTag";
	name_ = "Camera";
	transform_.position(position);
	transform_.lookAt(at);

	// ��������̉�]�p�x�̏�����
	pitch_ = (at - position).getPitch();
	// ��������̉�]�p�x�̏�����
	yaw_ = (at - position).getYaw();
}

void CameraRotateAround::update(float delta_time){
	Actor* player = world_->find_actor("Player");
	if (player == nullptr) return;

	//// y���܂��ɃJ��������]������
	//if (gsGetKeyState(GKEY_LEFT)) yaw_ += 1.0f * delta_time;
	//if (gsGetKeyState(GKEY_RIGHT)) yaw_ -= 1.0f * delta_time;
	//// x���܂��ɃJ��������]������
	//if (gsGetKeyState(GKEY_UP)) pitch_ += 1.0f * delta_time;
	//if (gsGetKeyState(GKEY_DOWN)) pitch_ -= 1.0f * delta_time;
	int mx, my;
	gsGetMouseVelocity(&mx, &my, NULL);
	const float MouseSensitivity{ 0.2f };
	yaw_ -= mx * delta_time * MouseSensitivity;
	pitch_ += my * delta_time * MouseSensitivity;

	// x���܂��̉�]�p�x�̐���������
	pitch_ = CLAMP(pitch_, -10.0f, 30.0f);

	GSvector3 at = player->transform().position() + ReferencePointOffset;
	GSvector3 position = at + GSquaternion::euler(pitch_, yaw_, 0.0f) * PlayerOffset;
	
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

	gsUpdateFrustum();
}

void CameraRotateAround::draw() const{
	GSvector3 eye = transform_.position();
	GSvector3 at = eye + transform_.forward();
	GSvector3 up = transform_.up();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		eye.x, eye.y, eye.z, // ���_�̈ʒu
		at.x, at.y, at.z, // �����_�̈ʒu
		up.x, up.y, up.z // ���_�̏����
	);
}

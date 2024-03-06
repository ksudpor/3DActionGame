#include "Camera.h"
#include "IWorld.h"

// �v���[���[����̑��΍��W
const GSvector3 PlayerOffset{ 0.0f, 3.0f, -5.0f };
// �J�����̒����_�̕␳�l
const GSvector3 ReferencePointOffset{ 0.0f, 1.7f, 0.0f };

Camera::Camera(IWorld* world){
	world_ = world;
	name_ = "Camera";
	tag_ = "CameraTag";
}

void Camera::draw() const{
	// �v���[���[������
	Actor* player = world_->find_actor("Player");
	if (player == nullptr) return;
	// �J�����̈ʒu�����߂�
	GSvector3 eye = PlayerOffset * player->transform().localToWorldMatrix();
	// �����_�̈ʒu�����߂�
	GSvector3 at = player->transform().position() + ReferencePointOffset;
	// OpenGL�̃J�����̐ݒ�
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		eye.x, eye.y, eye.z, // ���_�̈ʒu
		at.x, at.y, at.z, // �����_�̈ʒu
		0.0f, 1.0f, 0.0f // ���_�̏����
	);

}

#ifndef BOUNDING_RECTANGLE_H_
#define BOUNDING_RECTANGLE_H_

#include <gslib.h>	//GSvector2

// ��`�N���X
class BoundingRectangle {
public:
	// �R���X�g���N�^
	BoundingRectangle() = default;
	BoundingRectangle(float left, float top, float right, float bottom);
	BoundingRectangle(const GSvector2& min, const GSvector2& max);

	// ��`���d�Ȃ��Ă��邩�H
	bool intersects(const BoundingRectangle& other) const;

	// ���s�ړ�
	BoundingRectangle translate(const GSvector2& position) const;

	// �Q�b�^�[�֐�
	float width() const;
	float height() const;
	const GSvector2& min() const;
	const GSvector2& max() const;

private:
	GSvector2 min_{ 0.0f, 0.0f };	// ����
	GSvector2 max_{ 0.0f, 0.0f };	// �E��
};

#endif

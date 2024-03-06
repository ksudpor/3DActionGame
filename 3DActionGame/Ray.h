#ifndef RAY_H_
#define RAY_H_

#include <gslib.h>

// ���C�N���X
class Ray {
public:
	Ray() = default;
	Ray(const GSvector3& position, const GSvector3& direction) :
		position{ position }, direction{ direction } {}
public:
	GSvector3 position{ 0.0f, 0.0f, 0.0f };			// ���W
	GSvector3 direction{ 0.0f, 0.0f, 0.0f };		// ����
};
#endif
#ifndef RIFLE_H_
#define RIFLE_H_

#include "Weapon.h"

class Rifle : public Weapon {
public:
	Rifle(IWorld* world);
	void event_fire() override;

private:
	void generate_bullet(const GSvector3& gen_pos);
};

#endif

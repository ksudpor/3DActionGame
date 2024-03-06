#ifndef SHOTGUN_H_
#define SHOTGUN_H_

#include "Weapon.h"

class Shotgun : public Weapon {
public:
	Shotgun(IWorld* world);
	void event_fire() override;

private:
	void generate_bullet(const GSvector3& gen_pos);
};

#endif
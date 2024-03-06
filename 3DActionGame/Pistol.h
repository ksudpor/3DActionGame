#ifndef PISTOL_H_
#define PISTOL_H_

#include "Weapon.h"

class Pistol : public Weapon {
public:
	Pistol(IWorld* world);
	void event_fire() override;

private:
	void generate_bullet(const GSvector3& gen_pos);
};

#endif
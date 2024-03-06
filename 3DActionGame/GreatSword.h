#ifndef GREAT_SWORD_H_
#define GREAT_SWORD_H_

#include "Weapon.h"

class GreatSword : public Weapon {
public:
	GreatSword(IWorld* world);
	void event_combo_attack1() override;
	void event_combo_attack2() override;
	void event_combo_attack3() override;

private:
	void generate_attack_collider(float d_forward, float d_side, float radius);
};

#endif

#ifndef SELECT_WEAPON_SCENE_H_
#define SELECT_WEAPON_SCENE_H_

#include "IScene.h"
#include <gslib.h>
#include "GameProgress.h"
#include "Fade.h"

class SelectWeaponScene : public IScene {
	enum class SelectMenu {
		Weapon1,
		Weapon2,
		Confirm
	};

	enum class WeaponMenu {
		Axe,
		SwordShield,
		GreatSword,
		Pistol,
		Rifle,
		Shotgun
	};

public:
	virtual void start() override;
	virtual void update(float delta_time) override;
	virtual void draw() const override;
	virtual bool is_end() const override;
	virtual std::string next() const override;
	virtual void end() override;

private:
	GameProgress::Weapon confirm_weapon(WeaponMenu weapon);

private:
	bool is_end_{ false };
	SelectMenu menu_;
	WeaponMenu weapon1_{ WeaponMenu::Axe };
	WeaponMenu weapon2_{ WeaponMenu::Axe };
	bool same_weapon_;
	Fade* fade_;
};

#endif
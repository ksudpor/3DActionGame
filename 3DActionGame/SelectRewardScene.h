#ifndef SELECT_REWARD_SCENE
#define SELECT_REWARD_SCENE

#include "IScene.h"
#include <gslib.h>
#include "GameProgress.h"
#include "Fade.h"

class SelectRewardScene : public IScene {
	enum class SelectMenu {
		Reward,
		Confirm
	};

	enum class RewardMenu {
		Health,
		Stamina,
		Speed,
		Attack
	};

public:
	virtual void start() override;
	virtual void update(float delta_time) override;
	virtual void draw() const override;
	virtual bool is_end() const override;
	virtual std::string next() const override;
	virtual void end() override;

private:
	//GameProgress::Weapon confirm_reward(RewardMenu weapon);

private:
	bool is_end_{ false };
	SelectMenu menu_;
	RewardMenu reward_{ RewardMenu::Health };
	Fade* fade_;
};

#endif

#include "StageManager.h"
#include "IWorld.h"
#include "Field.h"
#include "Player.h"
#include "EnemyMelee.h"
#include "EnemyRanged.h"
#include "Assets.h"
#include "Language.h"
#include "Screen.h"
#include "NumberTexture.h"
#include "KeyItem.h"
#include "HealthPotion.h"
#include "StageOne.h"
#include "StageTwo.h"
#include "StageThree.h"

StageManager::StageManager(IWorld* world) {
	world_ = world;
	tag_ = "StageManager";
	name_ = "StageManager";
	enable_collider_ = false;

	switch (GameProgress::stage_) {
	case GameProgress::StageProgress::One:
		stage_ = new StageOne{ world_ };	break;
	case GameProgress::StageProgress::Two:
		stage_ = new StageTwo{ world_ };	break;
	case GameProgress::StageProgress::Three:
		stage_ = new StageThree{ world_ };	break;
	}
}

void StageManager::update(float delta_time) {
	stage_->update(delta_time);
}

void StageManager::draw() const {
	stage_->draw();
}

bool StageManager::game_clear() const{
	return stage_->game_clear();
}

bool StageManager::game_over() const{
	return stage_->game_over();
}

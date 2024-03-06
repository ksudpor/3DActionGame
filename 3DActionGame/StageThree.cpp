#include "StageThree.h"
#include "IWorld.h"
#include "Field.h"
#include "Player.h"
#include "EnemyMelee.h"
#include "EnemyRanged.h"
#include "Assets.h"
#include "Language.h"
#include "Screen.h"
#include "NumberTexture.h"
#include "HealthPotion.h"
#include "KeyItem.h"
#include "Goal.h"

StageThree::StageThree(IWorld* world){
	world_ = world;

	gsLoadOctree(Octree_Stage, "Assets/Octree/Stage3/stage3.oct");
	gsLoadOctree(Octree_Collider, "Assets/Octree/Stage3/stage3_collider.oct");

	for (int i = 0; i < 15; ++i) {
		spawn_enemies_[i] = nullptr;
		respawn_timer_[i] = 0.0f;
	}

	for (int i = 0; i < 5; ++i) {
		keys_[i] = nullptr;
	}

	world_->add_actor(new Player{ world_, GSvector3{ -21.0f, 0.125, 21.0f } });
	world_->add_actor(new HealthPotion{ world_, GSvector3{ -21.0f, 0.125, 21.0f } });
	player_ = world_->find_actor("Player");
	goal_ = nullptr;
}

void StageThree::update(float delta_time){
	int spawnIndex = gsRand(0, 14);
	if (spawn_enemies_[spawnIndex] == nullptr
		&& respawn_timer_[spawnIndex] <= 0) {
		float spawnX = gsRandf(-21, 19);
		float spawnZ = gsRandf(-19, 21);
		if ((spawnX <= player_->transform().position().x - 5.0f || spawnX >= player_->transform().position().x + 5.0f)
			&& (spawnZ <= player_->transform().position().z - 5.0f || spawnZ >= player_->transform().position().z + 5.0f)) {
			GSvector3 position = GSvector3{ spawnX, 0.125f, spawnZ };
			Actor* enemy;
			if (gsRandf(1, 100) < 50) enemy = new EnemyMelee{ world_, position };
			else enemy = new EnemyRanged{ world_, position };
			world_->add_actor(enemy);
			spawn_enemies_[spawnIndex] = enemy;
		}
	}

	for (int i = 0; i < 15; ++i) {
		if (spawn_enemies_[i] == nullptr)  respawn_timer_[i] -= delta_time;
		else {
			respawn_timer_[i] = 1800.0f;
			if (spawn_enemies_[i]->is_dead()) {
				spawn_enemies_[i] = nullptr;
			}
		}
	}

	for (int i = 0; i < 5; ++i) {
		if (keys_[i] == nullptr) {
			float spawnX = gsRandf(-21, 19);
			float spawnZ = gsRandf(-19, 21);
			if ((spawnX <= player_->transform().position().x - 1.0f || spawnX >= player_->transform().position().x + 1.0f)
				&& (spawnZ <= player_->transform().position().z - 1.0f || spawnZ >= player_->transform().position().z + 1.0f)) {
				GSvector3 position = GSvector3{ spawnX, 0.125f, spawnZ };
				keys_[i] = new KeyItem{ world_, position };
				world_->add_actor(keys_[i]);
			}
		}
		else {
			if (keys_[i]->is_dead()) keys_[i] = nullptr;
		}
	}

	if (world_->time() <= 0) {
		if (goal_ == nullptr) {
			//goal_ = new Goal{ world_, GSvector3{ -1.0f, 0.125, 1.0f } };
			world_->add_actor(new Goal{ world_, GSvector3{ -1.0f, 0.125, 1.0f } });
		}
		goal_ = world_->find_actor("Goal");
	}
}

void StageThree::draw(){
	GSvector2 quest_pos{ Screen::Width * 0.85f, Screen::Height * 0.15f };
	gsDrawSprite2D(Language::quest_stage3_image, &quest_pos, NULL, NULL, NULL, &Screen::Scale, 0.0f);
}

bool StageThree::game_clear(){
	if (goal_ != nullptr) {
		return goal_->is_dead();
	}
	return false;
}

bool StageThree::game_over() {
	return false;
}

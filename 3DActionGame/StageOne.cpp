#include "StageOne.h"
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

StageOne::StageOne(IWorld* world){
	world_ = world;

	gsLoadOctree(Octree_Stage, "Assets/Octree/Stage1/stage1.oct");
	gsLoadOctree(Octree_Collider, "Assets/Octree/Stage1/stage1_collider.oct");

	for (int i = 0; i < 10; ++i) {
		spawn_enemies_[i] = nullptr;
		respawn_timer_[i] = 0.0f;
	}
	dead_count_ = 0;

	world_->add_actor(new Player{ world_, GSvector3{ -3.0f, 0.125, 0.5f } });
	world_->add_actor(new HealthPotion{ world_, GSvector3{ -3.0f, 0.125, 0.5f } });
}

void StageOne::update(float delta_time){
	GSvector3 spawnPosition[10] = {
		GSvector3{-1.5f, 0.125f, -11.0f}, GSvector3{7.5f, 0.125f, -11.0f},
		GSvector3{6.5f, 0.125f, -2.0f}, GSvector3{15.0f, 0.125f, -3.5f},
		GSvector3{0.5f, 0.125f, -15.0f}, GSvector3{6.0f, 0.125f, -23.0f},
		GSvector3{20.0f, 0.125f, -9.0f}, GSvector3{10.0f, 0.125f, -15.0f},
		GSvector3{21.5f, 0.125f, -23.0f}, GSvector3{16.0f, 0.125f, -23.0f} };

	int spawnIndex = gsRand(0, 9);
	if (spawn_enemies_[spawnIndex] == nullptr
		&& respawn_timer_[spawnIndex] <= 0) {
		GSvector3 position = spawnPosition[spawnIndex];
		Actor* enemy;
		if (gsRandf(1, 100) < 50) enemy = new EnemyMelee{ world_, position };
		else enemy = new EnemyRanged{ world_, position };

		int count = world_->count_actor_with_tag("EnemyTag");
		if (count < 3) {
			world_->add_actor(enemy);
			spawn_enemies_[spawnIndex] = enemy;
		}
	}

	for (int i = 0; i < 10; ++i) {
		if (spawn_enemies_[i] == nullptr)  respawn_timer_[i] -= delta_time;
		else {
			respawn_timer_[i] = 1800.0f;
			if (spawn_enemies_[i]->is_dead()) {
				spawn_enemies_[i] = nullptr;
				++dead_count_;
			}
		}
	}
}

void StageOne::draw(){
	GSvector2 quest_pos{ Screen::Width * 0.85f, Screen::Height * 0.15f };
	GSvector2 slash_pos{ Screen::Width * 0.9f, Screen::Height * 0.25f };
	GSvector2 slash_scale{ Screen::Scale * 0.65f };
	gsDrawSprite2D(Language::quest_stage1_image, &quest_pos, NULL, NULL, NULL, &Screen::Scale, 0.0f);
	gsDrawSprite2D(Image_Slash, &slash_pos, NULL, NULL, NULL, &slash_scale, 0.0f);
	NumberTexture number{ Image_TextureNumber, 64, 64 };
	number.set_scale(0.65f);
	number.draw(GSvector2{ Screen::Width * 0.8625f, Screen::Height * 0.25f }, dead_count_, 2);
	number.draw(GSvector2{ Screen::Width * 0.9125f, Screen::Height * 0.25f }, 15, 2);
}

bool StageOne::game_clear(){
	return dead_count_ >= 15;
}

bool StageOne::game_over() {
	return world_->time() <= 0;
}

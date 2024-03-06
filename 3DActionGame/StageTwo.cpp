#include "StageTwo.h"
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

StageTwo::StageTwo(IWorld* world){
	world_ = world;

	gsLoadOctree(Octree_Stage, "Assets/Octree/Stage2/stage2.oct");
	gsLoadOctree(Octree_Collider, "Assets/Octree/Stage2/stage2_collider.oct");

	for (int i = 0; i < 10; ++i) {
		spawn_enemies_[i] = nullptr;
		respawn_timer_[i] = 0.0f;
	}
	key_count_ = 0;
	world_->add_actor(new Player{ world_, GSvector3{ -13.0f, 0.125, -23.0f } });
	world_->add_actor(new HealthPotion{ world_, GSvector3{ -13.0f, 0.125, -23.0f } });

	GSvector3 keyPosition[10] = {
		GSvector3{ -9.0f ,0.125f ,-1.0f }, GSvector3{ 14.0f, 0.125f, -7.0f },
		GSvector3{ -1.5f, 0.125f, -16.0f }, GSvector3{ 14.0f, 3.125f, 2.5f },
		GSvector3{ -5.0f, 6.125f, -1.0f }, GSvector3{ 13.0f, 6.125f, -23.0f },
		GSvector3{ 8.0f, 6.125f, -1.0f }, GSvector3{ 11.0f, 6.125f, -5.0f },
		GSvector3{ 8.0f, 12.125f, -18.5f }, GSvector3{ -6.0f, 12.125f, -2.0f } };

	for (int i = 0; i < 10; ++i) {
		keys_[i] = new KeyItem{ world_, keyPosition[i] };
		world_->add_actor(keys_[i]);
	}

}

void StageTwo::update(float delta_time){
	GSvector3 spawnPosition[10] = {
	GSvector3{-0.0f, 0.125f, -18.5f}, GSvector3{13.0f, 0.125f, -1.0f},
	GSvector3{2.0f, 3.125f, 2.5f}, GSvector3{-9.5f, 3.125f, -2.0f},
	GSvector3{-6.0f, 6.125f, -8.0f}, GSvector3{5.0f, 6.125f, -19.0f},
	GSvector3{5.0f, 6.125f, -6.0f}, GSvector3{11.0f, 6.125f, -12.0f},
	GSvector3{7.0f, 12.125f, -1.0f}, GSvector3{-6.0f, 12.125f, -14.0f} };
	
	int spawnIndex = gsRand(0, 9);
	if (spawn_enemies_[spawnIndex] == nullptr
		&& respawn_timer_[spawnIndex] <= 0) {
		GSvector3 position = spawnPosition[spawnIndex];
		Actor* enemy;
		if (gsRandf(1, 100) < 50) enemy = new EnemyMelee{ world_, position };
		else enemy = new EnemyRanged{ world_, position };

		world_->add_actor(enemy);
		spawn_enemies_[spawnIndex] = enemy;
	}

	for (int i = 0; i < 10; ++i) {
		if (spawn_enemies_[i] == nullptr)  respawn_timer_[i] -= delta_time;
		else {
			respawn_timer_[i] = 1800.0f;
			if (spawn_enemies_[i]->is_dead()) {
				spawn_enemies_[i] = nullptr;
			}
		}
	}
	
	for (int i = 0; i < 10; ++i) {
		if (keys_[i] != nullptr && keys_[i]->is_dead()) {
			++key_count_;
			keys_[i] = nullptr;
			
		}
	}
}

void StageTwo::draw(){
	GSvector2 quest_pos{ Screen::Width * 0.85f, Screen::Height * 0.15f };
	GSvector2 slash_pos{ Screen::Width * 0.9f, Screen::Height * 0.25f };
	GSvector2 slash_scale{ Screen::Scale * 0.65f };
	gsDrawSprite2D(Language::quest_stage2_image, &quest_pos, NULL, NULL, NULL, &Screen::Scale, 0.0f);
	gsDrawSprite2D(Image_Slash, &slash_pos, NULL, NULL, NULL, &slash_scale, 0.0f);
	NumberTexture number{ Image_TextureNumber, 64, 64 };
	number.set_scale(0.65f);
	number.draw(GSvector2{ Screen::Width * 0.8625f, Screen::Height * 0.25f }, key_count_, 2);
	number.draw(GSvector2{ Screen::Width * 0.9125f, Screen::Height * 0.25f }, 10, 2);
}

bool StageTwo::game_clear(){
    return key_count_ >= 10;
}

bool StageTwo::game_over() {
	return world_->time() <= 0;
}

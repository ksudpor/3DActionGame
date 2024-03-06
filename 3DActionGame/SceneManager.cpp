#include "SceneManager.h"
#include "SceneNull.h"

// nullシーン（何もしないダミーシーン）
static SceneNull scene_null;

SceneManager::SceneManager() :
	current_scene_{ &scene_null }{
}

SceneManager::~SceneManager(){
	clear();
}

void SceneManager::update(float delta_time) {
	current_scene_->update(delta_time);
	if (current_scene_->is_end()) {
		change(current_scene_->next());
	}
}

void SceneManager::draw() const{
	current_scene_->draw();
}

void SceneManager::end(){
	current_scene_->end();
	current_scene_ = &scene_null;
}

void SceneManager::add(const std::string& name, IScene* scene){
	scenes_[name] = scene;
}

void SceneManager::change(const std::string& name) {
	end();
	current_scene_ = scenes_[name];
	current_scene_->start();
}

void SceneManager::clear(){
	end();
	for (auto& pair : scenes_) {
		delete pair.second;
	}
	scenes_.clear();
}

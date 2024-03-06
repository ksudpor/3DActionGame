#ifndef SCENE_MANAGER_H_
#define SCENE_MANAGER_H_

#include <unordered_map>
#include <vector>
#include <string>

class IScene;

// �V�[���Ǘ��N���X
class SceneManager {
public:
	SceneManager();
	~SceneManager();
	void update(float delta_time);
	void draw() const;
	void end();
	void add(const std::string& name, IScene* scene);
	void change(const std::string& name);
	void clear();

	// �R�s�[�֎~
	SceneManager(const SceneManager& other) = delete;
	SceneManager& operator = (const SceneManager& other) = delete;

private:
	std::unordered_map<std::string, IScene*> scenes_;
	IScene* current_scene_;
};


#endif
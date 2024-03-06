#include <GSgame.h>
#include "SceneManager.h"
#include "InitialSettingScene.h"
#include "TitleScene.h"
#include "SelectWeaponScene.h"
#include "SelectRewardScene.h"
#include "GamePlayScene.h"
#include "OptionScene.h"
#include "GameOverScene.h"
#include "GameClearScene.h"
#include "ResultScene.h"
#include "Screen.h"

// �Q�[���N���X
class MyGame : public gslib::Game {
public:
	// �R���X�g���N�^
	MyGame() : gslib::Game{ Screen::Width, Screen::Height, 
		Screen::Is_Fullscreen, (float)Screen::Refresh_Rate } {
	}

private:
	SceneManager scene_manager_;
	
	// �J�n
	void start() override {
		GLFWwindow* window = gslib::Window::instance().get_glfw_window();
		glfwSetWindowTitle(window, "Space Fighter");
		// �V�[���}�l�[�W���[�̏�����
		scene_manager_.add("InitialSettingScene", new InitialSettingScene());
		scene_manager_.add("TitleScene", new TitleScene());
		scene_manager_.add("SelectWeaponScene", new SelectWeaponScene());
		scene_manager_.add("SelectRewardScene", new SelectRewardScene());
		scene_manager_.add("GamePlayScene", new GamePlayScene());
		scene_manager_.add("OptionScene", new OptionScene());
		scene_manager_.add("GameOverScene", new GameOverScene());
		scene_manager_.add("GameClearScene", new GameClearScene());
		scene_manager_.add("ResultScene", new ResultScene());
		scene_manager_.change("InitialSettingScene");
	}

	// �X�V
	void update(float delta_time) override {
		scene_manager_.update(delta_time);
	}

	// �`��
	void draw() override {
		scene_manager_.draw();
	}

	// �I��
	void end() override {
		// �V�[���}�l�[�W���[�̏I��
		scene_manager_.end();
		scene_manager_.clear();
	}
};

// ���C���֐�
int main() {
	try {
		return MyGame().run();
	}
	catch (const std::exception& e) {
		return 0;
	}
}

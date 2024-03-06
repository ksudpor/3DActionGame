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

// ゲームクラス
class MyGame : public gslib::Game {
public:
	// コンストラクタ
	MyGame() : gslib::Game{ Screen::Width, Screen::Height, 
		Screen::Is_Fullscreen, (float)Screen::Refresh_Rate } {
	}

private:
	SceneManager scene_manager_;
	
	// 開始
	void start() override {
		GLFWwindow* window = gslib::Window::instance().get_glfw_window();
		glfwSetWindowTitle(window, "Space Fighter");
		// シーンマネージャーの初期化
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

	// 更新
	void update(float delta_time) override {
		scene_manager_.update(delta_time);
	}

	// 描画
	void draw() override {
		scene_manager_.draw();
	}

	// 終了
	void end() override {
		// シーンマネージャーの終了
		scene_manager_.end();
		scene_manager_.clear();
	}
};

// メイン関数
int main() {
	try {
		return MyGame().run();
	}
	catch (const std::exception& e) {
		return 0;
	}
}

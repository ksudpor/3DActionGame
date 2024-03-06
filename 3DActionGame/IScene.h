#ifndef ISCENE_H_
#define ISCENE_H_

#include <vector>
#include <string>

// シーン抽象インターフェース
class IScene {
public:
	virtual ~IScene() = default;
	virtual void start() = 0;
	virtual void update(float delta_time) = 0;
	virtual void draw() const = 0;
	virtual bool is_end() const = 0;
	virtual std::string next() const = 0;
	virtual void end() = 0;
};

#endif

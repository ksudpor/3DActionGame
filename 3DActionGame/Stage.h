#ifndef STAGE_H_
#define STAGE_H_

class IWorld;

class Stage {
public:
	Stage() = default;
	~Stage() = default;
	virtual void update(float delta_time) = 0;
	virtual void draw() = 0;
	virtual bool game_clear() = 0;
	virtual bool game_over() = 0;
};

#endif

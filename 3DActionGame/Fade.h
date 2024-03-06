#ifndef FADE_H_
#define FADE_H_

#include <gslib.h>

class Fade {
public:
	Fade();
	void update(float delta_time);
	void draw() const;
	bool is_fading();
	void start_fade_in(float fade_time);
	void start_fade_out(float fade_time);
	bool end_scene();

private:
	float fade_time_;
	float timer_;
	GScolor fade_in_color_;
	GScolor fade_out_color_;
	bool is_fade_in_;
	bool is_fade_out_;
	bool is_fade_out_end_;
};

#endif

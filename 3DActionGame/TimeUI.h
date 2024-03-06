#ifndef TIME_UI_H_
#define TIME_UI_H_

class TimeUI {
public:
	//10 min = 36000
	TimeUI(float time = 36000.0f);
	void initialize(float time);
	void update(float delta_time);
	void draw() const;
	float get() const;
	float spent_time() const;
	void clear();

private:
	float time_;
	float used_time_;
	float max_time_;
};

#endif

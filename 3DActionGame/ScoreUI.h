#ifndef SCORE_UI_H_
#define SCORE_UI_H_

class ScoreUI {
public:
	ScoreUI(int score = 0);
	void initialize(int score = 0);
	void add(int score);
	void draw() const;
	int get() const;
	void clear();

private:
	int score_;
};

#endif

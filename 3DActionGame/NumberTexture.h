#ifndef NUMBER_TEXTURE_H_
#define NUMBER_TEXTURE_H_

#include <gslib.h>
#include <string>

class NumberTexture {
public:
	NumberTexture(GSuint texture, int width, int height);
	void draw(const GSvector2& position, int num, int digit, char fill = '0', const GScolor& color = GScolor(1.0f, 1.0f, 1.0f, 1.0f)) const;
	void draw(const GSvector2& position, int num, const GScolor& color = GScolor(1.0f, 1.0f, 1.0f, 1.0f)) const;
	void draw(const GSvector2& position, const std::string& num, const GScolor& color = GScolor(1.0f, 1.0f, 1.0f, 1.0f)) const;
	void set_scale(float scale);

private:
	GSuint texture_;
	int width_;
	int height_;
	float scale_;
};

#endif

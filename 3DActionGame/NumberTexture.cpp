#include "NumberTexture.h"
#include "Screen.h"
#include <sstream>
#include <iomanip>

NumberTexture::NumberTexture(GSuint texture, int width, int height) :
	texture_{ texture }, width_{ width }, height_{ height } {
}

void NumberTexture::draw(const GSvector2& position, int num, int digit, char fill, const GScolor& color) const{
	std::stringstream ss;
	ss << std::setw(digit) << std::setfill(fill) << num;
	draw(position, ss.str(), color);
}

void NumberTexture::draw(const GSvector2& position, int num, const GScolor& color) const{
	draw(position, std::to_string(num), color);
}

void NumberTexture::draw(const GSvector2& position, const std::string& num, const GScolor& color) const {
	for (int i = 0; i < (int)num.size(); ++i) {
		if (num[i] == ' ') continue;
		int n = num[i] - '0';
		GSvector2 scale = Screen::Scale * scale_;
		GSrect rect(n * width_, 0.0f, (n * width_) + width_, height_);
		GSvector2 pos{ position.x + i * width_ * scale.x , position.y };
		gsDrawSprite2D(texture_, &pos, &rect, NULL, &color, &scale, 0);
	}
}

void NumberTexture::set_scale(float scale){
	scale_ = scale;
}

#ifndef BOUNDING_RECTANGLE_H_
#define BOUNDING_RECTANGLE_H_

#include <gslib.h>	//GSvector2

// 矩形クラス
class BoundingRectangle {
public:
	// コンストラクタ
	BoundingRectangle() = default;
	BoundingRectangle(float left, float top, float right, float bottom);
	BoundingRectangle(const GSvector2& min, const GSvector2& max);

	// 矩形が重なっているか？
	bool intersects(const BoundingRectangle& other) const;

	// 平行移動
	BoundingRectangle translate(const GSvector2& position) const;

	// ゲッター関数
	float width() const;
	float height() const;
	const GSvector2& min() const;
	const GSvector2& max() const;

private:
	GSvector2 min_{ 0.0f, 0.0f };	// 左上
	GSvector2 max_{ 0.0f, 0.0f };	// 右下
};

#endif

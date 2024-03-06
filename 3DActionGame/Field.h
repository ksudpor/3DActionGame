#ifndef FIELD_H_
#define FIELD_H_

#include <gslib.h>

class Ray;
class Line;
class BoundingSphere;

// フィールドクラス
class Field {
public:
	Field(GSuint octree, GSuint collider, GSuint skybox);
	void update(float delta_time);
	void draw() const;

	// 線分との衝突判定
	bool collide(const Line& line,
		GSvector3* intersect = nullptr, GSplane* plane = nullptr) const;
	// レイとの衝突判定
	bool collide(const Ray& ray, float max_distance,
		GSvector3* intersect = nullptr, GSplane* plane = nullptr) const;
	// 球体との衝突判定
	bool collide(const BoundingSphere& sphere, GSvector3* center = nullptr) const;

	// コピー禁止
	Field(const Field& other) = delete;
	Field& operator = (const Field& other) = delete;
private:
	GSuint octree_;			// 描画用オクトリー
	GSuint collider_;		// 衝突判定用オクトリー
	GSuint skybox_;			// スカイボックス
};
#endif
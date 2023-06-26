#pragma once

//球
struct Sphere {
	//中心座標
	DirectX::XMVECTOR center = { 0,0,0,1 };
	//半径
	float radius = 1.0f;

	void SetPos(const Vector3& pos) {
		center.m128_f32[0] = pos.x;
		center.m128_f32[1] = pos.y;
		center.m128_f32[2] = pos.z;
	};

	// フラグ
	bool isActive = false;
};
//平面
struct Plane {
	//法線ベクトル
	DirectX::XMVECTOR normal = { 0,1,0,0 };
	//原点(0,0,0)空の距離
	float distance = 0.0f;
};
//法線付き三角形(時計回りが表面)
struct Triangle {
	//頂点座標３つ
	DirectX::XMVECTOR p0;
	DirectX::XMVECTOR p1;
	DirectX::XMVECTOR p2;
	//法線ベクトル
	DirectX::XMVECTOR normal;
};

bool RayCollision(const WorldTransform& ray, const WorldTransform& obj);

bool BallCollision(const WorldTransform& a, const WorldTransform& b);

bool BallCollision(const Vector3& a, const float& aSize, const Vector3& b, const float& bSize);

//球と球
bool BallCollision(const Sphere& a, const Sphere& b);
//平面と球
bool Sphere2PlaneCol(const Sphere& sphere, const Plane& plane,
	DirectX::XMVECTOR* inter = nullptr);
//点と三角形
void ClosestPtPoint2Triangle(const DirectX::XMVECTOR& point, const Triangle& triangle, DirectX::XMVECTOR* closest);
//弾と法線付き三角形の当たり判定チェック
bool Sphere2TriangleCol(const Sphere& sphere, const Triangle& triangle,
	DirectX::XMVECTOR* inter = nullptr);

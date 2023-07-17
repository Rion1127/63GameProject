#pragma once
#include "Vector3.h"
#include "WorldTransform.h"
//球
struct Sphere {
	//中心座標
	Vector3 center = {0,0,0};
	//半径
	float radius = 1.0f;

	void SetPos(Vector3 pos) {
		center = pos;
	};

	// フラグ
	bool isActive = true;
};
//平面
struct Plane {
	//法線ベクトル
	Vector3 normal = { 0,1,0};
	//原点(0,0,0)からの距離
	float distance = 0.0f;
};
//法線付き三角形(時計回りが表面)
struct Triangle {
	//頂点座標３つ
	Vector3 p0;
	Vector3 p1;
	Vector3 p2;
	//法線ベクトル
	Vector3 normal;
	//法背の計算
	void ComputeNormal();
};
//レイ(半直線)
struct Ray {
	//始点座標
	Vector3 start = { 0,0,0};
	//方向
	Vector3 dir = { 1,0,0};
};

bool RayCollision(WorldTransform ray, WorldTransform obj);

bool BallCollision(const WorldTransform& a, const WorldTransform& b);

bool BallCollision(const Vector3& a, const float& aSize, const Vector3& b, const float& bSize);

//球と球
bool BallCollision(const Sphere& a, const Sphere& b);
//平面と球
bool Sphere2PlaneCol(const Sphere& sphere, const Plane& plane,
	Vector3* inter = nullptr);
//点と三角形
void ClosestPtPoint2Triangle(const Vector3& point, const Triangle& triangle, Vector3* closest);
//球と法線付き三角形の当たり判定チェック
bool Sphere2TriangleCol(const Sphere& sphere, const Triangle& triangle,
	Vector3* inter = nullptr);
//レイと平面
bool CheckRay2Plane(const Ray& ray, const Plane& plane, float* distance = nullptr, Vector3* inter = nullptr);

//レイと法線付き三角形の当たり判定
bool CheckRay2Traiangle(const Ray& ray, const Triangle& triangle,
	float* distance = nullptr, Vector3* inter = nullptr);

//レイと級の当たり判定
bool CheckRay2Sphere(const Ray& ray, const Sphere& sphere,
	float* distance = nullptr, Vector3* inter = nullptr);
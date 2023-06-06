#include "Vector3.h"
#include <list>
#include <memory>
#include "WorldTransform.h"
#include "Collision.h"



bool RayCollision(WorldTransform ray, WorldTransform obj)
{
	//レイの当たり判定

	//ワールド座標を代入
	Vector3 objPos;
	Vector3 rayPos;
	objPos = obj.position;
	rayPos = ray.position;
#pragma region レイの当たり判定
	//レイの始点と終点を代入
	Vector3 rayStart;
	Vector3 rayEnd;
	rayStart = {
		ray.position.x,
		ray.position.y,
		ray.position.z - ray.scale.z - 11.0f,
	};
	rayEnd = {
		ray.position.x,
		ray.position.y,
		ray.position.z + ray.scale.z + 11.0f,
	};
	//始点と終点からレイのベクトル(a→)を求める
	Vector3 rayVec;
	rayVec = rayEnd - rayStart;
	float raySize;
	raySize = rayVec.length();
	//正規化(a→N)
	rayVec.normalize();
	//レイとオブジェクトのベクトル(b→)を求める
	// レイの始点とオブジェクトへのベクトル(b→)を求める
	Vector3 ABVec;
	ABVec = {
		objPos.x - rayStart.x,
		objPos.y - rayStart.y,
		objPos.z - rayStart.z
	};

	//b→・a→N をray2ObjLengthに格納
	float ray2ObjectLength = ABVec.dot(rayVec);

	//Qを求める a→N * b→・a→N + P
	Vector3 Q;
	//Q = rayVec * ABVec.dot(rayVec) + rayPos;
	Q = rayVec * ray2ObjectLength + rayStart;
	//オブジェクトからレイの垂線(obj-Q)を求める
	Vector3 line;
	line = objPos - Q;
	//垂線の長さを求める
	line.length();

	//垂線の長さが半径+半径より短ければ当たってる
	bool isHit = false;
	if (line.length() <= obj.scale.x + 4.3f) {
		if (raySize >= ray2ObjectLength) {
			isHit = true;
		}
	}

	if (ray2ObjectLength <= 0) {
		isHit = false;
	}

	return isHit;

#pragma endregion
}
//球と球
bool BallCollision(const WorldTransform& a, const WorldTransform& b) {
	float x, y, z;
	float r;

	x = (float)pow(b.position.x - a.position.x, 2);
	y = (float)pow(b.position.y - a.position.y, 2);
	z = (float)pow(b.position.z - a.position.z, 2);

	float pos = x + y + z;

	r = (float)pow(a.scale.x + b.scale.x, 2);
	if (pos <= r) {
		return true;
	}
	return false;
}
//球と球
bool BallCollision(const Vector3& a, const float& aSize, const Vector3& b, const float& bSize) {
	float x, y, z;
	float r;

	x = (float)pow(b.x - a.x, 2);
	y = (float)pow(b.y - a.y, 2);
	z = (float)pow(b.z - a.z, 2);

	float pos = x + y + z;

	r = (float)pow(aSize + bSize, 2);
	if (pos <= r) {
		return true;
	}
	return false;
}
//球と球
bool BallCollision(const Sphere& a, const Sphere& b)
{
	float x, y, z;
	float r;

	x = (float)pow(b.center.m128_f32[0] - a.center.m128_f32[0], 2);
	y = (float)pow(b.center.m128_f32[1] - a.center.m128_f32[1], 2);
	z = (float)pow(b.center.m128_f32[2] - a.center.m128_f32[2], 2);

	float pos = x + y + z;

	r = (float)pow(a.radius + b.radius, 2);
	if (pos <= r) {
		return true;
	}
	return false;
}
//平面と球
bool Sphere2PlaneCol(const Sphere& sphere, const Plane& plane, DirectX::XMVECTOR* inter)
{
	//座標系の原点から球の中心座標への距離
	XMVECTOR distV = XMVector3Dot(sphere.center, plane.normal);
	//平面の原点距離を減産することで、平面と球の中心との距離が出る
	float dist = distV.m128_f32[0] - plane.distance;
	//距離の絶対値が半径より大きければ当たっていない
	if (fabsf(dist) > sphere.radius) return false;

	//疑似交点を計算
	if (inter) {
		//平面上の最近接点を、議事好転とする
		*inter = -dist * plane.normal + sphere.center;
	}

	return true;
}
//点と三角形
void ClosestPtPoint2Triangle(const DirectX::XMVECTOR& point, const Triangle& triangle, DirectX::XMVECTOR* closest)
{
	// pointがp0の外側の頂点領域の中にあるかどうかチェック
	XMVECTOR p0_p1 = triangle.p1 - triangle.p0;
	XMVECTOR p0_p2 = triangle.p2 - triangle.p0;
	XMVECTOR p0_pt = point - triangle.p0;

	XMVECTOR d1 = XMVector3Dot(p0_p1, p0_pt);
	XMVECTOR d2 = XMVector3Dot(p0_p2, p0_pt);

	if (d1.m128_f32[0] <= 0.0f && d2.m128_f32[0] <= 0.0f)
	{
		// p0が最近傍
		*closest = triangle.p0;
		return;
	}

	// pointがp1の外側の頂点領域の中にあるかどうかチェック
	XMVECTOR p1_pt = point - triangle.p1;

	XMVECTOR d3 = XMVector3Dot(p0_p1, p1_pt);
	XMVECTOR d4 = XMVector3Dot(p0_p2, p1_pt);

	if (d3.m128_f32[0] >= 0.0f && d4.m128_f32[0] <= d3.m128_f32[0])
	{
		// p1が最近傍
		*closest = triangle.p1;
		return;
	}

	// pointがp0_p1の辺領域の中にあるかどうかチェックし、あればpointのp0_p1上に対する射影を返す
	float vc = d1.m128_f32[0] * d4.m128_f32[0] - d3.m128_f32[0] * d2.m128_f32[0];
	if (vc <= 0.0f && d1.m128_f32[0] >= 0.0f && d3.m128_f32[0] <= 0.0f)
	{
		float v = d1.m128_f32[0] / (d1.m128_f32[0] - d3.m128_f32[0]);
		*closest = triangle.p0 + v * p0_p1;
		return;
	}

	// pointがp2の外側の頂点領域の中にあるかどうかチェック
	XMVECTOR p2_pt = point - triangle.p2;

	XMVECTOR d5 = XMVector3Dot(p0_p1, p2_pt);
	XMVECTOR d6 = XMVector3Dot(p0_p2, p2_pt);
	if (d6.m128_f32[0] >= 0.0f && d5.m128_f32[0] <= d6.m128_f32[0])
	{
		*closest = triangle.p2;
		return;
	}

	// pointがp0_p2の辺領域の中にあるかどうかチェックし、あればpointのp0_p2上に対する射影を返す
	float vb = d5.m128_f32[0] * d2.m128_f32[0] - d1.m128_f32[0] * d6.m128_f32[0];
	if (vb <= 0.0f && d2.m128_f32[0] >= 0.0f && d6.m128_f32[0] <= 0.0f)
	{
		float w = d2.m128_f32[0] / (d2.m128_f32[0] - d6.m128_f32[0]);
		*closest = triangle.p0 + w * p0_p2;
		return;
	}

	// pointがp1_p2の辺領域の中にあるかどうかチェックし、あればpointのp1_p2上に対する射影を返す
	float va = d3.m128_f32[0] * d6.m128_f32[0] - d5.m128_f32[0] * d4.m128_f32[0];
	if (va <= 0.0f && (d4.m128_f32[0] - d3.m128_f32[0]) >= 0.0f && (d5.m128_f32[0] - d6.m128_f32[0]) >= 0.0f)
	{
		float w = (d4.m128_f32[0] - d3.m128_f32[0]) / ((d4.m128_f32[0] - d3.m128_f32[0]) + (d5.m128_f32[0] - d6.m128_f32[0]));
		*closest = triangle.p1 + w * (triangle.p2 - triangle.p1);
		return;
	}

	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	*closest = triangle.p0 + p0_p1 * v + p0_p2 * w;
}
//弾と法線付き三角形の当たり判定チェック
bool Sphere2TriangleCol(const Sphere& sphere, const Triangle& triangle, DirectX::XMVECTOR* inter)
{
	XMVECTOR p;
	//弾の中心に対する最近接点である三角形上にある点pを見つける
	ClosestPtPoint2Triangle(sphere.center, triangle, &p);
	//点pと球の中心の差分ベクトル
	XMVECTOR v = p - sphere.center;
	//距離の二乗を求める
	//（同じベクトル同士の内積は三平方の定理のルート内部の式と一致する）
	v = XMVector3Dot(v, v);
	//球と三角形の距離が半径以下なら当たっていない
	if (v.m128_f32[0] > sphere.radius * sphere.radius) return false;
	//疑似交点を計算
	if (inter) {
		//三角形上の最近設定pを疑似交点とする
		*inter = p;
	}

	return true;
}


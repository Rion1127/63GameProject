#include "Vector3.h"
#include <list>
#include <memory>
#include "WorldTransform.h"
#include "Collision.h"

void Triangle::ComputeNormal()
{
	Vector3 p0_p1 = p1 - p0;
	Vector3 p0_p2 = p2 - p0;

	//外積により、2辺に垂直なベクトルを算出する
	normal = p0_p1.cross(p0_p2);
	normal = normal.normalize();
}

bool RayCollision(WorldTransform ray, WorldTransform obj)
{
	//レイの当たり判定

	//ワールド座標を代入
	Vector3 objPos;
	Vector3 rayPos;
	objPos = obj.position_;
	rayPos = ray.position_;
#pragma region レイの当たり判定
	//レイの始点と終点を代入
	Vector3 rayStart;
	Vector3 rayEnd;
	rayStart = {
		ray.position_.x,
		ray.position_.y,
		ray.position_.z - ray.scale_.z - 11.0f,
	};
	rayEnd = {
		ray.position_.x,
		ray.position_.y,
		ray.position_.z + ray.scale_.z + 11.0f,
	};
	//始点と終点からレイのベクトル(a→)を求める
	Vector3 rayVec;
	rayVec = rayEnd - rayStart;
	float raySize;
	raySize = rayVec.length();
	//正規化(a→N)
	rayVec = rayVec.normalize();
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
	if (line.length() <= obj.scale_.x + 4.3f)
	{
		if (raySize >= ray2ObjectLength)
		{
			isHit = true;
		}
	}

	if (ray2ObjectLength <= 0)
	{
		isHit = false;
	}

	return isHit;

#pragma endregion
}
//球と球
bool BallCollision(const WorldTransform& a, const WorldTransform& b) {
	float x, y, z;
	float r;

	x = (float)pow(b.position_.x - a.position_.x, 2);
	y = (float)pow(b.position_.y - a.position_.y, 2);
	z = (float)pow(b.position_.z - a.position_.z, 2);

	float pos = x + y + z;

	r = (float)pow(a.scale_.x + b.scale_.x, 2);
	if (pos <= r)
	{
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
	if (pos <= r)
	{
		return true;
	}
	return false;
}
//球と球
bool BallCollision(const Sphere& a, const Sphere& b)
{
	if (a.isActive == false ||
		b.isActive == false)
	{
		return false;
	}
	float x, y, z;
	float r;

	x = (float)pow(b.center.x - a.center.x, 2);
	y = (float)pow(b.center.y - a.center.y, 2);
	z = (float)pow(b.center.z - a.center.z, 2);

	float pos = x + y + z;

	r = (float)pow(a.radius + b.radius, 2);
	if (pos <= r)
	{
		return true;
	}
	return false;
}
//平面と球
bool Sphere2PlaneCol(const Sphere& sphere, const Plane& plane, Vector3* inter)
{
	if (sphere.isActive == false)
	{
		return false;
	}
	//座標系の原点から球の中心座標への距離
	float distV = sphere.center.dot(plane.normal);
	//平面の原点距離を減産することで、平面と球の中心との距離が出る
	float dist = distV - plane.distance;
	//距離の絶対値が半径より大きければ当たっていない
	if (fabsf(dist) > sphere.radius) return false;

	//疑似交点を計算
	if (inter)
	{
		//平面上の最近接点を、疑似交点とする
		*inter = -dist * plane.normal + sphere.center;
	}

	return true;
}
//点と三角形
void ClosestPtPoint2Triangle(const Vector3& point, const Triangle& triangle, Vector3* closest)
{
	// pointがp0の外側の頂点領域の中にあるかどうかチェック
	Vector3 p0_p1 = triangle.p1 - triangle.p0;
	Vector3 p0_p2 = triangle.p2 - triangle.p0;
	Vector3 p0_pt = point - triangle.p0;

	float d1 = p0_p1.dot(p0_pt);
	float d2 = p0_p2.dot(p0_pt);

	if (d1 <= 0.0f && d2 <= 0.0f)
	{
		// p0が最近傍
		*closest = triangle.p0;
		return;
	}

	// pointがp1の外側の頂点領域の中にあるかどうかチェック
	Vector3 p1_pt = point - triangle.p1;

	float d3 = p0_p1.dot(p1_pt);
	float d4 = p0_p2.dot(p1_pt);

	if (d3 >= 0.0f && d4 <= d3)
	{
		// p1が最近傍
		*closest = triangle.p1;
		return;
	}

	// pointがp0_p1の辺領域の中にあるかどうかチェックし、あればpointのp0_p1上に対する射影を返す
	float vc = d1 * d4 - d3 * d2;
	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
	{
		float v = d1 / (d1 - d3);
		*closest = triangle.p0 + v * p0_p1;
		return;
	}

	// pointがp2の外側の頂点領域の中にあるかどうかチェック
	Vector3 p2_pt = point - triangle.p2;

	float d5 = p0_p1.dot(p2_pt);
	float d6 = p0_p2.dot(p2_pt);
	if (d6 >= 0.0f && d5 <= d6)
	{
		*closest = triangle.p2;
		return;
	}

	// pointがp0_p2の辺領域の中にあるかどうかチェックし、あればpointのp0_p2上に対する射影を返す
	float vb = d5 * d2 - d1 * d6;
	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
	{
		float w = d2 / (d2 - d6);
		*closest = triangle.p0 + w * p0_p2;
		return;
	}

	// pointがp1_p2の辺領域の中にあるかどうかチェックし、あればpointのp1_p2上に対する射影を返す
	float va = d3 * d6 - d5 * d4;
	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
	{
		float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		*closest = triangle.p1 + w * (triangle.p2 - triangle.p1);
		return;
	}

	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	*closest = triangle.p0 + p0_p1 * v + p0_p2 * w;
}
//弾と法線付き三角形の当たり判定チェック
bool Sphere2TriangleCol(const Sphere& sphere, const Triangle& triangle, Vector3* inter)
{
	Vector3 p;
	//弾の中心に対する最近接点である三角形上にある点pを見つける
	ClosestPtPoint2Triangle(sphere.center, triangle, &p);
	//点pと球の中心の差分ベクトル
	Vector3 v = p - sphere.center;
	//距離の二乗を求める
	//（同じベクトル同士の内積は三平方の定理のルート内部の式と一致する）
	float d;
	d = v.dot(v);
	//球と三角形の距離が半径以下なら当たっていない
	if (d > sphere.radius * sphere.radius) return false;
	//疑似交点を計算
	if (inter)
	{
		//三角形上の最近設定pを疑似交点とする
		*inter = p;
	}

	return true;
}
//レイと平面
bool CheckRay2Plane(const Ray& ray, const Plane& plane, float* distance, Vector3* inter)
{
	const float epsilon = 1.0e-5f;//誤差吸収用の微小な値
	//面法線とレイの方向ベクトルの内積
	float d1 = plane.normal.dot(ray.dir);
	//裏面には当たらない
	if (d1 > -epsilon) { return false; }
	//始点と原点の距離（平面の法線方向）
	//面法線とレイの始点座標
	float d2 = plane.normal.dot(ray.start);
	//始点と平面の距離（平面の法線方向）
	float dist = d2 - plane.distance;
	//始点と平面の距離（レイ方向）
	float t = dist / -d1;
	//好天が視点より後ろにあるので、当たらない
	if (t < 0)return false;
	//距離を書き込む
	if (distance) { *distance = t; }
	//交点を計算
	if (inter) { *inter = ray.start + t * ray.dir; }
	return true;
}
//レイと三角形
bool CheckRay2Traiangle(const Ray& ray, const Triangle& triangle, float* distance, Vector3* inter)
{
	//三角形が乗っている平面を算出
	Plane plane;
	Vector3 interPlane;
	plane.normal = triangle.normal;
	plane.distance = triangle.normal.dot(triangle.p0);
	//レイと平面が当たっていなければ、当たっていない
	if (!CheckRay2Plane(ray, plane, distance, &interPlane)) { return false; }
	//レイと平面が当たっていたので、距離と交点が書き込まれた
	//レイと平面の好転が三角形の内側にあるか判定
	const float epsilon = 1.0e-5f;//誤差吸収用の微小な値
	Vector3 m;
	//辺p0_p1について
	Vector3 pt_p0 = triangle.p0 - interPlane;
	Vector3 p0_p1 = triangle.p1 - triangle.p0;
	m = pt_p0.cross(p0_p1);
	//辺の外側であれば当たっていないので判定を打ち切る
	if (m.dot(triangle.normal) < -epsilon) { return false; }

	Vector3 m2;
	//辺p1_p2について
	Vector3 pt_p1 = triangle.p1 - interPlane;
	Vector3 p1_p2 = triangle.p2 - triangle.p1;
	m2 = pt_p1.cross(p1_p2);
	//辺の外側であれば当たっていないので判定を打ち切る
	if (m2.dot(triangle.normal) < -epsilon) { return false; }

	//辺p2_p0について
	Vector3 pt_p2 = triangle.p2 - interPlane;
	Vector3 p2_p0 = triangle.p0 - triangle.p2;
	m = pt_p2.cross(p2_p0);
	//辺の外側であれば当たっていないので判定を打ち切る
	if (m.dot(triangle.normal) < -epsilon) { return false; }

	//内側なので、当たっている
	if (inter)
	{
		*inter = interPlane;
	}


	return true;
}
//レイと球
bool CheckRay2Sphere(const Ray& ray, const Sphere& sphere, float* distance, Vector3* inter)
{
	Vector3 m = ray.start - sphere.center;
	float b = m.dot(ray.dir);
	float c = m.dot(m) - sphere.radius * sphere.radius;
	//layの始点がsphereの外側にあり(c > 0)、layがsphereから離れていく方向を指している場合(b > 0)、当たらない
	if (c > 0.0f && b > 0.0f) { return false; }

	float discr = b * b - c;
	//負の判別式は例が急を外れていることに一致
	if (discr < 0.0f) { return false; }

	//レイは球と交差している
	//交差する最小の値tを計算
	float t = -b - sqrtf(discr);
	//tが負である場合、レイは級の内側から介しているのでtをゼロにクランプ
	if (t < 0)t = 0.0f;
	if (distance) { *distance = t; }

	if (inter) { *inter = ray.start + t * ray.dir; }

	return true;
}



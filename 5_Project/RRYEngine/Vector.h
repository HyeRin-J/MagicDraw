#pragma once

#define  PI  3.141592741f

#define ToDegree(rad)	((rad) / PI) * 180.0f
#define ToRadian(deg)	((deg) / 180.0f) * PI

struct double2
{
	union
	{
		struct
		{
			double x, y;
		};
		double f[2];
	};
	double2();
	double2(double _x, double _y);
};
struct Vector2 : double2
{
	static const Vector2 Zero2;
	static const Vector2 UnitX2;
	static const Vector2 UnitY2;

	Vector2();
	explicit Vector2(double x);
	Vector2(double x, double y);
	Vector2(double2& V);
	//Vector2(Vector2& F);

	//���� ������ 
	Vector2(const Vector2&) = default;
	Vector2(Vector2&&) = default;

	//������������ 
	Vector2& operator=(const Vector2&) = default;
	Vector2& operator=(Vector2&&) = default;
	bool operator== (Vector2& V);
	bool operator!= (Vector2& V);
	Vector2 operator+= (Vector2 V);
	Vector2 operator-= (Vector2 V);
	Vector2 operator*= (float S);
	Vector2 operator/= (float S);
	Vector2 operator+ (Vector2 V);
	Vector2 operator- (Vector2 V);
	Vector2 operator* (float S);
	Vector2 operator/ (float S);
	Vector2 operator+ ();
	Vector2 operator- (); //����Լ� 

	double Length();
	double LengthSquared();
	double Dot(const Vector2 V);
	static double Angle(Vector2 V1, Vector2 V2);
	Vector2 Normalize();
	void Normalize(Vector2 result);
	Vector2 Clamp(Vector2 min, Vector2 max);
	void Clamp(Vector2 min, Vector2 max, Vector2 ret); //static function 

	//static Vector2 operator* (double S, Vector2 V); 
	//static Vector2 operator/ (double S, Vector2 V); 

	static double Dot(Vector2 v1, Vector2 v2);
	static double Distance(Vector2 v1, Vector2 v2);
	static double DistanceSquared(Vector2 v1, Vector2 v2);
	static Vector2 Min(Vector2 v1, Vector2 v2);
	static Vector2 Max(Vector2 v1, Vector2 v2);
	static Vector2 Lerp(Vector2 v1, Vector2 v2, double t);

	//���� ������
	static Vector2 SmoothStep(Vector2 v1, Vector2 v2, double t); //����������. (�ʿ��Ѱ�..?) 
	static Vector2 Reflect(Vector2 iV, Vector2 nV); //iV:�ݻ��ų ����, nV:����� �븻����. 
	static Vector2 Refract(Vector2 iV, Vector2 nV, double rI); //iV:������ų ����, nV:����� �븻����. rI:������ 
};
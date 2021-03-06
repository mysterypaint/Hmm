#pragma once
#include <iostream>

class Vector2D {
public:
	float x;
	float y;

	Vector2D();
	Vector2D(float _x, float _y);

	Vector2D& Add(const Vector2D& _vec);
	Vector2D& Subtract(const Vector2D& _vec);
	Vector2D& Multiply(const Vector2D& _vec);
	Vector2D& Divide(const Vector2D& _vec);

	friend Vector2D& operator+(Vector2D& _v1, const Vector2D& _v2);
	friend Vector2D& operator-(Vector2D& _v1, const Vector2D& _v2);
	friend Vector2D& operator*(Vector2D& _v1, const Vector2D& _v2);
	friend Vector2D& operator/(Vector2D& _v1, const Vector2D& _v2);

	Vector2D& operator+=(const Vector2D& _vec);
	Vector2D& operator-=(const Vector2D& _vec);
	Vector2D& operator*=(const Vector2D& _vec);
	Vector2D& operator/=(const Vector2D& _vec);

	Vector2D& operator*(const int& _i);
	Vector2D& Zero();

	friend std::ostream& operator<<(std::ostream& _stream, const Vector2D& _vec);
};
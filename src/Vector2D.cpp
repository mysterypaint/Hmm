#include "Vector2D.hpp"

Vector2D::Vector2D() {
	x = 0.0f;
	y = 0.0f;
}

Vector2D::Vector2D(float _x, float _y) {
	this->x = _x;
	this->y = _y;
}

Vector2D& Vector2D::Add(const Vector2D& _vec) {
	this->x += _vec.x;
	this->y += _vec.y;
	return *this;
}

Vector2D& Vector2D::Subtract(const Vector2D& _vec) {
	this->x -= _vec.x;
	this->y -= _vec.y;
	return *this;
}

Vector2D& Vector2D::Multiply(const Vector2D& _vec) {
	this->x *= _vec.x;
	this->y *= _vec.y;
	return *this;
}

Vector2D& Vector2D::Divide(const Vector2D& _vec) {
	this->x /= _vec.x;
	this->y /= _vec.y;
	return *this;
}

Vector2D& operator+(Vector2D& _v1, const Vector2D& _v2) {
	return _v1.Add(_v2);
}

Vector2D& operator-(Vector2D& _v1, const Vector2D& _v2) {
	return _v1.Subtract(_v2);
}

Vector2D& operator*(Vector2D& _v1, const Vector2D& _v2) {
	return _v1.Multiply(_v2);
}

Vector2D& operator/(Vector2D& _v1, const Vector2D& _v2) {
	return _v1.Divide(_v2);
}

Vector2D& Vector2D::operator+=(const Vector2D& _vec) {
	return this->Add(_vec);
}

Vector2D& Vector2D::operator-=(const Vector2D& _vec) {
	return this->Subtract(_vec);
}

Vector2D& Vector2D::operator*=(const Vector2D& _vec) {
	return this->Multiply(_vec);
}

Vector2D& Vector2D::operator/=(const Vector2D& _vec) {
	return this->Divide(_vec);
}

Vector2D& Vector2D::operator*(const int& _i) {
	this->x *= _i;
	this->y *= _i;
	
	return *this;
}

Vector2D& Vector2D::Zero() {
	this->x = 0;
	this->y = 0;

	return *this;
}

std::ostream& operator<<(std::ostream& _stream, const Vector2D& _vec) {
	_stream << "(" << _vec.x << ", " << _vec.y << ")";
	return _stream;
}
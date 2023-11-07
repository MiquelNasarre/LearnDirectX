#pragma once

struct _float4color {
	float r;
	float g;
	float b;
	float a;
};

struct Color {
	unsigned char B;
	unsigned char G;
	unsigned char R;
	unsigned char A;

	Color();
	Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255u);

	static const Color Black;
	static const Color White;
	static const Color Red;
	static const Color Green;
	static const Color Blue;
	static const Color Yellow;
	static const Color Cyan;
	static const Color Purple;
	static const Color Gray;
	static const Color Orange;
	static const Color Transparent;

	Color operator+(const Color& other) const;
	Color operator-(const Color& other) const;
	Color operator*(const Color& other) const;
	Color operator/(const Color& other) const;
	Color operator*(const int& rhs) const;
	Color operator/(const int& rhs) const;
	Color operator*(const float& rhs) const;
	Color operator/(const float& rhs) const;
	Color operator*(const double& rhs) const;
	Color operator/(const double& rhs) const;
	Color operator-() const;
	Color& operator+=(const Color& other);
	Color& operator-=(const Color& other);
	Color& operator*=(const Color& other);
	Color& operator/=(const Color& other);
	Color& operator*=(const int& other);
	Color& operator/=(const int& other);
	Color& operator*=(const float& other);
	Color& operator/=(const float& other);
	Color& operator*=(const double& other);
	Color& operator/=(const double& other);
	bool operator==(const Color& other) const;
	bool operator!=(const Color& other) const;

	_float4color getColor4() const;
};

Color operator*(const int& rhs, const Color& lhs);
Color operator*(const float& rhs, const Color& lhs);
Color operator*(const double& rhs, const Color& lhs);
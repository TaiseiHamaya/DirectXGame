#pragma once

class Color final {
public:
	Color();
	Color(unsigned int hex);
	Color(unsigned int _red, unsigned int _green, unsigned int _blue, float _alpha);
	Color(float _red, float _green, float _blue, float _alpha);

public:
	float red;
	float green;
	float blue;
	float alpha;

public:
	const unsigned int hex() const;
#ifdef _DEBUG
	void debug_gui();
#endif // _DEBUG

public:
	static unsigned int ToHex(unsigned int red, unsigned int green, unsigned int blue, unsigned int alpha);
	static unsigned int LerpC(unsigned int hex1, unsigned int hex2, float t);
	static Color LerpC(const Color& color1, const Color& color2, float t);
};
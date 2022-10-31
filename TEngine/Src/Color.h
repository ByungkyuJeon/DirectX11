#pragma once
typedef unsigned char BYTE;

class Color
{
public:
	Color();
	Color(unsigned int val);
	Color(BYTE r, BYTE g, BYTE b);
	Color(BYTE r, BYTE g, BYTE b, BYTE a);
	Color(const Color& other);

	Color& operator=(const Color& other);
	bool operator==(const Color& other) const;
	bool operator!=(const Color& other) const;

	BYTE GetR() const;
	void SetR(BYTE r);

	BYTE GetG() const;
	void SetG(BYTE g);

	BYTE GetB() const;
	void SetB(BYTE b);

	BYTE GetA() const;
	void SetA(BYTE a);

private:
	union
	{
		BYTE rgba[4];
		unsigned int color;
	};
};

namespace Colors
{
	const Color UnloadedTextureColor(100, 100, 100);
	const Color UnhandledTextureColor(250, 0, 0);
}
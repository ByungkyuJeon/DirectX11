#pragma once

class Grid
{
public:
	Grid();

	void enableGrid();
	void disableGrid();
	bool getGridState();

private:
	void Draw();

	bool gridState = false;
};
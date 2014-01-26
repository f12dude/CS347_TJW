#ifndef _GRID_
#define _GRID_

#include <string>
#include <vector>
#include <list>

enum Direction
{
	Up,
	Down,
	Left,
	Right,
};

struct ivec2
{
	int x;
	int y;
};

struct uvec2
{
	unsigned int x;
	unsigned int y;
};

struct Wriggler
{
	std::list<uvec2> positons;
	unsigned int id;
};

// Defines a grid for TJ-wriggle puzzles which stores the location of all the objects
class Grid
{
public:

	// Builds an empty grid
	Grid();

	// Loads puzzle description from the specified file
	Grid(const std::string& file);

	virtual ~Grid() {}

	bool MoveWriggler(unsigned int id, Direction dir, bool bHead);

	// Loads puzzle description from the specified file
	// The old state of the grid is discared
	bool Load(const std::string& file);

	// Returns the number of wrigglers on the grid
	unsigned int GetNumWrigglers() const;

	// Returns the width of the grid
	unsigned int GetWidth() const;

	// Returns the height of the grid
	unsigned int GetHeight() const;

	// todo: make this pure virtual
	virtual void RunAI() {}

protected:

	std::vector<std::vector<char>> m_grid;
	std::vector<Wriggler> m_wrigglers;
	unsigned int m_uiWidth;
	unsigned int m_uiHeight;

	static const ivec2 s_adjacentTiles[];

	Grid(std::istream& stream);
	bool Load(std::istream& stream);
	ivec2 GetDir(char) const;
	bool IsHead(char) const;
	bool IsTail(char) const;
	bool IsWriggler(char) const;

	bool IsOpen(const uvec2& pos) const;

private:
	void ClearPos(const uvec2& pos);
	void FlipTile(const uvec2& pos);
	void SetExternalPos(const uvec2& pos, int id);
	void SetFlipedExternalPos(const uvec2& newPos, Direction dir);
	void SetExternalPos(const uvec2& newPos, Direction dir);
	void SetInternalPos(const uvec2& pos, Direction dir);

	friend std::ostream& operator <<(std::ostream& stream, const Grid&);
	friend std::istream& operator >>(std::istream& stream, Grid&);
};

std::ostream& operator <<(std::ostream& stream, const Grid&);
std::istream& operator >>(std::istream& stream, Grid&);

#endif // _GRID_

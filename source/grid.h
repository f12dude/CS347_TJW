#ifndef _GRID_
#define _GRID_

#include <string>
#include <vector>
#include <deque>

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

bool operator==(const uvec2& a, const uvec2& b);
bool operator!=(const uvec2& a, const uvec2& b);

struct Wriggler
{
	std::deque<uvec2> positons;
	unsigned int id;
};

bool operator==(const Wriggler& a, const Wriggler& b);
bool operator!=(const Wriggler& a, const Wriggler& b);

// Defines a hash function for a wriggler
class WrigglerHash
{
public:

	std::size_t operator()(const Wriggler& a) const
	{
		const unsigned int p1 = 73856093;
		const unsigned int p2 = 83492791;
		std::size_t h = 1427;
		for(auto iter : a.positons)
		{
			h += ((iter.x * p1) ^ (iter.y * p2));
		}

		h += std::hash<unsigned int>()(a.id);

		return h;
	}
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

	// Loads puzzle description from the specified file
	// The old state of the grid is discared
	bool Load(const std::string& file);

	// Returns true if the wriggler at index id can move from the head specified by bHead in the direction specified by dir
	// id: id of the wriggler
	// bHead: true if moving the head, false if moving the tail
	// dir: the direction that the wriggler will move in
	bool CanMoveWriggler(unsigned int id, bool bHead, Direction dir) const;

	// Moves a wriggler. Returns true if the move is valid, false otherwise
	// id: id of the wriggler
	// bHead: true if moving the head, false if moving the  tail.
	// dir: the direction that the wriggler will move in
	bool MoveWriggler(unsigned int id, bool bHead, Direction dir);

	// Returns the number of wrigglers on the grid
	unsigned int GetNumWrigglers() const;

	// Returns the width of the grid
	unsigned int GetWidth() const;

	// Returns the height of the grid
	unsigned int GetHeight() const;

	// This method should do all the work to solve the puzzle
	virtual void RunAI() = 0;

protected:

	// Grid containing all of the tiles
	std::vector<std::vector<char>> m_grid;

	// List containing all of the wriglers
	std::vector<Wriggler> m_wrigglers;

	// Width and height of the grid
	unsigned int m_uiWidth;
	unsigned int m_uiHeight;

	static const ivec2 s_adjacentTiles[];

	// Constructs a grid from the specified stream and returns true if successful, false otherwise
	Grid(std::istream& stream);

	// Loads the grid from the specified stream and returns true if successful, false otherwise
	bool Load(std::istream& stream);

	// Returns the direction that the wriggler will move away from, the opposite side of the wriggler that is set to move
	// w: index of the wriggler
	// bHead: true to move the head, false to move the tail
	Direction GetGetWrigglerTailDir(unsigned int w, bool bHead);

	// Returns the direction specified by the current tile character c in array space, which can either be 'U', '^', 'D', 'v', 'L', '<', 'R', '>'
	ivec2 GetDirVector(char c) const;

	// Returns the direction specified by the current tile character c,  which can either be 'U', '^', 'D', 'v', 'L', '<', 'R', '>'
	Direction GetDir(char c) const;

	// Returns true if c is the head of a wriggler, false otherwise
	bool IsHead(char c) const;

	// Returns true is c is the tail of a wriggler, false otherwise
	bool IsTail(char c) const;

	// Returns true if c is part of a wriggler(head, tail, or internal section), false otherwise
	bool IsWriggler(char c) const;

	// Returns true is pos is a valid position to move to, false otherwise
	bool IsValid(const uvec2& pos) const;

private:

	// Sets the position specified by pos to empty
	void ClearPos(const uvec2& pos);

	// Inverts the direction at the specified positon in the grid
	// 'U' <--> '^'
	// 'D' <--> 'v'
	// 'L' <--> '<'
	// 'R' <--> '>'
	void FlipTileDirection(const uvec2& pos);


	// Sets the tail of the wriggler
	// pos: the position of the wriggler's tail
	// id: id of the wriggler
	void SetWrigglerTail(const uvec2& pos, unsigned int id);

	// Sets the direction of the head of the wriggler
	// pos: the position of the head
	// dir: the direction that the head should be facing
	void SetWrigglerHead(const uvec2& pos, Direction dir);

	// Sets the direction of a specific part of the wriggler
	// pos: the position of the wriggler to update the direction
	// dir: direction of the wriggler
	void SetWrigglerDirection(const uvec2& pos, Direction dir);

	// Grid I/O
	friend std::ostream& operator <<(std::ostream& stream, const Grid&);
	friend std::istream& operator >>(std::istream& stream, Grid&);
};

std::ostream& operator <<(std::ostream& stream, const Grid&);
std::istream& operator >>(std::istream& stream, Grid&);

#endif // _GRID_

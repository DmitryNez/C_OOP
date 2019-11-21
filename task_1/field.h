#pragma once
#include <vector>
#include "cell.h" 

class field
{
public:
	field();
	~field();
	std::vector<std::vector<cell>> field_of_cells;
	size_t get_size_x() const;
	size_t get_size_y() const;
	void set_size(size_t&, size_t&);
private:
	size_t size_x;
	size_t size_y;
};


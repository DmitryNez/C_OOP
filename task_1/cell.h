#pragma once
enum class state { alive, empty };

class cell
{
private:
	//size_t x;
	//size_t y;
	state status;
public:
	cell();
	~cell();
	void set_status(state&);
	state get_status() const;
};


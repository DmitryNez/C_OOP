#pragma once
#include "RNA.h"
class DNA
{
private:
	size_t size;
	std::pair<RNA, RNA> dnk;
public:
	DNA(const RNA& , const RNA&);
};


#pragma once
#include <unordered_map>
#include <exception>
#include <utility>
#include <bitset>
#include "reference.h"

class reference;

class RNA
{
private:
	size_t* NuclArray;
	size_t size;
	size_t capacity;
	size_t RealSize; // кол-во блоков размером size_t
	friend class reference;
public:

	RNA();
	RNA(Nucleotide,size_t);
	RNA(const RNA &);
	RNA(RNA &&);

	size_t* TrueAlloc(size_t);
	size_t get_capacity() const;
	size_t get_real_size() const;
	void push_back(const Nucleotide&);
	size_t cardinality(Nucleotide) const;
	std::unordered_map<Nucleotide,int,std::hash<int>> cardinality() const;
	void trim(size_t);
	size_t length() const;
	bool isComplementary(const RNA&) const;
	std::pair<RNA, RNA> split(size_t index);

	reference operator[](size_t);
	const reference operator[](size_t) const;
	friend RNA operator + (const RNA& r1, const RNA& r2);
	bool operator == (const RNA &) const;
	void operator ! ();
	bool operator != (const RNA &) const;
	RNA& operator = (const RNA&);
	RNA& operator = (RNA&&) noexcept;
	//template <typename T>
	//RNA& operator=(T &&);

	void print();
	~RNA();

	size_t Size_tPerNucl = (sizeof(size_t) * 8) / 2; // кол-во нуклеотидов в одном size_t
};

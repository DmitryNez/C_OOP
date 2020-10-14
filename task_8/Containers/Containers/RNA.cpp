#include <iostream> 
#include <utility> 
#include "RNA.h"

size_t * RNA::TrueAlloc(size_t size) {
	RealSize = ceil(size / (1.0 * Size_tPerNucl)); 

	size_t * NewNuclArray = new size_t[RealSize];

	if (NewNuclArray == nullptr) {
		throw std::bad_alloc();
	} 
	else {
		for (size_t i = 0; i < RealSize; i++) { // initial initialization
			NewNuclArray[i] = 0;
		}
		return NewNuclArray;
	}
}

/*------Constructors-------*/
RNA::RNA():size(0),capacity(16){
	NuclArray = TrueAlloc(1);
}

RNA::RNA(Nucleotide nucl,size_t amount): size(amount) {

	NuclArray = TrueAlloc(size);

	size_t step = 0;


	if (size <= 16) {
		capacity = 16;
	}
	else {
		capacity = ceil(1.0 * size / 16) * 16;
	}

	for (size_t i = 0; i < size; i++) {
		if (step == Size_tPerNucl) {
			step = 0;
		}
		NuclArray[i / Size_tPerNucl] = NuclArray[i / Size_tPerNucl] | (nucl << 2 * (Size_tPerNucl - step - 1));
		step++;
	}

 }

RNA::RNA(const RNA& rna) {
	if (rna.NuclArray != nullptr) {
		this->RealSize = rna.RealSize;
		this->capacity = rna.capacity;
		this->size = rna.size;
		NuclArray = TrueAlloc(size);
		memcpy(NuclArray, rna.NuclArray, RealSize * sizeof(size_t));
	}
	else {
		NuclArray = nullptr;
	}
}

RNA::RNA(RNA&& rna) {
	this->capacity = rna.capacity;
	this->size = rna.size;
	this->RealSize = rna.RealSize;
	this->NuclArray = rna.NuclArray;
	rna.NuclArray = nullptr;
}


/*------Operators---------*/
reference RNA::operator[](size_t number) {
	if (number >= size || number < 0) {
		throw std::out_of_range("Invalid index");
	}
	reference ref(this, number);
	return ref;
}

const reference RNA::operator[](size_t number) const {
	if (number >= size || number < 0) {
		throw std::out_of_range("Invalid index");
	}
	reference ref(const_cast<RNA*>(this), number);
	return ref;
}

RNA& RNA::operator=(const RNA& rna) {
	if (this == &rna) {
		return *this;
	}
	if (rna.NuclArray != nullptr) {
		this->RealSize = rna.RealSize;
		this->capacity = rna.capacity;
		this->size = rna.size;
		NuclArray = TrueAlloc(size);
		memcpy(NuclArray, rna.NuclArray, RealSize * sizeof(size_t));
	}
	else {
		NuclArray = nullptr;
	}
	return *this;
}

RNA& RNA::operator=(RNA&& rna) noexcept {
	if (this == &rna) {
		return *this;
	}
	this->capacity = rna.capacity;
	this->size = rna.size;
	this->RealSize = rna.RealSize;
	this->NuclArray = rna.NuclArray;
	rna.NuclArray = nullptr;
	return *this;
}

bool RNA::operator== (const RNA& rna) const {
	if (size != rna.size) {
		return false;
	}
	for (int i = 0; i < size; i++) {
		if ((Nucleotide)rna[i] != (Nucleotide)(*this)[i]) {
			return false;
		}
	}
	return true;
}

RNA operator+ (const RNA& r1, const RNA& r2) {

	Nucleotide x, y;
	x = r1[r1.size - 1];
	y = r2[0];

	if ((x ^ 3) == y) {
		RNA newRNA(r1);
		size_t ContainerIndex, step = 0, nucleotid;
		for (int i = 0; i < r2.size; i++) {
			ContainerIndex = i / r2.Size_tPerNucl;
			step = (ContainerIndex + 1) * sizeof(size_t) * 8 - (2 * i) % (sizeof(size_t) * 8) - 2;
			nucleotid = (r2.NuclArray[ContainerIndex] >> step) & 3;
			newRNA.push_back(static_cast<Nucleotide>(nucleotid));
		}
		return newRNA;
	}
	return RNA();
}

void RNA::operator!() {
	if (this == nullptr) {
		throw std::runtime_error("RNA is empty");
	}
	else {
		for (int i = 0; i < this->size; i++) {
			(*this)[i] = static_cast<Nucleotide>((*this)[i] ^ 3);
		}
	}
}

bool RNA::operator != (const RNA & rna) const {
	if (size != rna.size) {
		return true;
	}
	for (int i = 0; i < size; i++) {
		if ((Nucleotide)rna[i] != (Nucleotide)(*this)[i]) {
			return true;
		}
	}
	return false;
}

//template <typename T>
//RNA& RNA::operator=(T&& rna) {
//	if (this == &rna) {
//		return *this;
//	}
//	RNA NewRNA(std::forward<T>(rna));
//	return NewRNA;
//}

/*------------Methods------------*/
size_t RNA::length() const {
	return this->size;
}

size_t RNA::get_capacity() const {
	return this->capacity;
}

size_t RNA::get_real_size() const {
	return this->RealSize * sizeof(size_t);
}

size_t RNA::cardinality(Nucleotide nucl) const {
	size_t nucl_counter = 0, ContainerIndex = 0, nucleotid, step;
	for (int i = 0; i < size; i++) {
		nucleotid = this->operator[](i);
		if (nucl == static_cast<Nucleotide>(nucleotid)) {
			nucl_counter++;
		}
	}
	return nucl_counter;
}

std::unordered_map<Nucleotide, int, std::hash<int>> RNA::cardinality() const{
	std::unordered_map<Nucleotide, int, std::hash<int>> map;
	for (int i = 0; i < size; i++) {
		map[(*this)[i]]++;
	}
	return map;
}

void RNA::push_back(const Nucleotide & nucl) {
	if (size == capacity) {
		capacity *= 2;
		size_t* tmp = TrueAlloc(capacity);
		memcpy(tmp, NuclArray, size * 2 / 8);
		delete[] NuclArray;
		NuclArray = tmp;
		tmp = nullptr;
		size_t ContainerIndex = size / Size_tPerNucl;
		NuclArray[ContainerIndex] = NuclArray[ContainerIndex] | (nucl << sizeof(size_t) * 8 - 2);
		size++;
	}
	else {
		size_t AmountOfNotEmptyCells = 2 * (size % Size_tPerNucl);
		size_t ContainerIndex = size / Size_tPerNucl;
		NuclArray[ContainerIndex] = NuclArray[ContainerIndex] | (nucl << sizeof(size_t) * 8 - AmountOfNotEmptyCells - 2);
		size++;
	}
}

void RNA::print() {
	for (size_t i = 0; i < ceil(size/16.0); i++) {
		//std::string str = std::bitset<sizeof(NuclArray[i])* CHAR_BIT>(NuclArray[i]);
		std::cout << std::bitset<sizeof(NuclArray[i])* CHAR_BIT>(NuclArray[i]) << std::endl;
	}

}

bool RNA::isComplementary(const RNA & rna) const {
	if (this->size != rna.size) {
		return false;
	}
	else {
		for (int i = 0; i < this->size; i++) {
			if ((*this)[i] != static_cast<Nucleotide>(rna[i] ^ 3)) {
				return false;
			}
		}
	}
	return true;
}

void RNA::trim(size_t LastIndex) {
	if (LastIndex >= 0 && LastIndex < size) {
		size = LastIndex;
		capacity = LastIndex;
		size_t* tmp = TrueAlloc(size);
		memcpy(tmp, this->NuclArray, this->RealSize * sizeof(size_t));
		delete[] this->NuclArray;
		this->NuclArray = tmp;
		tmp = nullptr;
	}
}

std::pair<RNA, RNA> RNA::split(size_t index) {
	RNA r1, r2;
	if (index >= this->length() || index < 0) {
		throw std::runtime_error("Invalid index");
	} 
	for (int i = 0; i < size; i++) {
		if (i <= index) {
			r1.push_back((*this)[i]);
		}
		else {
			r2.push_back((*this)[i]);
		}
	}
	std::pair<RNA, RNA> two_rna;
	two_rna = std::make_pair(r1, r2);
	return two_rna;
}

RNA::~RNA() {
	delete[] NuclArray;
}


#include "reference.h"
#include "RNA.h"
reference::reference(RNA* rna,size_t num) {
	this->rna = rna;
	this->num = num;
}

reference::operator Nucleotide() const {
	size_t nucl;
	size_t step = sizeof(size_t) * 8 - (2 * num) % (sizeof(size_t) * 8) - 2;
	nucl = (rna->NuclArray[num / rna->Size_tPerNucl] >> step) & 3;
	Nucleotide current_nucl = static_cast<Nucleotide>(nucl);
	return current_nucl;
}

reference& reference::operator=(Nucleotide nucl) {
	size_t step = sizeof(size_t) * 8 - (2 * num) % (sizeof(size_t) * 8) - 1;
	std::bitset<sizeof(size_t) * 8> bytes(nucl);
	if (bytes[0] == 0) {
		rna->NuclArray[num / rna->Size_tPerNucl] &= ~(1 << (step - 1));
	}
	else {
		rna->NuclArray[num / rna->Size_tPerNucl] |= (1 << (step - 1));
	}
	if (bytes[1] == 0) {
		rna->NuclArray[num / rna->Size_tPerNucl] &= ~(1 << step);
	}
	else {
		rna->NuclArray[num / rna->Size_tPerNucl] |= (1 << step);
	}

	return *this;
}

reference& reference::operator=(const reference ref) {
	rna->operator[](num).operator=((Nucleotide)ref);
	return *this;
}
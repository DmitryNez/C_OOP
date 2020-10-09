#include "DNA.h"
DNA::DNA(const RNA& first_rna, const RNA& second_rna) {
	if (first_rna.isComplementary(second_rna)) {
		dnk = std::make_pair(first_rna, second_rna);
	}
}
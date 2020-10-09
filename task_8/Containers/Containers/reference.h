#pragma once
enum Nucleotide { A, G, C, U };

class RNA;

class reference {
private:
	RNA* rna;
	size_t num;
public:
	reference(RNA* rna, size_t num);
	operator Nucleotide() const;
	reference& operator=(const reference);
	reference& operator=(Nucleotide);
};


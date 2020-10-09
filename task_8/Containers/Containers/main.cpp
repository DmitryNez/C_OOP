#include <iostream>
#include "RNA.h"

using namespace std;

int main(int argc, char* argv[]) {
	RNA rna(A, 10);
	for (int i = 0; i < 1000; i++) {
		rna.push_back(U);
	}
	rna.trim(0);
	return 0;
}
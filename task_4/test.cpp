
#include <iostream>
#include "CSVParser.h"

int main(int argc, char* argv[])
{
    std::ifstream in("data.txt");
    testing::InitGoogleTest(&argc, argv);

    CSVParser<string, double> parser(in, 0);

    for (auto tuple : parser) {
        cout << tuple << endl;
    }
    return RUN_ALL_TESTS();
}



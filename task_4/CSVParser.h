#pragma once
#include "CSV_iterator.h"
#include <vector>
#include <fstream>
#include "StringToT.h"
#include "../packages/Microsoft.googletest.v140.windesktop.msvcstl.static.rt-dyn.1.8.1/build/native/include/gtest/gtest.h"
using namespace std;

template<typename...Args>
class CSVParser {
private:
    ifstream& input;
    unsigned int skip_num;
    unsigned int count_ = 1;
    char delimetr;
    char shield;
    vector<string> row;
    vector<tuple<Args...>> storage;
    string str;
public:

    CSVParser(ifstream& in, unsigned int num) : input(in), skip_num(num){
        delimetr = ',';
        shield = '"';
        for (unsigned int i = 0; i < skip_num; i++) {
            static const int max_line = 65536;
            input.ignore(max_line, '\n');
        }
        FillingStorage();
    }

    void FillingStorage() {
        while (!input.eof()) {
            row = csv_read_row(input);
            if (sizeof...(Args) != row.size()) {
                throw runtime_error("Number of tuple's arguments and number of file's row agruments are not equal ");
            }
            count_++;
            auto tmp = Call();
            storage.push_back(gen_tuple(tmp));
        }
    }

    void SetDelimetr(const char& NewDelimetr){
        if (NewDelimetr == shield) {
            throw runtime_error("Delimetr and shield do not be same! ");
        }
        else {
            delimetr = NewDelimetr;
        }
    }

    void SetShield(const char& NewShield) {
        if (NewShield == delimetr) {
            throw runtime_error("Delimetr and shield do not be same! ");
        }
        else {
            shield = NewShield;
        }
    }

    template <class TCall, size_t... Is>
    auto gen_tuple_impl(TCall& callback, std::index_sequence<Is...>) {
        return std::make_tuple(callback.operator() < Args > (row[Is])...);
    }

    template <class TCall>
    auto gen_tuple(TCall& callback) {
        return gen_tuple_impl(callback, make_index_sequence<sizeof...(Args)>{});
    }

    vector<string> csv_read_row(ifstream& input) {

        bool inquotes = false;
        stringstream ss;
        vector<string> row;
        unsigned int column = 1;
        while (!input.eof()) {

            char tmp = input.get();
            column++;

            if (tmp == shield && !inquotes) {
                inquotes = true;
                ss << tmp;
            }

            else if (tmp == shield && inquotes) {

                if (input.peek() == delimetr || input.peek() == '\n' || input.peek() == -1) {
                    inquotes = false;
                    ss << tmp;
                }
                else {
                    stringstream message;
                    message << "Error: ";
                    message << "'" << tmp << "'" << " is detected in the ";
                    message << count_ << " row and ";
                    message << column << " column";
                    static const int max_line = 65536;
                    input.ignore(max_line, '\n');
                    throw runtime_error(message.str());
                }

            }
            else if (!inquotes && tmp == delimetr) {
                row.push_back(ss.str());
                ss.str("");
            }
            else if (!inquotes && tmp == '\n') {
                row.push_back(ss.str());
                return row;
            }
            else if(tmp != -1)
            {
                ss << tmp;
            }

        }
        row.push_back(ss.str());

        return row;
    }

    Iterator<Args...> begin() {
        
        if (storage.empty()) {
            throw runtime_error("Storage is empty");
        }
        return &storage[0];
    }

    Iterator<Args...> end() {
        return &storage[0] + storage.size();
    }

};

template<typename Tuple, size_t... Is>
void PrintTuple(ostream& output, Tuple& tuple_, index_sequence<Is...>) {
    ((output << get<Is>(tuple_) << " "), ...);
}

template<typename...Args>
ostream& operator << (ostream& output, const tuple<Args...>& tuple_) {
    PrintTuple(output, tuple_, make_index_sequence<sizeof...(Args)>{});
    return output;
}

TEST(CSVParserTest, OpenFileTest) {
    std::ifstream input("data.txt");
    ASSERT_TRUE(input);
}
TEST(CSVParserTest, NoExceptionTest) {
    std::ifstream input("data.txt");
    CSVParser<string,double> test(input,0);
    ASSERT_NO_THROW(test.FillingStorage());
    ASSERT_NO_THROW({
        for (std::tuple<std::string, double> rs : test) {}
    });
}
TEST(CSVParserTest, AnyExceptionTest) {
    std::ifstream input("data.txt");
    ASSERT_ANY_THROW(
        {
            CSVParser<string> test(input, 0);
            for (std::tuple<std::string> rs : test) {}
        }
    );
}


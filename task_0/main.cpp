
#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <string>
#include <algorithm>
#include <iomanip>

using namespace std;


void Map_filling(map<string, int> &, const string &);

void Map_into_vector(map<string, int> &, vector<pair<string, int>> &);

void Print_in_CSV(vector<pair<string, int>> &, const string &);



int main(int argc, char * argv[])
{
    map<string, int> m;
    vector<pair<string, int>> v;
    if (argc != 3){
        cout << "Amount of argc is wrong" << endl;
        return -1;
    }
    Map_filling(m, argv[1]);
    Map_into_vector(m, v);
    Print_in_CSV(v, argv[2]);

    return 0;
}

void Map_filling(map<string, int> & initial_map, const string & name_of_input) {
    ifstream in(name_of_input);
    vector<int> p;
    string s;
    while (in >> s) {
        for (string::iterator it = s.begin(); it != s.end(); ++it) {
            if ((*it < 48) || (*it > 57 && *it < 65) || (*it > 90 && *it < 97) || (*it > 122)) {
                p.push_back(it - s.begin());
            }
            else if (*it >= 65 && *it <= 90) {
                *it += 32;
            }
        }
        for (size_t i = 0; i < p.size(); ++i) {
            s.erase(p[i] - i, 1);
        }
        initial_map[s]++;
        p.erase(p.begin(), p.end());
    }
}
void Map_into_vector(map<string, int> & initial_map, vector<pair<string, int>> & Reverse) {
    for (map<string, int>::iterator it = initial_map.begin(); it != initial_map.end(); ++it) {
        Reverse.push_back(*it);
    }
    initial_map.erase(initial_map.begin(), initial_map.end());
    sort(Reverse.begin(), Reverse.end(), [](const pair<string, int> & one, const pair<string, int> & two) {
        return one.second > two.second;
    });
}
void Print_in_CSV(vector<pair<string, int>> & Reverse, const string & name_of_csv) {
    ofstream fout(name_of_csv);
    for (auto i : Reverse) {

        fout << i.first << ';' << i.second << ';' <<
             (static_cast<float>(i.second) / Reverse.size()) * 100 << "%" << endl;

    }
}


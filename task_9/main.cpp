#include <iostream>
#include "HashMap.cpp"
#include <unordered_map>

using namespace std;

int main() {
	HashMap<int, string> map;
	string str("dsfd");
	HashMap<int, string> map1 { make_pair(1, "first"), make_pair(2, "second"), make_pair(3, "third") };
	const HashMap<int, string> map2(move(map1));
	//map = map1;
	//HashMap<int, string> mapa;
	//cout << map1.size() << endl;
	//HashMap<int, std::string> map{ std::make_pair(1, "first"), std::make_pair(2, "second"), std::make_pair(3, "third") };
	//for (int i = 4; i < 14; ++i) {
	//	map.insert(std::make_pair(i, "TestString"));
	//}
	//mapa[1] = str;
	//mapa.insert(1,"io");
	//pair<int, string> pai = { 3, "first" };
	//mapa.insert(pai);
	//int i = 0;
	//HashMap<int, string>::const_iterator it(map.find(3));
	//auto hz = map1.get(2);
	//cout << hz.has_value() << endl;
	//cout << (*it).first << ' ' << (*it).second << endl;
	//mapa[1] = "hsdf";
	//auto pair = map.extract(it);
	//cout << pair.first << ' ' << pair.second << endl;
	//auto it = map.emplace(1, "sad");
	//cout << it.first << " " << it.second << endl;
	//int i = 0;
	//for (;i<=10;) {
	//	if (i == 5) {
	//		mapa.insert(i, "sobaki");
	//	}
	//	else {
	//		mapa.insert(i, str);
	//	}
	//	i++;
	//}
	//cout << mapa[5] << endl;
	//cout << mapa.size();
	//int i = 0;
	//map.swap(map1);
	//map.insert(1, "first");
	//map.insert(2, "second");
	//HashMap<int, string>::iterator it = map.find(3);
	//cout << (*it).first << ' ' << (*it).second << endl;
	for (int i = 0; i < 1000000; i++) {
		map.insert(i, "first");
	}
	//map.swap(map1);
	//for (auto& it : map) {
	//	cout << it.first << ' ' << it.second << endl;
	//}
	//int i = 3;
	//map1[i] = map[3];
	//map[3] = map1[5];
	/*for (auto& it : map) {
		cout << it.first << ' ' << it.second << endl;
	}*/
}
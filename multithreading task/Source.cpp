#include <condition_variable>
#include <iostream>
#include <mutex>
#include <vector>
#include <thread>

std::mutex lock;
std::condition_variable cv;
bool flag = 0; // 1 - car is painted; 0 - car is dried

class Car {
public:
	static void dry() {
		for (;;) {
			std::unique_lock<std::mutex> un_lock(lock);
			cv.wait(un_lock, []() {
				return flag == 1;
			});
			std::cout << "Worker " << std::this_thread::get_id() << " is drying ..." << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(1));
			flag = 0;
			cv.notify_one();
		}
	}

	static void paint() {
		for (;;) {
			std::unique_lock<std::mutex> un_lock(lock);
			cv.wait(un_lock, []() {
				return flag == 0;
			});
			std::cout << "Worker " << std::this_thread::get_id() << " is painting ..." << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(1));
			flag = 1;
			cv.notify_one();
		}
	}
};
int main() {
	Car MyCar;
	std::thread worker1(Car::dry);
	std::thread worker2(Car::paint);
	worker1.join();
	worker2.join();
}

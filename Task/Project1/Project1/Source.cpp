#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

std::mutex lock;
std::condition_variable cv;

class Car {
public:
	static void dry() {
		std::unique_lock<std::mutex> un_lock(lock);
		std::cout <<"Worker "<< std::this_thread::get_id() << " is drying" << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(5));
		cv.wait(un_lock);
	}

	static void paint() {
		std::unique_lock<std::mutex> un_lock(lock);
		std::cout << "Worker " << std::this_thread::get_id() << " is painting" << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(5));
		cv.wait(un_lock);
	}
};

int main() {
	Car MyCar;
	std::thread worker1(Car::dry);
	std::thread worker2(Car::paint);
	worker1.join();
	worker2.join();
}
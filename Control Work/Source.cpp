#include <condition_variable>
#include <iostream>
#include <mutex>
#include <vector>
#include <thread>

std::mutex lock;
std::condition_variable cv;
/*-----------------Two workers----------------*/
class Car {
private:
	bool flag; // 1 - car is painted; 0 - car is dryted
public:
	Car() {
		flag = 0;
	}
	void SetFlag(bool new_value) {
		flag = new_value;
	}
	bool GetFlag() const {
		return flag;
	}
};

class Dryer {
private:
	Car* car;
	
public:
	Dryer() {
		car = nullptr;
	}
	Dryer(Car& new_car) {
		car = &new_car;
	}
	void dry() {
		for (;;) {
			std::unique_lock<std::mutex> un_lock(lock);
			cv.wait(un_lock, [&]() {
				return (*car).GetFlag() == 1;
			});
			std::cout << "Worker " << std::this_thread::get_id() << " is drying ..." << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(1));
			(*car).SetFlag(0);
			cv.notify_one();
		}
	}
};

class Painter {
private:
	Car *car;

public:
	Painter() {
		car = nullptr;
	}

	Painter(Car& new_car) {
		car = &new_car;
	}
	
	void paint() {
		for (;;) {
			std::unique_lock<std::mutex> un_lock(lock);
			cv.wait(un_lock, [&]() {
				return (*car).GetFlag() == 0;
			});
			std::cout << "Worker " << std::this_thread::get_id() << " is painting ..." << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(1));
			(*car).SetFlag(1);
			cv.notify_one();
		}
	}
};
/*-----------------MyThreadRAII----------------*/
class MyThreadRAII {
private:
	std::thread thread;
public:
	MyThreadRAII(std::thread&& other_thread) {
		thread = std::move(other_thread);
	}
	std::thread& GetThred() {
		return thread;
	}
	~MyThreadRAII() {
		if (thread.joinable()) {
			thread.join();
		}
	}
};

int main() {
	MyThreadRAII thread(std::thread());

	Car MyCar;
	MyCar.SetFlag(0);
	Painter p(MyCar);
	Dryer d(MyCar);

	std::thread worker1([&]() 
		{
			d.dry();
		}
	);
	std::thread worker2([&]()
		{
			p.paint();
		}
	);

	worker1.join();
	worker2.join();

}

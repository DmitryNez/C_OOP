#include <condition_variable>
#include <iostream>
#include <mutex>
#include <vector>
#include <thread>
#include <atomic>
#include <cassert>

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
/*---------Singleton----------*/
class Singleton {
private:
	static Singleton* singleton;
public:
	static Singleton* instance() {
		static std::mutex mutex;
		if (singleton == nullptr) {
			std::unique_lock<std::mutex> lock(mutex);
			if (singleton == nullptr)
			{
				singleton = new Singleton;
			}
		}
		return singleton;
	}
};
Singleton* Singleton::singleton = nullptr;

/*--------------Singleton with memory barrier---------------*/
class Singleton_ {
private:
	std::atomic<Singleton_*> singleton;
	std::mutex mutex;
public:
	Singleton_* instance(){
		Singleton_* tmp = singleton.load(std::memory_order_relaxed);
		std::atomic_thread_fence(std::memory_order_acquire);
		if (tmp == nullptr) {
			std::lock_guard<std::mutex> lock(mutex);
			tmp = singleton.load(std::memory_order_relaxed);
			if (tmp == nullptr) {
				tmp = new Singleton_;
				std::atomic_thread_fence(std::memory_order_release);
				singleton.store(tmp, std::memory_order_relaxed);
			}
		}
		return tmp;
	}
};

int main() {
	MyThreadRAII thread(std::thread());
Singleton_ OnlyOne;
	Singleton_* s_;
	Singleton_* v_;

	std::thread worker3([&]()
	{
		s_ = OnlyOne.instance();
	}
	);
	std::thread worker4([&]()
	{
		v_ = OnlyOne.instance();
	}
	);
	worker3.join();
	worker4.join();

	assert(s_ == v_);

	Singleton* s;
	Singleton* v;

	std::thread worker5([&]()
	{
		s = Singleton::instance();
	}
	);
	std::thread worker6([&]()
	{
		v = Singleton::instance();
	}
	);

	worker5.join();
	worker6.join();

	assert(s == v);

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

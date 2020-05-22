#include <iostream>
#include <memory>
using namespace std;

/*----------1-------------*/
template<class T>
class Weak_ptr {
private:
	shared_ptr<T>* ptr;
public:
	Weak_ptr() {
		ptr = nullptr;
	}
	Weak_ptr(const Weak_ptr<T>& other)
	{
		this->ptr = other.ptr;
	}
	Weak_ptr(shared_ptr<T>& other)
	{
		this->ptr = &other;
	}
	Weak_ptr(Weak_ptr<T>&& other) {
		this->ptr = other.ptr;
		other.ptr = nullptr;
	}

	Weak_ptr<T>& operator= (Weak_ptr<T> other) {
		Weak_ptr tmp(other);
		return *this;
	}

	~Weak_ptr() {

	}
};
class B;

class A {

public:
	A() {
		cout << "A - constructor" << endl;
	}
	~A() {
		cout << "A - destructor" << endl;
	}

	shared_ptr<B> Bptr;
};

class B {
public:
	B() {
		cout << "B - constructor" << endl;
	}
	~B() {
		cout << "B - destructor" << endl;
	}

	Weak_ptr<A> Aptr;
};
int main()
{
	auto a = std::make_shared<A>();

	a->Bptr = std::make_shared<B>();
	a->Bptr->Aptr = a;

	return 0;
}

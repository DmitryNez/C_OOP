#include "cell.h"
cell::cell() {
	this->status = state::empty;
}
cell::~cell() {

}

//cell::cell(size_t& x, size_t& y, state& status) {
//	this->x = x;
//	this->y = y;
//	this->status = status;
//}

//void cell::set_x(size_t& x) {
//	this->x = x;
//}
//
//void cell::set_y(size_t& y) {
//	this->y = y;
//}

void cell::set_status(state& status) {
	this->status = status;
}

//size_t cell::get_x() const {
//	return this->x;
//}
//
//size_t cell::get_y() const {
//	return this->y;
//}

state cell::get_status() const {
	return this->status;
}
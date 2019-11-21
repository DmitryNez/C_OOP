#include "field.h"

field::field() {

	this->size_x = 0;
	this->size_y = 0;
}
field::~field() {

}

void field::set_size(size_t& size_x, size_t& size_y) {
	this->field_of_cells.assign(size_x, std::vector<cell>(size_y));
	this->size_x = size_x;
	this->size_y = size_y;
}
size_t field::get_size_x() const{
	return this->size_x;
}
size_t field::get_size_y() const{
	return this->size_y;
}

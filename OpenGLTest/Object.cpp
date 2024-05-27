#include "Object.hpp"
#include <atomic>

size_t Object::NextIndex() {
	static std::atomic_size_t index(0);
	return ++index;
}

Object::Object() : id(NextIndex()), name() {

}

Object::Object(std::string const& _name) : id(NextIndex()), name(_name) {

}

Object::Object(const Object& other) : id(NextIndex()), name(other.name) {

}

Object::~Object() {

}

Object& Object::operator=(const Object& other) {
	name = other.name;
	return *this;
}

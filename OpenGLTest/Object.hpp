#pragma once
#include <string>
#include <atomic>

class __declspec(novtable) Object {
	static size_t NextIndex();
public:
	const size_t id;
	std::string name;

	Object();

	Object(std::string const& _name);
	
	Object(const Object& other);

	virtual ~Object();

	Object& operator=(const Object& other);
};
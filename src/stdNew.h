#pragma once
#include <memory>

// Convience class for std shared allocations
template<class T>
class stdNew
{
public:
	stdNew() { object = std::make_shared<T>(); }

public:
	T* operator->() const { return object.get(); }
	T* get() const { return object.get(); }
	std::shared_ptr<T> getSharedPtr() const { return object; }
	T& operator*() const { return *static_cast<T*>(object); }

	operator std::shared_ptr<T>() const { return object; }

private:
	stdNew(stdNew<T> const&) = delete;
	void operator=(stdNew<T> const&) = delete;
	std::shared_ptr<T> object;
};
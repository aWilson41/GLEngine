#pragma once
#include <memory>

// Convience class for std shared allocations
template<class T>
class stdNew
{
public:
	explicit stdNew(std::shared_ptr<T> obj) : object(obj) { }
	stdNew() : object(std::make_shared<T>()) { }

public:
	T* operator->() const { return object.get(); }
	T* get() const { return object.get(); }
	std::shared_ptr<T> getSharedPtr() const { return object; }
	T& operator*() const { return *static_cast<T*>(object); }

	operator std::shared_ptr<T>() const { return object; }

private:
	stdNew(const stdNew<T>&) = delete;
	stdNew(const stdNew<T>&&) = delete;
	void operator=(const stdNew<T>&) = delete;
	std::shared_ptr<T> object;
};
#pragma once
#include "const.h"
template <typename T>
class Singleton
{
protected:
	Singleton();
	Singleton(const Singleton<T>&) = delete;
	Singleton<T>& operator=(const Singleton<T>&) = delete;
	static std::shared_ptr<T> _instance;
public:
	static std::shared_ptr<T> getInstance();
	void PrintAddress();
};

template<typename T>
std::shared_ptr<T> Singleton<T>::_instance = nullptr;

template<typename T>
inline Singleton<T>::Singleton()
{
}

template<typename T>
inline std::shared_ptr<T> Singleton<T>::getInstance()
{
	static std::once_flag once;
	std::call_once(once, [] {
		_instance = std::shared_ptr<T>(new T());
		});
	return _instance;
}

template<typename T>
inline void Singleton<T>::PrintAddress()
{
	std::cout << _instance.get() << std::endl;
}

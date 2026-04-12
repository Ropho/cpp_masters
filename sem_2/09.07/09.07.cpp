#include <cstddef>
#include <iostream>
#include <new>

template<typename Derived>
class Entity
{
public:

	static void * operator new(std::size_t size)
	{
		std::cout << "Entity::operator new\n";
		return ::operator new(size);
	}

	static void operator delete(void * pointer) noexcept
	{
		std::cout << "Entity::operator delete\n";
		::operator delete(pointer);
	}

	static void * operator new[](std::size_t size)
	{
		std::cout << "Entity::operator new[]\n";
		return ::operator new[](size);
	}

	static void operator delete[](void * pointer) noexcept
	{
		std::cout << "Entity::operator delete[]\n";
		::operator delete[](pointer);
	}

	static void * operator new(std::size_t size, std::nothrow_t const & tag) noexcept
	{
		(void)tag;
		std::cout << "Entity::operator new(std::nothrow_t)\n";
		return ::operator new(size, std::nothrow);
	}

	static void operator delete(void * pointer, std::nothrow_t const & tag) noexcept
	{
		(void)tag;
		std::cout << "Entity::operator delete(void*, std::nothrow_t)\n";
		::operator delete(pointer);
	}

	static void * operator new[](std::size_t size, std::nothrow_t const & tag) noexcept
	{
		(void)tag;
		std::cout << "Entity::operator new[](std::nothrow_t)\n";
		return ::operator new[](size, std::nothrow);
	}

	static void operator delete[](void * pointer, std::nothrow_t const & tag) noexcept
	{
		(void)tag;
		std::cout << "Entity::operator delete[](void*, std::nothrow_t)\n";
		::operator delete[](pointer);
	}

protected:

	Entity() = default;
};

class Client : private Entity<Client>
{
public:

	Client()
	{
		std::cout << "Client::Client\n";
	}

	~Client()
	{
		std::cout << "Client::~Client\n";
	}

	using Entity<Client>::operator new;
	using Entity<Client>::operator new[];
	using Entity<Client>::operator delete;
	using Entity<Client>::operator delete[];
};

int main()
{
	std::cout << "--- single object ---\n";
	delete new Client;

	std::cout << "--- array ---\n";
	delete[] new Client[2];

	std::cout << "--- single nothrow ---\n";
	if (Client * const p = new (std::nothrow) Client)
	{
		delete p;
	}

	std::cout << "--- array nothrow ---\n";
	if (Client * const block = new (std::nothrow) Client[2])
	{
		delete[] block;
	}

	return 0;
}

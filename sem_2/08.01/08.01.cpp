////////////////////////////////////////////////////////////////////

#include <cassert>
#include <iostream>

////////////////////////////////////////////////////////////////////

class Entity_v1
{
public:

	Entity_v1(int x) : m_data(x) {}

	void print() const
	{
		std::cout << "Entity_v1::m_data = " << m_data << std::endl;
	}

private:

	int m_data;
};

////////////////////////////////////////////////////////////////////

class Entity_v2
{
public:

	int m_data;
};

////////////////////////////////////////////////////////////////////

int main()
{
	Entity_v1 entity(42);

	entity.print();

//  ----------------------------------------------------------------

	auto & ref = reinterpret_cast < Entity_v2 & > (entity);

	ref.m_data = 100;

	entity.print();

	assert(reinterpret_cast < Entity_v2 & > (entity).m_data == 100);

//  ----------------------------------------------------------------

	auto ptr = reinterpret_cast < int * > (&entity);

	*ptr = 200;

	entity.print();

	assert(reinterpret_cast < Entity_v2 & > (entity).m_data == 200);
}

////////////////////////////////////////////////////////////////////

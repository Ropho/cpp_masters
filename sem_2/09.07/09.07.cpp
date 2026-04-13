#include "06.09.hpp"

#include <bit>
#include <cstddef>
#include <iostream>
#include <new>
#include <utility>

namespace
{
constexpr std::size_t k_entity_storage_bytes = 16;
}

static_assert(k_entity_storage_bytes == 16);

class Entity::Implementation
{
public:

	Implementation()
		: m_value(0)
	{
	}

	explicit Implementation(int value)
		: m_value(value)
	{
	}

	Implementation(Implementation && other) noexcept
		: m_value(other.m_value)
	{
		other.m_value = 0;
	}

	void test() const
	{
		std::cout << "Implementation::test value=" << m_value << '\n';
	}

	int value() const
	{
		return m_value;
	}

	void set_value(int value)
	{
		m_value = value;
	}

private:

	int m_value{};
};

Entity::Entity()
{
	static_assert(sizeof(Implementation) <= k_entity_storage_bytes);
	static_assert(alignof(Implementation) <= alignof(std::max_align_t));

	::new (m_storage.data()) Implementation();
}

Entity::Entity(Entity && other) noexcept
{
	::new (m_storage.data()) Implementation(std::move(*other.get()));
	std::destroy_at(other.get());
	::new (other.m_storage.data()) Implementation();
}

Entity & Entity::operator=(Entity && other) noexcept
{
	if (this == &other)
	{
		return *this;
	}

	std::destroy_at(get());

	Implementation * const source = other.get();
	::new (m_storage.data()) Implementation(std::move(*source));

	std::destroy_at(source);
	::new (other.m_storage.data()) Implementation();

	return *this;
}

Entity::~Entity()
{
	std::destroy_at(get());
}

void Entity::test() const
{
	get()->test();
}

Entity::Implementation * Entity::get()
{
	return std::launder(std::bit_cast<Implementation *>(m_storage.data()));
}

Entity::Implementation const * Entity::get() const
{
	return std::launder(std::bit_cast<Implementation const *>(m_storage.data()));
}

int main()
{
	Entity a;

	a.test();

	a.get()->set_value(42);

	a.test();

	Entity b = std::move(a);

	b.test();

	Entity c;

	c = std::move(b);

	c.test();

	return 0;
}

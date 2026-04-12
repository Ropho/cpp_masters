#pragma once

class Entity
{
public:

	Entity();

	Entity(Entity && other);

	~Entity();

	Entity & operator=(Entity && other);

	void test() const;

	Entity(Entity const &) = delete;

	Entity & operator=(Entity const &) = delete;

private:

	class Implementation;

	Implementation * m_pimpl = nullptr;
};

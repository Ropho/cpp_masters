#pragma once

#include <array>
#include <cstddef>

class Entity
{
public:

	class Implementation;

	Entity();

	Entity(Entity && other) noexcept;

	Entity & operator=(Entity && other) noexcept;

	~Entity();

	Entity(Entity const &) = delete;

	Entity & operator=(Entity const &) = delete;

	void test() const;

	Implementation * get();

	Implementation const * get() const;

private:

	alignas(std::max_align_t) std::array<std::byte, 16> m_storage{};
};

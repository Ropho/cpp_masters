#include <cassert>
#include <cstddef>
#include <memory>
#include <vector>

class Entity
{
public:

	virtual ~Entity() = default;

	virtual int test() const = 0;
};

class Client : public Entity
{
public:

	int test() const override
	{
		return 1;
	}
};

class Server : public Entity
{
public:

	int test() const override
	{
		return 2;
	}
};

class Composite : public Entity
{
public:

	void add(std::unique_ptr<Entity> entity)
	{
		m_entities.push_back(std::move(entity));
	}

	int test() const override
	{
		int sum = 0;

		for (auto const & entity : m_entities)
		{
			if (entity)
			{
				sum += entity->test();
			}
		}

		return sum;
	}

private:

	std::vector<std::unique_ptr<Entity>> m_entities{};
};

std::unique_ptr<Entity> make_composite(std::size_t size_1, std::size_t size_2)
{
	auto composite = std::make_unique<Composite>();

	for (std::size_t i = 0; i < size_1; ++i)
	{
		composite->add(std::make_unique<Client>());
	}

	for (std::size_t i = 0; i < size_2; ++i)
	{
		composite->add(std::make_unique<Server>());
	}

	return composite;
}

int main()
{
	auto composite = std::make_unique<Composite>();

	for (std::size_t i = 0; i < 5; ++i)
	{
		composite->add(make_composite(1, 1));
	}

	assert(composite->test() == 15);
}

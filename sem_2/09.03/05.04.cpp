#include <memory>
#include <vector>

class Entity
{
public:

	virtual ~Entity() = default;

	virtual std::unique_ptr<Entity> copy() const = 0;
};

class Client : public Entity
{
public:

	std::unique_ptr<Entity> copy() const override
	{
		return std::make_unique<Client>(*this);
	}
};

class Server : public Entity
{
public:

	std::unique_ptr<Entity> copy() const override
	{
		return std::make_unique<Server>(*this);
	}
};

class Prototype
{
public:

	Prototype()
	{
		m_entities.push_back(std::make_unique<Client>());
		m_entities.push_back(std::make_unique<Server>());
	}

	std::unique_ptr<Entity> make_client()
	{
		return m_entities.at(0)->copy();
	}

	std::unique_ptr<Entity> make_server()
	{
		return m_entities.at(1)->copy();
	}

private:

	std::vector<std::unique_ptr<Entity>> m_entities{};
};

int main()
{
	std::unique_ptr<Entity> const clone = Prototype().make_client();

	(void)clone;
}

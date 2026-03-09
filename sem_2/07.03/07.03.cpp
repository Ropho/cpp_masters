////////////////////////////////////////////////////////////////////////////////
//
// Normal branching:
//
//   if-else: save(Status::success, ...) or save(Status::failure, ...) based
//   on person.grade() == 10 || person.salary() > 1'000'000.
//
//   Short-circuit evaluation of ||: if person.grade() == 10 is true then
//   person.salary() is not evaluated.
//
// Possible exceptions:
//
//   person.name()   -- may throw (e.g. std::bad_alloc during string copy).
//   std::cout << .. -- may throw std::ios_base::failure if exceptions enabled.
//   person.grade(), person.salary(), person.id() -- user-defined, may throw.
//   save(...)       -- user-defined, may throw; person.id() evaluated first.
//
////////////////////////////////////////////////////////////////////////////////

#include <iostream>

////////////////////////////////////////////////////////////////////////////////

enum class Status { success, failure };

struct Person
{
	auto name()  const -> const char * { return ""; }
	auto grade() const -> int          { return 0; }
	auto salary()const -> long long    { return 0; }
	auto id()    const -> int          { return 0; }
};

void save(Status, int) {}

////////////////////////////////////////////////////////////////////////////////

void test(Person const & person)
{
	std::cout << "test : " << person.name() << '\n';

	if (person.grade() == 10 || person.salary() > 1'000'000)
	{
		save(Status::success, person.id());
	}
	else
	{
		save(Status::failure, person.id());
	}
}

////////////////////////////////////////////////////////////////////////////////

int main()
{
	Person p;

	test(p);
}

////////////////////////////////////////////////////////////////////////////////

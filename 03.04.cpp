#include <iostream>
#include <string>


class EntityAttorney_v1;

class EntityAttorney_v2;

class Entity {
private:
	int test_v1(int value) const {
		return value * 2;
	}

	bool test_v2(const std::string& str) const {
		return !str.empty();
	}

	friend class EntityAttorney_v1;
	friend class EntityAttorney_v2;

public:
	Entity() = default;
};

class EntityAttorney_v1 {
private:
	static int call_test_v1(const Entity& entity, int value) {
		return entity.test_v1(value);
	}

	friend class Tester_v1;
};

class EntityAttorney_v2 {
private:
	static bool call_test_v2(const Entity& entity, const std::string& str) {
		return entity.test_v2(str);
	}

	friend class Tester_v2;
};

class Tester_v1 {
public:
	static int test(const Entity& entity, int value) {
		return EntityAttorney_v1::call_test_v1(entity, value);
	}

	static void run_tests() {
		std::cout << "=== Testing test_v1 via Tester_v1 ===" << std::endl;

		Entity entity;
		int tests_passed = 0;
		int tests_total = 0;

		{
			tests_total++;
			int result = Tester_v1::test(entity, 5);
			if (result == 10)
			{
				std::cout << "✓ Test " << tests_total << ": test_v1(5) = 10 [PASSED]" << std::endl;
				tests_passed++;
			}
			else
			{
				std::cout << "✗ Test " << tests_total << ": test_v1(5) expected 10, got " << result << " [FAILED]" << std::endl;
			}
		}

		{
			tests_total++;
			int result = Tester_v1::test(entity, 0);
			if (result == 0)
			{
				std::cout << "✓ Test " << tests_total << ": test_v1(0) = 0 [PASSED]" << std::endl;
				tests_passed++;
			}
			else
			{
				std::cout << "✗ Test " << tests_total << ": test_v1(0) [FAILED]" << std::endl;
			}
		}

		{
			tests_total++;
			int result = Tester_v1::test(entity, -3);
			if (result == -6)
			{
				std::cout << "✓ Test " << tests_total << ": test_v1(-3) = -6 [PASSED]" << std::endl;
				tests_passed++;
			}
			else
			{
				std::cout << "✗ Test " << tests_total << ": test_v1(-3) [FAILED]" << std::endl;
			}
		}

		{
			tests_total++;
			int result = Tester_v1::test(entity, 1000);
			if (result == 2000)
			{
				std::cout << "✓ Test " << tests_total << ": test_v1(1000) = 2000 [PASSED]" << std::endl;
				tests_passed++;
			}
			else
			{
				std::cout << "✗ Test " << tests_total << ": test_v1(1000) [FAILED]" << std::endl;
			}
		}

		std::cout << "Tests passed: " << tests_passed << " / " << tests_total << std::endl;
		std::cout << "Success rate: " << (100.0 * tests_passed / tests_total) << "%\n" << std::endl;
	}
};

class Tester_v2 {
public:
	static bool test(const Entity& entity, const std::string& str) {
		return EntityAttorney_v2::call_test_v2(entity, str);
	}

	static void run_tests() {
		std::cout << "=== Testing test_v2 via Tester_v2 ===" << std::endl;

		Entity entity;
		int tests_passed = 0;
		int tests_total = 0;

		{
			tests_total++;
			bool result = Tester_v2::test(entity, "hello");
			if (result == true)
			{
				std::cout << "✓ Test " << tests_total << ": test_v2(\"hello\") = true [PASSED]" << std::endl;
				tests_passed++;
			}
			else
			{
				std::cout << "✗ Test " << tests_total << ": test_v2(\"hello\") [FAILED]" << std::endl;
			}
		}

		{
			tests_total++;
			bool result = Tester_v2::test(entity, "");
			if (result == false)
			{
				std::cout << "✓ Test " << tests_total << ": test_v2(\"\") = false [PASSED]" << std::endl;
				tests_passed++;
			}
			else
			{
				std::cout << "✗ Test " << tests_total << ": test_v2(\"\") [FAILED]" << std::endl;
			}
		}

		{
			tests_total++;
			bool result = Tester_v2::test(entity, "   ");
			if (result == true)
			{
				std::cout << "✓ Test " << tests_total << ": test_v2(\"   \") = true [PASSED]" << std::endl;
				tests_passed++;
			}
			else
			{
				std::cout << "✗ Test " << tests_total << ": test_v2(\"   \") [FAILED]" << std::endl;
			}
		}

		{
			tests_total++;
			std::string long_str(1000, 'a');
			bool result = Tester_v2::test(entity, long_str);
			if (result == true)
			{
				std::cout << "✓ Test " << tests_total << ": test_v2(long string) = true [PASSED]" << std::endl;
				tests_passed++;
			}
			else
			{
				std::cout << "✗ Test " << tests_total << ": test_v2(long string) [FAILED]" << std::endl;
			}
		}

		std::cout << "Tests passed: " << tests_passed << " / " << tests_total << std::endl;
		std::cout << "Success rate: " << (100.0 * tests_passed / tests_total) << "%\n" << std::endl;
	}
};

int main()
{
	Tester_v1::run_tests();
	Tester_v2::run_tests();

	std::cout << "=== All Tests Summary ===" << std::endl;
	std::cout << "✓ All tests executed using Attorney-Client pattern" << std::endl;
	std::cout << "✓ Access to private methods restricted via Attorney classes" << std::endl;
	std::cout << "✓ Single Responsibility Principle maintained" << std::endl;

	return 0;
}


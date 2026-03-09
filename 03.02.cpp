#include <cmath>
#include <numbers>
#include <iostream>
#include <iomanip>

class Triangle {
private:
	double side1;
	double side2;
	double side3;

public:
	Triangle(double a, double b, double c) : side1(a), side2(b), side3(c){}

	double perimeter() const {
		return side1 + side2 + side3;
	}

	double area() const{
		double s = perimeter() / 2.0;
		return std::sqrt(s * (s - side1) * (s - side2) * (s - side3));
	}
};

class Square {
private:
	double side;

public:
	Square(double s) : side(s){}

	double perimeter() const {
		return 4.0 * side;
	}

	double area() const {
		return side * side;
	}
};

class Circle {
private:
	double radius;

public:
	Circle(double r) : radius(r){}

	double perimeter() const {
		return 2.0 * std::numbers::pi * radius;
	}

	double area() const {
		return std::numbers::pi * radius * radius;
	}
};

bool approximately_equal(double a, double b, double epsilon = 1e-9)
{
	return std::abs(a - b) < epsilon;
}

void run_unit_tests()
{
	int tests_passed = 0;
	int tests_total = 0;
	const double epsilon = 1e-9;

	std::cout << std::fixed << std::setprecision(10);

	std::cout << "=== Testing Triangle class ===" << std::endl;

	{
		tests_total++;
		Triangle triangle(3.0, 3.0, 3.0);
		double expected_perimeter = 9.0;
		double expected_area = (3.0 * 3.0 * std::sqrt(3.0)) / 4.0;
		
		if (approximately_equal(triangle.perimeter(), expected_perimeter, epsilon) &&
		    approximately_equal(triangle.area(), expected_area, epsilon))
		{
			std::cout << "✓ Test " << tests_total << ": Equilateral triangle (3, 3, 3) [PASSED]" << std::endl;
			tests_passed++;
		}
		else
		{
			std::cout << "✗ Test " << tests_total << ": Equilateral triangle [FAILED]" << std::endl;
			std::cout << "  Perimeter: expected " << expected_perimeter 
			          << ", got " << triangle.perimeter() << std::endl;
			std::cout << "  Area: expected " << expected_area 
			          << ", got " << triangle.area() << std::endl;
		}
	}

	{
		tests_total++;
		Triangle triangle(3.0, 4.0, 5.0);
		double expected_perimeter = 12.0;
		double expected_area = 6.0;
		
		if (approximately_equal(triangle.perimeter(), expected_perimeter, epsilon) &&
		    approximately_equal(triangle.area(), expected_area, epsilon))
		{
			std::cout << "✓ Test " << tests_total << ": Right triangle (3, 4, 5) [PASSED]" << std::endl;
			tests_passed++;
		}
		else
		{
			std::cout << "✗ Test " << tests_total << ": Right triangle [FAILED]" << std::endl;
		}
	}

	{
		tests_total++;
		Triangle triangle(5.0, 6.0, 7.0);
		double expected_perimeter = 18.0;
		double s = 9.0;
		double expected_area = std::sqrt(s * (s - 5.0) * (s - 6.0) * (s - 7.0));
		
		if (approximately_equal(triangle.perimeter(), expected_perimeter, epsilon) &&
		    approximately_equal(triangle.area(), expected_area, epsilon))
		{
			std::cout << "✓ Test " << tests_total << ": Arbitrary triangle (5, 6, 7) [PASSED]" << std::endl;
			tests_passed++;
		}
		else
		{
			std::cout << "✗ Test " << tests_total << ": Arbitrary triangle [FAILED]" << std::endl;
		}
	}

	{
		tests_total++;
		Triangle triangle(1.0, 1.0, 1.0);
		double expected_perimeter = 3.0;
		
		if (approximately_equal(triangle.perimeter(), expected_perimeter, epsilon) &&
		    triangle.area() > 0)
		{
			std::cout << "✓ Test " << tests_total << ": Unit triangle (1, 1, 1) [PASSED]" << std::endl;
			tests_passed++;
		}
		else
		{
			std::cout << "✗ Test " << tests_total << ": Unit triangle [FAILED]" << std::endl;
		}
	}

	std::cout << "\n=== Testing Square class ===" << std::endl;

	{
		tests_total++;
		Square square(5.0);
		double expected_perimeter = 20.0;
		double expected_area = 25.0;
		
		if (approximately_equal(square.perimeter(), expected_perimeter, epsilon) &&
		    approximately_equal(square.area(), expected_area, epsilon))
		{
			std::cout << "✓ Test " << tests_total << ": Square with side 5 [PASSED]" << std::endl;
			tests_passed++;
		}
		else
		{
			std::cout << "✗ Test " << tests_total << ": Square with side 5 [FAILED]" << std::endl;
		}
	}

	{
		tests_total++;
		Square square(1.0);
		double expected_perimeter = 4.0;
		double expected_area = 1.0;
		
		if (approximately_equal(square.perimeter(), expected_perimeter, epsilon) &&
		    approximately_equal(square.area(), expected_area, epsilon))
		{
			std::cout << "✓ Test " << tests_total << ": Unit square [PASSED]" << std::endl;
			tests_passed++;
		}
		else
		{
			std::cout << "✗ Test " << tests_total << ": Unit square [FAILED]" << std::endl;
		}
	}

	{
		tests_total++;
		Square square(2.5);
		double expected_perimeter = 10.0;
		double expected_area = 6.25;
		
		if (approximately_equal(square.perimeter(), expected_perimeter, epsilon) &&
		    approximately_equal(square.area(), expected_area, epsilon))
		{
			std::cout << "✓ Test " << tests_total << ": Square with side 2.5 [PASSED]" << std::endl;
			tests_passed++;
		}
		else
		{
			std::cout << "✗ Test " << tests_total << ": Square with side 2.5 [FAILED]" << std::endl;
		}
	}

	{
		tests_total++;
		Square square(100.0);
		double expected_perimeter = 400.0;
		double expected_area = 10000.0;
		
		if (approximately_equal(square.perimeter(), expected_perimeter, epsilon) &&
		    approximately_equal(square.area(), expected_area, epsilon))
		{
			std::cout << "✓ Test " << tests_total << ": Large square (side 100) [PASSED]" << std::endl;
			tests_passed++;
		}
		else
		{
			std::cout << "✗ Test " << tests_total << ": Large square [FAILED]" << std::endl;
		}
	}

	std::cout << "\n=== Testing Circle class ===" << std::endl;

	{
		tests_total++;
		Circle circle(1.0);
		double expected_perimeter = 2.0 * std::numbers::pi;
		double expected_area = std::numbers::pi;
		
		if (approximately_equal(circle.perimeter(), expected_perimeter, epsilon) &&
		    approximately_equal(circle.area(), expected_area, epsilon))
		{
			std::cout << "✓ Test " << tests_total << ": Circle with radius 1 [PASSED]" << std::endl;
			tests_passed++;
		}
		else
		{
			std::cout << "✗ Test " << tests_total << ": Circle with radius 1 [FAILED]" << std::endl;
			std::cout << "  Perimeter: expected " << expected_perimeter 
			          << ", got " << circle.perimeter() << std::endl;
			std::cout << "  Area: expected " << expected_area 
			          << ", got " << circle.area() << std::endl;
		}
	}

	{
		tests_total++;
		Circle circle(5.0);
		double expected_perimeter = 2.0 * std::numbers::pi * 5.0;
		double expected_area = std::numbers::pi * 25.0;
		
		if (approximately_equal(circle.perimeter(), expected_perimeter, epsilon) &&
		    approximately_equal(circle.area(), expected_area, epsilon))
		{
			std::cout << "✓ Test " << tests_total << ": Circle with radius 5 [PASSED]" << std::endl;
			tests_passed++;
		}
		else
		{
			std::cout << "✗ Test " << tests_total << ": Circle with radius 5 [FAILED]" << std::endl;
		}
	}

	{
		tests_total++;
		Circle circle(2.5);
		double expected_perimeter = 2.0 * std::numbers::pi * 2.5;
		double expected_area = std::numbers::pi * 2.5 * 2.5;
		
		if (approximately_equal(circle.perimeter(), expected_perimeter, epsilon) &&
		    approximately_equal(circle.area(), expected_area, epsilon))
		{
			std::cout << "✓ Test " << tests_total << ": Circle with radius 2.5 [PASSED]" << std::endl;
			tests_passed++;
		}
		else
		{
			std::cout << "✗ Test " << tests_total << ": Circle with radius 2.5 [FAILED]" << std::endl;
		}
	}

	{
		tests_total++;
		Circle circle(0.5);
		double expected_perimeter = 2.0 * std::numbers::pi * 0.5;
		double expected_area = std::numbers::pi * 0.25;
		
		if (approximately_equal(circle.perimeter(), expected_perimeter, epsilon) &&
		    approximately_equal(circle.area(), expected_area, epsilon))
		{
			std::cout << "✓ Test " << tests_total << ": Circle with radius 0.5 [PASSED]" << std::endl;
			tests_passed++;
		}
		else
		{
			std::cout << "✗ Test " << tests_total << ": Circle with radius 0.5 [FAILED]" << std::endl;
		}
	}

	std::cout << "\n=== Test Results ===" << std::endl;
	std::cout << "Tests passed: " << tests_passed << " / " << tests_total << std::endl;
	std::cout << "Success rate: " << (100.0 * tests_passed / tests_total) << "%" << std::endl;
	
	if (tests_passed == tests_total)
	{
		std::cout << "\n✓ All tests passed successfully!" << std::endl;
	}
	else
	{
		std::cout << "\n✗ Some tests failed!" << std::endl;
	}
}

int main()
{
	run_unit_tests();
	return 0;
}


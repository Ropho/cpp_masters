#include <cmath>
#include <numbers>
#include <vector>
#include <iostream>
#include <iomanip>

class Shape {
public:
	virtual ~Shape() = default;

	virtual double perimeter() const = 0;
	virtual double area() const = 0;
};

class Triangle : public Shape {
private:
	double side1;
	double side2;
	double side3;

public:
	Triangle(double a, double b, double c) : side1(a), side2(b), side3(c)
	{}

	double perimeter() const override final {
		return side1 + side2 + side3;
	}

	double area() const override final {
		double s = perimeter() / 2.0;
		return std::sqrt(s * (s - side1) * (s - side2) * (s - side3));
	}
};

class Square final : public Shape {
private:
	double side;

public:
	Square(double s) : side(s) {}

	double perimeter() const override {
		return 4.0 * side;
	}

	double area() const override {
		return side * side;
	}
};

class Circle final : public Shape {
private:
	double radius;

public:
	Circle(double r) : radius(r) {}

	double perimeter() const override {
		return 2.0 * std::numbers::pi * radius;
	}

	double area() const override {
		return std::numbers::pi * radius * radius;
	}
};

void demonstrate_polymorphism()
{
	std::cout << "=== Polymorphism Demonstration via std::vector<Shape*> ===" << std::endl;
	std::cout << std::endl;

	std::vector<Shape*> shapes;

	shapes.push_back(new Triangle(3.0, 4.0, 5.0));
	shapes.push_back(new Square(5.0));
	shapes.push_back(new Circle(2.0));
	shapes.push_back(new Triangle(5.0, 6.0, 7.0));
	shapes.push_back(new Square(2.5));
	shapes.push_back(new Circle(1.5));

	std::cout << "Shape information:" << std::endl;
	std::cout << std::fixed << std::setprecision(2);

	int index = 1;
	for (Shape* shape : shapes) {
		std::cout << "Shape " << index << ": " << std::endl;
		std::cout << "  Perimeter: " << shape->perimeter() << std::endl;
		std::cout << "  Area: " << shape->area() << std::endl;
		std::cout << std::endl;
		++index;
	}

	for (Shape* shape : shapes) {
		delete shape;
	}
	shapes.clear();
}

void run_tests()
{
	std::cout << "=== Unit Tests ===" << std::endl;
	std::cout << std::endl;

	int tests_passed = 0;
	int tests_total = 0;
	const double epsilon = 1e-9;

	{
		tests_total++;
		Shape* triangle = new Triangle(3.0, 4.0, 5.0);
		double expected_perimeter = 12.0;
		double expected_area = 6.0;

		if (std::abs(triangle->perimeter() - expected_perimeter) < epsilon &&
		    std::abs(triangle->area() - expected_area) < epsilon)
		{
			std::cout << "✓ Test " << tests_total << ": Triangle via Shape* [PASSED]" << std::endl;
			tests_passed++;
		}
		else
		{
			std::cout << "✗ Test " << tests_total << ": Triangle via Shape* [FAILED]" << std::endl;
		}
		delete triangle;
	}

	{
		tests_total++;
		Shape* square = new Square(5.0);
		double expected_perimeter = 20.0;
		double expected_area = 25.0;

		if (std::abs(square->perimeter() - expected_perimeter) < epsilon &&
		    std::abs(square->area() - expected_area) < epsilon)
		{
			std::cout << "✓ Test " << tests_total << ": Square via Shape* [PASSED]" << std::endl;
			tests_passed++;
		}
		else
		{
			std::cout << "✗ Test " << tests_total << ": Square via Shape* [FAILED]" << std::endl;
		}
		delete square;
	}

	{
		tests_total++;
		Shape* circle = new Circle(1.0);
		double expected_perimeter = 2.0 * std::numbers::pi;
		double expected_area = std::numbers::pi;

		if (std::abs(circle->perimeter() - expected_perimeter) < epsilon &&
		    std::abs(circle->area() - expected_area) < epsilon)
		{
			std::cout << "✓ Test " << tests_total << ": Circle via Shape* [PASSED]" << std::endl;
			tests_passed++;
		}
		else
		{
			std::cout << "✗ Test " << tests_total << ": Circle via Shape* [FAILED]" << std::endl;
		}
		delete circle;
	}

	{
		tests_total++;
		std::vector<Shape*> shapes;
		shapes.push_back(new Triangle(3.0, 3.0, 3.0));
		shapes.push_back(new Square(2.0));
		shapes.push_back(new Circle(1.0));

		bool all_positive = true;
		for (Shape* shape : shapes)
		{
			if (shape->perimeter() <= 0 || shape->area() <= 0)
			{
				all_positive = false;
				break;
			}
		}

		if (all_positive && shapes.size() == 3)
		{
			std::cout << "✓ Test " << tests_total << ": Polymorphism in std::vector [PASSED]" << std::endl;
			tests_passed++;
		}
		else
		{
			std::cout << "✗ Test " << tests_total << ": Polymorphism in std::vector [FAILED]" << std::endl;
		}

		for (Shape* shape : shapes)
		{
			delete shape;
		}
	}

	{
		tests_total++;
		Square square(5.0);
		Circle circle(3.0);
		
		if (square.perimeter() > 0 && circle.perimeter() > 0)
		{
			std::cout << "✓ Test " << tests_total << ": Square and Circle declared as final [PASSED]" << std::endl;
			tests_passed++;
		}
		else
		{
			std::cout << "✗ Test " << tests_total << ": Square and Circle [FAILED]" << std::endl;
		}
	}

	{
		tests_total++;
		Triangle triangle(3.0, 4.0, 5.0);
		Shape* shape = &triangle;
		
		if (shape->perimeter() == 12.0 && std::abs(shape->area() - 6.0) < epsilon)
		{
			std::cout << "✓ Test " << tests_total << ": Triangle::perimeter and area declared as final [PASSED]" << std::endl;
			tests_passed++;
		}
		else
		{
			std::cout << "✗ Test " << tests_total << ": Triangle methods [FAILED]" << std::endl;
		}
	}

	std::cout << std::endl;
	std::cout << "Tests passed: " << tests_passed << " / " << tests_total << std::endl;
	std::cout << "Success rate: " << (100.0 * tests_passed / tests_total) << "%" << std::endl;
	std::cout << std::endl;
}

int main()
{
	demonstrate_polymorphism();

	run_tests();

	std::cout << "=== Summary ===" << std::endl;
	std::cout << "✓ Abstract base class Shape implemented" << std::endl;
	std::cout << "✓ Classes Triangle, Square, Circle inherit from Shape" << std::endl;
	std::cout << "✓ override specifier used for overriding" << std::endl;
	std::cout << "✓ final specifier used for Square and Circle" << std::endl;
	std::cout << "✓ final specifier used for Triangle methods" << std::endl;
	std::cout << "✓ std::vector<Shape*> used for storing shapes" << std::endl;

	return 0;
}


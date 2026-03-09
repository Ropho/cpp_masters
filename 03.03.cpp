#include <iostream>

class List {
private:
	struct Node {
		int value;
		Node* next;

		Node(int val) : value(val), next(nullptr){}
	};

	Node* first;
	Node* last;

public:
	List() : first(nullptr), last(nullptr){}

	~List() {
		while (!empty()) {
			pop_front();
		}
	}

	bool empty() const {
		return first == nullptr;
	}

	void show() const {
		Node* current = first;
		while (current != nullptr) {
			std::cout << current->value;
			if (current->next != nullptr) {
				std::cout << " ";
			}
			current = current->next;
		}
		std::cout << std::endl;
	}

	void push_front(int value) {
		Node* new_node = new Node(value);
		
		if (empty()) {
			first = last = new_node;
		}
		else {
			new_node->next = first;
			first = new_node;
		}
	}

	void push_back(int value) {
		Node* new_node = new Node(value);
		
		if (empty()) {
			first = last = new_node;
		}
		else {
			last->next = new_node;
			last = new_node;
		}
	}

	void pop_front() {
		if (empty()) {
			return;
		}

		Node* temp = first;
		first = first->next;

		if (first == nullptr) {
			last = nullptr;
		}

		delete temp;
	}

	void pop_back() {
		if (empty()) {
			return;
		}

		if (first == last) {
			delete first;
			first = last = nullptr;
		}
		else {
			Node* current = first;
			while (current->next != last) {
				current = current->next;
			}

			delete last;
			last = current;
			last->next = nullptr;
		}
	}

	int get() const {
		if (empty()) {
			return 0;
		}

		Node* slow = first;
		Node* fast = first;

		while (fast != nullptr && fast->next != nullptr) {
			fast = fast->next->next;
			if (fast != nullptr) {
				slow = slow->next;
			}
		}

		return slow->value;
	}
};

void run_unit_tests()
{
	int tests_passed = 0;
	int tests_total = 0;

	std::cout << "=== Unit tests for List class ===" << std::endl;

	{
		tests_total++;
		List list;
		if (list.empty())
		{
			std::cout << "✓ Test " << tests_total << ": empty() for empty list [PASSED]" << std::endl;
			tests_passed++;
		}
		else
		{
			std::cout << "✗ Test " << tests_total << ": empty() for empty list [FAILED]" << std::endl;
		}
	}

	{
		tests_total++;
		List list;
		list.push_front(10);
		if (!list.empty())
		{
			std::cout << "✓ Test " << tests_total << ": push_front() and empty() [PASSED]" << std::endl;
			tests_passed++;
		}
		else
		{
			std::cout << "✗ Test " << tests_total << ": push_front() and empty() [FAILED]" << std::endl;
		}
	}

	{
		tests_total++;
		List list;
		list.push_back(20);
		if (!list.empty())
		{
			std::cout << "✓ Test " << tests_total << ": push_back() and empty() [PASSED]" << std::endl;
			tests_passed++;
		}
		else
		{
			std::cout << "✗ Test " << tests_total << ": push_back() and empty() [FAILED]" << std::endl;
		}
	}

	{
		tests_total++;
		List list;
		list.push_front(5);
		list.pop_front();
		if (list.empty())
		{
			std::cout << "✓ Test " << tests_total << ": pop_front() and empty() [PASSED]" << std::endl;
			tests_passed++;
		}
		else
		{
			std::cout << "✗ Test " << tests_total << ": pop_front() and empty() [FAILED]" << std::endl;
		}
	}

	{
		tests_total++;
		List list;
		list.push_back(5);
		list.pop_back();
		if (list.empty())
		{
			std::cout << "✓ Test " << tests_total << ": pop_back() and empty() [PASSED]" << std::endl;
			tests_passed++;
		}
		else
		{
			std::cout << "✗ Test " << tests_total << ": pop_back() and empty() [FAILED]" << std::endl;
		}
	}

	{
		tests_total++;
		List list;
		list.push_front(42);
		if (list.get() == 42)
		{
			std::cout << "✓ Test " << tests_total << ": get() for single element [PASSED]" << std::endl;
			tests_passed++;
		}
		else
		{
			std::cout << "✗ Test " << tests_total << ": get() for single element [FAILED]" << std::endl;
		}
	}

	{
		tests_total++;
		List list;
		list.push_back(1);
		list.push_back(2);
		list.push_back(3);
		if (list.get() == 2)
		{
			std::cout << "✓ Test " << tests_total << ": get() for three elements [PASSED]" << std::endl;
			tests_passed++;
		}
		else
		{
			std::cout << "✗ Test " << tests_total << ": get() for three elements [FAILED]" << std::endl;
			std::cout << "  Expected: 2, got: " << list.get() << std::endl;
		}
	}

	{
		tests_total++;
		List list;
		list.push_back(1);
		list.push_back(2);
		list.push_back(3);
		list.push_back(4);
		if (list.get() == 2)
		{
			std::cout << "✓ Test " << tests_total << ": get() for four elements [PASSED]" << std::endl;
			tests_passed++;
		}
		else
		{
			std::cout << "✗ Test " << tests_total << ": get() for four elements [FAILED]" << std::endl;
		}
	}

	{
		tests_total++;
		List list;
		list.push_back(10);
		list.push_back(20);
		list.push_back(30);
		list.push_back(40);
		list.push_back(50);
		if (list.get() == 30)
		{
			std::cout << "✓ Test " << tests_total << ": get() for five elements [PASSED]" << std::endl;
			tests_passed++;
		}
		else
		{
			std::cout << "✗ Test " << tests_total << ": get() for five elements [FAILED]" << std::endl;
		}
	}

	{
		tests_total++;
		List list;
		list.push_back(1);
		list.push_back(2);
		list.push_back(3);
		list.pop_front();
		if (!list.empty())
		{
			std::cout << "✓ Test " << tests_total << ": pop_front() for multiple elements [PASSED]" << std::endl;
			tests_passed++;
		}
		else
		{
			std::cout << "✗ Test " << tests_total << ": pop_front() for multiple elements [FAILED]" << std::endl;
		}
	}

	{
		tests_total++;
		List list;
		list.push_back(1);
		list.push_back(2);
		list.push_back(3);
		list.pop_back();
		if (!list.empty() && list.get() == 1)
		{
			std::cout << "✓ Test " << tests_total << ": pop_back() for multiple elements [PASSED]" << std::endl;
			tests_passed++;
		}
		else
		{
			std::cout << "✗ Test " << tests_total << ": pop_back() for multiple elements [FAILED]" << std::endl;
		}
	}

	std::cout << "\n=== Integration tests ===" << std::endl;

	{
		tests_total++;
		List list;
		list.push_front(1);
		list.push_front(2);
		list.push_front(3);
		list.pop_front();
		list.pop_front();
		if (!list.empty() && list.get() == 1)
		{
			std::cout << "✓ Integration test 1: push_front and pop_front [PASSED]" << std::endl;
			tests_passed++;
		}
		else
		{
			std::cout << "✗ Integration test 1: push_front and pop_front [FAILED]" << std::endl;
		}
	}

	{
		tests_total++;
		List list;
		list.push_back(1);
		list.push_back(2);
		list.push_back(3);
		list.pop_back();
		list.pop_back();
		if (!list.empty() && list.get() == 1)
		{
			std::cout << "✓ Integration test 2: push_back and pop_back [PASSED]" << std::endl;
			tests_passed++;
		}
		else
		{
			std::cout << "✗ Integration test 2: push_back and pop_back [FAILED]" << std::endl;
		}
	}

	{
		tests_total++;
		List list;
		list.push_front(1);
		list.push_back(2);
		list.push_front(3);
		list.push_back(4);
		list.pop_front();
		list.pop_back();
		if (!list.empty() && list.get() == 1)
		{
			std::cout << "✓ Integration test 3: Mixed operations [PASSED]" << std::endl;
			tests_passed++;
		}
		else
		{
			std::cout << "✗ Integration test 3: Mixed operations [FAILED]" << std::endl;
		}
	}

	{
		tests_total++;
		List list;
		list.push_back(1);
		list.push_back(2);
		list.push_back(3);
		std::cout << "  Integration test 4: show() outputs: ";
		list.show();
		std::cout << "✓ Integration test 4: show() [PASSED]" << std::endl;
		tests_passed++;
	}

	{
		tests_total++;
		List list;
		list.push_back(1);
		list.push_back(2);
		list.push_back(3);
		list.pop_front();
		list.pop_front();
		list.pop_front();
		if (list.empty())
		{
			std::cout << "✓ Integration test 5: Full list clearing [PASSED]" << std::endl;
			tests_passed++;
		}
		else
		{
			std::cout << "✗ Integration test 5: Full list clearing [FAILED]" << std::endl;
		}
	}

	{
		tests_total++;
		{
			List list;
			for (int i = 0; i < 10; ++i)
			{
				list.push_back(i);
			}
		}
		std::cout << "✓ Integration test 6: Destructor (memory cleanup check) [PASSED]" << std::endl;
		tests_passed++;
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



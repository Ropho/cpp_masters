#include <iostream>
#include <cstdint>

class IPv4
{
private:
	std::uint8_t m_octets[4];

	void increment(){
		for (int i = 3; i >= 0; --i)

			if (m_octets[i] < 255){
				++m_octets[i];
				return;
			} else {
				m_octets[i] = 0;
			}
	}

	void decrement()
	{
		for (int i = 3; i >= 0; --i)
			if (m_octets[i] > 0){
				--m_octets[i];
				return;
			}
			else {
				m_octets[i] = 255;
			}
	}

public:
	IPv4() : m_octets{0, 0, 0, 0} {}

	IPv4(std::uint8_t a, std::uint8_t b, std::uint8_t c, std::uint8_t d)
		: m_octets{a, b, c, d} {}

	IPv4& operator++() {
		increment();
		return *this;
	}

	IPv4 operator++(int){
		IPv4 temp = *this;
		increment();
		return temp;
	}

	IPv4& operator--(){
		decrement();
		return *this;
	}

	IPv4 operator--(int){
		IPv4 temp = *this;
		decrement();
		return temp;
	}

	friend bool operator==(const IPv4& lhs, const IPv4& rhs);
	friend bool operator!=(const IPv4& lhs, const IPv4& rhs);
	friend bool operator<(const IPv4& lhs, const IPv4& rhs);
	friend bool operator>(const IPv4& lhs, const IPv4& rhs);
	friend bool operator<=(const IPv4& lhs, const IPv4& rhs);
	friend bool operator>=(const IPv4& lhs, const IPv4& rhs);
	friend std::istream& operator>>(std::istream& is, IPv4& ip);
	friend std::ostream& operator<<(std::ostream& os, const IPv4& ip);
};

bool operator==(const IPv4& lhs, const IPv4& rhs) {
	return lhs.m_octets[0] == rhs.m_octets[0] &&
	       lhs.m_octets[1] == rhs.m_octets[1] &&
	       lhs.m_octets[2] == rhs.m_octets[2] &&
	       lhs.m_octets[3] == rhs.m_octets[3];
}

bool operator!=(const IPv4& lhs, const IPv4& rhs) {
	return !(lhs == rhs);
}

bool operator<(const IPv4& lhs, const IPv4& rhs) {
	for (int i = 0; i < 4; ++i) {
		if (lhs.m_octets[i] < rhs.m_octets[i]) {
			return true;
		}
		if (lhs.m_octets[i] > rhs.m_octets[i]) {
			return false;
		}
	}
	return false;
}

bool operator>(const IPv4& lhs, const IPv4& rhs) {
	return rhs < lhs;
}

bool operator<=(const IPv4& lhs, const IPv4& rhs) {
	return !(lhs > rhs);
}

bool operator>=(const IPv4& lhs, const IPv4& rhs) {
	return !(lhs < rhs);
}

std::istream& operator>>(std::istream& is, IPv4& ip)
{
	int a, b, c, d;
	char dot1, dot2, dot3;

	if (is >> a >> dot1 >> b >> dot2 >> c >> dot3 >> d) {
		if (dot1 == '.' && dot2 == '.' && dot3 == '.' &&
		    a >= 0 && a <= 255 &&
		    b >= 0 && b <= 255 &&
		    c >= 0 && c <= 255 &&
		    d >= 0 && d <= 255) {
			ip.m_octets[0] = static_cast<std::uint8_t>(a);
			ip.m_octets[1] = static_cast<std::uint8_t>(b);
			ip.m_octets[2] = static_cast<std::uint8_t>(c);
			ip.m_octets[3] = static_cast<std::uint8_t>(d);
		}
		else{
			is.setstate(std::ios::failbit);
		}
	}

	return is;
}

std::ostream& operator<<(std::ostream& os, const IPv4& ip) {
	os << static_cast<int>(ip.m_octets[0]) << '.'
	   << static_cast<int>(ip.m_octets[1]) << '.'
	   << static_cast<int>(ip.m_octets[2]) << '.'
	   << static_cast<int>(ip.m_octets[3]);
	return os;
}

void test_increment_decrement(){
	std::cout << "=== Increment and Decrement Test ===" << std::endl;

	IPv4 ip(192, 168, 1, 1);
	std::cout << "Initial address: " << ip << std::endl;

	++ip;
	std::cout << "After ++ip: " << ip << std::endl;

	ip++;
	std::cout << "After ip++: " << ip << std::endl;

	--ip;
	std::cout << "After --ip: " << ip << std::endl;

	ip--;
	std::cout << "After ip--: " << ip << std::endl;

	std::cout << "\nOverflow test:" << std::endl;
	IPv4 max_ip(255, 255, 255, 255);
	std::cout << "Maximum address: " << max_ip << std::endl;
	++max_ip;
	std::cout << "After ++: " << max_ip << std::endl;

	IPv4 min_ip(0, 0, 0, 0);
	std::cout << "Minimum address: " << min_ip << std::endl;
	--min_ip;
	std::cout << "After --: " << min_ip << std::endl;
}

    void test_comparison()
{
	std::cout << "\n=== Comparison Operators Test ===" << std::endl;

	IPv4 ip1(192, 168, 1, 1);
	IPv4 ip2(192, 168, 1, 2);
	IPv4 ip3(192, 168, 1, 1);

	std::cout << "ip1 = " << ip1 << std::endl;
	std::cout << "ip2 = " << ip2 << std::endl;
	std::cout << "ip3 = " << ip3 << std::endl;

	std::cout << "\nip1 == ip2: " << (ip1 == ip2) << std::endl;
	std::cout << "ip1 == ip3: " << (ip1 == ip3) << std::endl;
	std::cout << "ip1 != ip2: " << (ip1 != ip2) << std::endl;
	std::cout << "ip1 < ip2: " << (ip1 < ip2) << std::endl;
	std::cout << "ip1 > ip2: " << (ip1 > ip2) << std::endl;
	std::cout << "ip1 <= ip2: " << (ip1 <= ip2) << std::endl;
	std::cout << "ip1 >= ip2: " << (ip1 >= ip2) << std::endl;
}

void test_input_output()
{
	std::cout << "\n=== Input and Output Test ===" << std::endl;

	IPv4 ip;
	std::cout << "Enter IP address in format a.b.c.d (e.g., 10.0.0.1): ";
	std::cin >> ip;

	if (std::cin.good())
	{
		std::cout << "Entered address: " << ip << std::endl;
	}
	else
	{
		std::cout << "Input error!" << std::endl;
		std::cin.clear();
		std::cin.ignore(10000, '\n');
	}
}

int main()
{
	test_increment_decrement();
	test_comparison();
	test_input_output();

	return 0;
}


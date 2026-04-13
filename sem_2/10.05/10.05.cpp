#include <boost/container_hash/hash.hpp>

#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <functional>
#include <iostream>
#include <random>
#include <unordered_map>
#include <vector>

namespace
{

	template <typename T>
	auto hash_variadic(T x, auto... xs)
	{
		auto seed = std::hash<T>{}(x);

		if constexpr (sizeof...(xs) > 0)
		{
			seed += hash_variadic(xs...) * 31;
		}

		return seed;
	}

	struct Entity
	{
		int x = 0;
		int y = 0;
	};

	auto hash_value(Entity const & entity)
	{
		std::size_t seed = 0;

		boost::hash_combine(seed, entity.x);
		boost::hash_combine(seed, entity.y);

		return seed;
	}

	void demo_from_example_10_41()
	{
		std::ignore = hash_variadic(1, 2.0, std::string{"aaaaa"});

		std::ignore = boost::hash<Entity>{}(Entity{1, 1});

		std::vector<int> vector = {1, 2, 3, 4, 5};

		std::ignore = boost::hash_range(vector.begin(), vector.end());
	}

unsigned int RSHash(char const * str, unsigned int length)
{
	unsigned int b = 378551;
	unsigned int a = 63689;
	unsigned int hash = 0;

	for (unsigned int i = 0; i < length; ++str, ++i)
	{
		hash = hash * a + static_cast<unsigned char>(*str);
		a = a * b;
	}

	return hash;
}

unsigned int JSHash(char const * str, unsigned int length)
{
	unsigned int hash = 1315423911;

	for (unsigned int i = 0; i < length; ++str, ++i)
	{
		hash ^= ((hash << 5) + static_cast<unsigned char>(*str) + (hash >> 2));
	}

	return hash;
}

unsigned int PJWHash(char const * str, unsigned int length)
{
	unsigned int const bits_in_unsigned_int =
		static_cast<unsigned int>(sizeof(unsigned int) * 8U);
	unsigned int const three_quarters =
		static_cast<unsigned int>((bits_in_unsigned_int * 3U) / 4U);
	unsigned int const one_eighth =
		static_cast<unsigned int>(bits_in_unsigned_int / 8U);
	unsigned int const high_bits =
		static_cast<unsigned int>(0xFFFFFFFFU) << (bits_in_unsigned_int - one_eighth);

	unsigned int hash = 0;
	unsigned int test = 0;

	for (unsigned int i = 0; i < length; ++str, ++i)
	{
		hash = (hash << one_eighth) + static_cast<unsigned char>(*str);

		if ((test = hash & high_bits) != 0)
		{
			hash = (hash ^ (test >> three_quarters)) & (~high_bits);
		}
	}

	return hash;
}

unsigned int ELFHash(char const * str, unsigned int length)
{
	unsigned int hash = 0;
	unsigned int x = 0;

	for (unsigned int i = 0; i < length; ++str, ++i)
	{
		hash = (hash << 4) + static_cast<unsigned char>(*str);

		if ((x = hash & 0xF0000000U) != 0)
		{
			hash ^= (x >> 24);
		}

		hash &= ~x;
	}

	return hash;
}

unsigned int BKDRHash(char const * str, unsigned int length)
{
	unsigned int const seed = 131;
	unsigned int hash = 0;

	for (unsigned int i = 0; i < length; ++str, ++i)
	{
		hash = (hash * seed) + static_cast<unsigned char>(*str);
	}

	return hash;
}

unsigned int SDBMHash(char const * str, unsigned int length)
{
	unsigned int hash = 0;

	for (unsigned int i = 0; i < length; ++str, ++i)
	{
		unsigned int const ch = static_cast<unsigned char>(*str);
		hash = ch + (hash << 6) + (hash << 16) - hash;
	}

	return hash;
}

unsigned int DJBHash(char const * str, unsigned int length)
{
	unsigned int hash = 5381;

	for (unsigned int i = 0; i < length; ++str, ++i)
	{
		hash = ((hash << 5) + hash) + static_cast<unsigned char>(*str);
	}

	return hash;
}

unsigned int DEKHash(char const * str, unsigned int length)
{
	unsigned int hash = length;

	for (unsigned int i = 0; i < length; ++str, ++i)
	{
		hash = ((hash << 5) ^ (hash >> 27)) ^ static_cast<unsigned char>(*str);
	}

	return hash;
}

unsigned int APHash(char const * str, unsigned int length)
{
	unsigned int hash = 0xAAAAAAAAU;

	for (unsigned int i = 0; i < length; ++str, ++i)
	{
		unsigned int const ch = static_cast<unsigned char>(*str);

		if ((i & 1U) == 0)
		{
			hash ^= ((hash << 7) + (ch * (hash >> 3)));
		}
		else
		{
			hash ^= (~((hash << 11) + (ch ^ (hash >> 5))));
		}
	}

	return hash;
}

using HashFn = unsigned int (*)(char const *, unsigned int);

struct HashDesc
{
	char const * name;
	HashFn fn;
};

std::array<HashDesc, 9> const k_hashes = {{
	{"RS", RSHash},
	{"JS", JSHash},
	{"PJW", PJWHash},
	{"ELF", ELFHash},
	{"BKDR", BKDRHash},
	{"SDBM", SDBMHash},
	{"DJB", DJBHash},
	{"DEK", DEKHash},
	{"AP", APHash},
}};

std::uint64_t count_collision_pairs(std::vector<unsigned int> const & values)
{
	std::unordered_map<unsigned int, int> freq;
	freq.reserve(values.size() * 2);

	for (unsigned int const v : values)
	{
		++freq[v];
	}

	std::uint64_t pairs = 0;

	for (auto const & entry : freq)
	{
		long long const c = entry.second;
		pairs += static_cast<std::uint64_t>(c) * static_cast<std::uint64_t>(c - 1) / 2U;
	}

	return pairs;
}

std::vector<std::string> make_random_strings(
	std::size_t const count,
	std::size_t const str_len,
	std::uint64_t const seed)
{
	std::vector<std::string> out;
	out.reserve(count);

	static char constexpr k_alphabet[] = "abcdefghijklmnopqrstuvwxyz";

	std::mt19937_64 engine(seed);
	std::uniform_int_distribution<unsigned> dist(0U, 25U);

	while (out.size() < count)
	{
		std::string s(str_len, 'a');

		for (std::size_t i = 0; i < str_len; ++i)
		{
			s[i] = k_alphabet[dist(engine)];
		}

		out.push_back(std::move(s));
	}

	return out;
}

void run_study(
	std::vector<std::size_t> const & sample_sizes,
	std::size_t const str_len,
	std::uint64_t const seed,
	std::ostream & csv)
{
	csv << "N";

	for (HashDesc const & h : k_hashes)
	{
		csv << ',' << h.name;
	}

	csv << '\n';

	std::size_t const max_n = sample_sizes.back();
	std::vector<std::string> const strings = make_random_strings(max_n, str_len, seed);

	for (std::size_t const n : sample_sizes)
	{
		assert(n <= strings.size());
		csv << n;

		for (HashDesc const & h : k_hashes)
		{
			std::vector<unsigned int> hashes;
			hashes.reserve(n);

			for (std::size_t i = 0; i < n; ++i)
			{
				std::string const & s = strings[i];
				unsigned int const v = h.fn(s.data(), static_cast<unsigned int>(s.size()));
				hashes.push_back(v);
			}

			csv << ',' << count_collision_pairs(hashes);
		}

		csv << '\n';
	}
}

void print_ranking_at_last_row(
	std::vector<std::size_t> const & sample_sizes,
	std::size_t const str_len,
	std::uint64_t const seed)
{
	std::size_t const max_n = sample_sizes.back();
	std::vector<std::string> const strings = make_random_strings(max_n, str_len, seed);

	std::vector<std::pair<std::uint64_t, char const *>> scores;
	scores.reserve(k_hashes.size());

	for (HashDesc const & h : k_hashes)
	{
		std::vector<unsigned int> hashes;
		hashes.reserve(max_n);

		for (std::size_t i = 0; i < max_n; ++i)
		{
			std::string const & s = strings[i];
			hashes.push_back(h.fn(s.data(), static_cast<unsigned int>(s.size())));
		}

		scores.emplace_back(count_collision_pairs(hashes), h.name);
	}

	std::sort(scores.begin(), scores.end());

	std::cout << "At N=" << max_n << " (pseudo-random " << str_len
		  << "-char strings, seed=" << seed
		  << "), pairwise hash collisions (lower is better):\n";

	for (auto const & p : scores)
	{
		std::cout << "  " << p.second << '\t' << p.first << '\n';
	}

	std::uint64_t const min_c = scores.front().first;
	std::uint64_t const max_c = scores.back().first;

	std::cout << "Best (fewest collisions, value=" << min_c << "): ";

	bool first = true;

	for (auto const & p : scores)
	{
		if (p.first != min_c)
		{
			break;
		}

		if (!first)
		{
			std::cout << ", ";
		}

		first = false;
		std::cout << p.second;
	}

	std::cout << '\n';

	std::cout << "Worst (most collisions, value=" << max_c << "): ";

	first = true;

	for (auto const & p : scores)
	{
		if (p.first != max_c)
		{
			continue;
		}

		if (!first)
		{
			std::cout << ", ";
		}

		first = false;
		std::cout << p.second;
	}

	std::cout << '\n';
	std::cout
		<< "Expect collisions to grow faster-than-linear in N (birthday effect): "
		   "probability of duplicate 32-bit hash grows roughly like N^2 for large N.\n";
}
} // namespace

int main(int argc, char * argv[])
{
	demo_from_example_10_41();

	char const * csv_path = "hash_collision_data.csv";
	std::uint64_t seed = 42;
	std::size_t str_len = 8;

	std::vector<std::size_t> sample_sizes = {
		500,	1000,  2500,   5000,  10000,
		20000,  40000,  80000,  120000, 180000,
	};

	for (int i = 1; i < argc; ++i)
	{
		std::string const arg = argv[i];

		if (arg == "--csv" && i + 1 < argc)
		{
			csv_path = argv[++i];
		}
		else if (arg == "--seed" && i + 1 < argc)
		{
			seed = std::stoull(argv[++i]);
		}
		else if (arg == "--len" && i + 1 < argc)
		{
			str_len = static_cast<std::size_t>(std::stoull(argv[++i]));
		}
	}

	{
		std::ofstream csv(csv_path);
		if (!csv)
		{
			std::cerr << "cannot open " << csv_path << '\n';
			return 1;
		}

		run_study(sample_sizes, str_len, seed, csv);
	}

	std::cout << "Wrote " << csv_path << " (columns: N + 9 hash families from Partow).\n";
	std::cout << "Plot each hash column vs N to compare collision growth.\n\n";

	print_ranking_at_last_row(sample_sizes, str_len, seed);

	return 0;
}

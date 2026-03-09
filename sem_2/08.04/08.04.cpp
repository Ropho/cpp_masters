///////////////////////////////////////////////////////////////////////////////////////////

#include <cstddef>
#include <iostream>
#include <random>
#include <string>
#include <vector>

///////////////////////////////////////////////////////////////////////////////////////////

auto hamming_distance(std::string const & a, std::string const & b) -> std::size_t
{
	std::size_t distance = 0;

	for (std::size_t i = 0; i < a.size(); ++i)
	{
		if (a[i] != b[i]) { ++distance; }
	}

	return distance;
}

///////////////////////////////////////////////////////////////////////////////////////////

int main()
{
	const std::string target = "methinksitislikeaweasel";

	const auto length = target.size();

//  ---------------------------------------------------------------------------------------

	std::random_device device;

	std::default_random_engine engine(device());

	std::uniform_int_distribution  < int    > letter_distribution(0, 25);

	std::uniform_real_distribution < double > mutation_distribution(0.0, 1.0);

//  ---------------------------------------------------------------------------------------

	std::string current;

	current.reserve(length);

	for (std::size_t i = 0; i < length; ++i)
	{
		current += static_cast < char > ('a' + letter_distribution(engine));
	}

//  ---------------------------------------------------------------------------------------

	const std::size_t copies = 100;

	const double mutation_rate = 0.05;

//  ---------------------------------------------------------------------------------------

	for (auto iteration = 0uz; ; ++iteration)
	{
		auto distance = hamming_distance(current, target);

		std::cout << "iteration " << iteration << " : " << current

			<< " (distance = " << distance << ")" << std::endl;

		if (distance == 0) { break; }

//      -----------------------------------------------------------------------------------

		std::vector < std::string > population(copies);

		for (auto & copy : population)
		{
			copy.reserve(length);

			for (std::size_t i = 0; i < length; ++i)
			{
				if (mutation_distribution(engine) < mutation_rate)
				{
					copy += static_cast < char > ('a' + letter_distribution(engine));
				}
				else
				{
					copy += current[i];
				}
			}
		}

//      -----------------------------------------------------------------------------------

		auto best_distance = hamming_distance(population[0], target);

		auto best_index = 0uz;

		for (auto i = 1uz; i < copies; ++i)
		{
			auto d = hamming_distance(population[i], target);

			if (d < best_distance)
			{
				best_distance = d;

				best_index = i;
			}
		}

		current = population[best_index];
	}
}

///////////////////////////////////////////////////////////////////////////////////////////

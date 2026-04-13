#include <boost/multi_array.hpp>

#include <cassert>
#include <cstddef>
#include <iostream>
#include <utility>

namespace
{
	constexpr std::size_t k_rows = 10;
	constexpr std::size_t k_cols = 10;
	constexpr char k_live = '#';
	constexpr char k_dead = '.';
	constexpr int k_generations = 24;

	using field_t = boost::multi_array<char, 2>;

	bool in_bounds(std::size_t const row, std::size_t const col)
	{
		return row < k_rows && col < k_cols;
	}

	bool is_live(field_t const & field, std::size_t const row, std::size_t const col)
	{
		if (!in_bounds(row, col))
		{
			return false;
		}

		return field[row][col] == k_live;
	}

	int count_neighbors(field_t const & field, std::size_t const row, std::size_t const col)
	{
		int count = 0;

		for (int dr = -1; dr <= 1; ++dr)
		{
			for (int dc = -1; dc <= 1; ++dc)
			{
				if (dr == 0 && dc == 0)
				{
					continue;
				}

				auto const nr = static_cast<std::ptrdiff_t>(row) + dr;
				auto const nc = static_cast<std::ptrdiff_t>(col) + dc;

				if (nr < 0 || nc < 0)
				{
					continue;
				}

				auto const ur = static_cast<std::size_t>(nr);
				auto const uc = static_cast<std::size_t>(nc);

				if (is_live(field, ur, uc))
				{
					++count;
				}
			}
		}

		return count;
	}

	void place_glider(field_t & field)
	{
		assert(k_rows >= 3 && k_cols >= 3);

		field[0][1] = k_live;
		field[1][2] = k_live;
		field[2][0] = k_live;
		field[2][1] = k_live;
		field[2][2] = k_live;
	}

	void print_field(field_t const & field, int const generation)
	{
		std::cout << "generation " << generation << '\n';

		for (std::size_t r = 0; r < k_rows; ++r)
		{
			for (std::size_t c = 0; c < k_cols; ++c)
			{
				std::cout << field[r][c];
			}

			std::cout << '\n';
		}

		std::cout << '\n';
	}

	void step(field_t const & current, field_t & next)
	{
		for (std::size_t r = 0; r < k_rows; ++r)
		{
			for (std::size_t c = 0; c < k_cols; ++c)
			{
				int const neighbors = count_neighbors(current, r, c);
				bool const alive = current[r][c] == k_live;

				if (alive)
				{
					next[r][c] = (neighbors == 2 || neighbors == 3) ? k_live : k_dead;
				}
				else
				{
					next[r][c] = (neighbors == 3) ? k_live : k_dead;
				}
			}
		}
	}
}

int main()
{
	field_t storage_a(boost::extents[k_rows][k_cols]);
	field_t storage_b(boost::extents[k_rows][k_cols]);

	field_t * current = &storage_a;
	field_t * next = &storage_b;

	for (std::size_t r = 0; r < k_rows; ++r)
	{
		for (std::size_t c = 0; c < k_cols; ++c)
		{
			(*current)[r][c] = k_dead;
		}
	}

	place_glider(*current);

	for (int g = 0; g <= k_generations; ++g)
	{
		print_field(*current, g);
		step(*current, *next);
		std::swap(current, next);
	}

	return 0;
}

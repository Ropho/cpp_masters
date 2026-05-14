#include <boost/graph/adjacency_matrix.hpp>
#include <boost/graph/properties.hpp>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <limits>
#include <numeric>
#include <random>
#include <vector>

namespace
{
constexpr std::size_t vertex_count = 10;

using EdgeWeight = boost::property<boost::edge_weight_t, int>;
using Graph = boost::adjacency_matrix<boost::undirectedS,
                                      boost::no_property,
                                      EdgeWeight>;
using Vertex = boost::graph_traits<Graph>::vertex_descriptor;
using Route = std::vector<Vertex>;

auto generate_graph() -> Graph
{
	Graph graph(vertex_count);

	std::random_device device;
	std::default_random_engine generator(device());
	std::uniform_int_distribution<int> distribution(1, 10);

	for (std::size_t i = 0; i < vertex_count; ++i)
	{
		for (std::size_t j = i + 1; j < vertex_count; ++j)
		{
			boost::add_edge(i, j, EdgeWeight(distribution(generator)), graph);
		}
	}

	return graph;
}

auto edge_weight(Graph const & graph, Vertex from, Vertex to) -> int
{
	auto const edge = boost::edge(from, to, graph);
	assert(edge.second);

	auto const weights = boost::get(boost::edge_weight, graph);
	return boost::get(weights, edge.first);
}

auto route_cost(Graph const & graph, Route const & route) -> int
{
	assert(route.size() == vertex_count);

	int cost = 0;

	for (std::size_t i = 0; i + 1 < route.size(); ++i)
	{
		cost += edge_weight(graph, route[i], route[i + 1]);
	}

	cost += edge_weight(graph, route.back(), route.front());

	return cost;
}

auto solve_travelling_salesman(Graph const & graph) -> std::pair<Route, int>
{
	Route route(vertex_count);
	std::iota(route.begin(), route.end(), 0);

	Route best_route = route;
	int best_cost = std::numeric_limits<int>::max();

	do
	{
		auto const cost = route_cost(graph, route);

		if (cost < best_cost)
		{
			best_cost = cost;
			best_route = route;
		}
	}
	while (std::next_permutation(std::next(route.begin()), route.end()));

	return {best_route, best_cost};
}

void print_adjacency_matrix(Graph const & graph)
{
	std::cout << "Adjacency matrix:" << '\n';

	for (std::size_t i = 0; i < vertex_count; ++i)
	{
		for (std::size_t j = 0; j < vertex_count; ++j)
		{
			if (i == j)
			{
				std::cout << 0;
			}
			else
			{
				std::cout << edge_weight(graph, i, j);
			}

			if (j + 1 < vertex_count)
			{
				std::cout << ' ';
			}
		}

		std::cout << '\n';
	}
}

void print_route(Route const & route)
{
	std::cout << "Best route:" << '\n';

	for (auto const vertex : route)
	{
		std::cout << vertex << " -> ";
	}

	std::cout << route.front() << '\n';
}
} // namespace

int main()
{
	auto const graph = generate_graph();
	auto const [best_route, best_cost] = solve_travelling_salesman(graph);

	print_adjacency_matrix(graph);
	print_route(best_route);
	std::cout << "Best cost: " << best_cost << '\n';

	return 0;
}

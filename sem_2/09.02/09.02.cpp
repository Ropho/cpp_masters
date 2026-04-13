#include <cassert>
#include <iostream>
#include <memory>
#include <queue>

namespace
{
	constexpr int k_value_root = 1;
	constexpr int k_value_level1_left = 2;
	constexpr int k_value_level1_right = 3;
	constexpr int k_value_leaf_a = 4;
	constexpr int k_value_leaf_b = 5;
	constexpr int k_value_leaf_c = 6;
	constexpr int k_value_leaf_d = 7;
}

class Tree
{
public:

	struct Node
	{
		int value{};
		std::shared_ptr<Node> left{};
		std::shared_ptr<Node> right{};
		std::weak_ptr<Node> parent{};

		~Node()
		{
			std::cout << "Node::~Node value=" << value << '\n';
		}
	};

	std::shared_ptr<Node> root{};

	void traverse_v1() const
	{
		if (!root)
		{
			return;
		}

		std::queue<std::shared_ptr<Node>> frontier;
		frontier.push(root);

		while (!frontier.empty())
		{
			std::shared_ptr<Node> const current = frontier.front();
			frontier.pop();

			std::cout << current->value << ' ';

			if (current->left)
			{
				frontier.push(current->left);
			}

			if (current->right)
			{
				frontier.push(current->right);
			}
		}

		std::cout << '\n';
	}

	void traverse_v2() const
	{
		traverse_depth_preorder(root);
		std::cout << '\n';
	}

private:

	static void traverse_depth_preorder(std::shared_ptr<Node> const & node)
	{
		if (!node)
		{
			return;
		}

		std::cout << node->value << ' ';
		traverse_depth_preorder(node->left);
		traverse_depth_preorder(node->right);
	}
};

static std::shared_ptr<Tree::Node> make_child(int value, std::shared_ptr<Tree::Node> const & parent)
{
	auto child = std::make_shared<Tree::Node>();
	child->value = value;
	child->parent = parent;
	return child;
}

int main()
{
	Tree tree;

	auto root = std::make_shared<Tree::Node>();
	root->value = k_value_root;

	auto level1_left = make_child(k_value_level1_left, root);
	auto level1_right = make_child(k_value_level1_right, root);
	root->left = level1_left;
	root->right = level1_right;

	auto leaf_a = make_child(k_value_leaf_a, level1_left);
	auto leaf_b = make_child(k_value_leaf_b, level1_left);
	level1_left->left = leaf_a;
	level1_left->right = leaf_b;

	auto leaf_c = make_child(k_value_leaf_c, level1_right);
	auto leaf_d = make_child(k_value_leaf_d, level1_right);
	level1_right->left = leaf_c;
	level1_right->right = leaf_d;

	tree.root = root;
	root.reset();
	level1_left.reset();
	level1_right.reset();
	leaf_a.reset();
	leaf_b.reset();
	leaf_c.reset();
	leaf_d.reset();

	std::cout << "breadth_first: ";
	tree.traverse_v1();

	std::cout << "depth_first_preorder: ";
	tree.traverse_v2();

	std::shared_ptr<Tree::Node> const sample_leaf = tree.root->left->left;
	std::shared_ptr<Tree::Node> const parent_via_weak = sample_leaf->parent.lock();
	assert(parent_via_weak);
	assert(parent_via_weak.get() == tree.root->left.get());
	std::cout << "weak_ptr parent of leaf: value=" << parent_via_weak->value
		<< " use_count=" << parent_via_weak.use_count() << '\n';

	std::cout << "reset root (destructors should run; no shared_ptr cycles):\n";
	tree.root.reset();

	return 0;
}

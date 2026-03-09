#include <algorithm>
#include <climits>
#include <vector>

struct Rectangle
{
	int left;
	int top;
	int right;
	int bottom;
};

int area(const Rectangle& rect)
{
	if (rect.right <= rect.left || rect.bottom <= rect.top) {
		return 0;
	}
	return (rect.right - rect.left) * (rect.bottom - rect.top);
}

Rectangle intersect(const Rectangle& rect1, const Rectangle& rect2) {
	Rectangle result;
	result.left = std::max(rect1.left, rect2.left);
	result.top = std::max(rect1.top, rect2.top);
	result.right = std::min(rect1.right, rect2.right);
	result.bottom = std::min(rect1.bottom, rect2.bottom);

	if (result.right <= result.left || result.bottom <= result.top) {
		result.left = result.right = 0;
		result.top = result.bottom = 0;
	}

	return result;
}

std::size_t intersection_area(const std::vector<Rectangle>& rectangles)
{
	if (rectangles.empty()) {
		return 0;
	}

	if (rectangles.size() == 1) {
		return area(rectangles[0]);
	}

	Rectangle intersection = rectangles[0];

	for (std::size_t i = 1; i < rectangles.size(); ++i) {
		intersection = intersect(intersection, rectangles[i]);

		if (area(intersection) == 0) {
			return 0;
		}
	}

	return area(intersection);
}

Rectangle bounding_box(const std::vector<Rectangle>& rectangles) {
	if (rectangles.empty()) {
		return {0, 0, 0, 0};
	}

	Rectangle result;
	result.left = INT_MAX;
	result.top = INT_MAX;
	result.right = INT_MIN;
	result.bottom = INT_MIN;

	for (const auto& rect : rectangles) {
		result.left = std::min(result.left, rect.left);
		result.top = std::min(result.top, rect.top);
		result.right = std::max(result.right, rect.right);
		result.bottom = std::max(result.bottom, rect.bottom);
	}

	return result;
}

int main()
{
	std::vector<Rectangle> rects1 = {
		{0, 0, 10, 10},
		{5, 5, 15, 15}
	};

	int intersection1 = intersection_area(rects1);

	std::vector<Rectangle> rects2 = {
		{0, 0, 10, 10},
		{5, 5, 15, 15},
		{7, 7, 12, 12}
	};

	int intersection2 = intersection_area(rects2);

	std::vector<Rectangle> rects3 = {
		{0, 0, 5, 5},
		{10, 10, 15, 15}
	};

	int intersection3 = intersection_area(rects3);

	std::vector<Rectangle> rects4 = {
		{0, 0, 10, 10},
		{10, 10, 20, 20}
	};

	int intersection4 = intersection_area(rects4);

	std::vector<Rectangle> rects5 = {
		{0, 0, 5, 5},
		{10, 10, 15, 15},
		{20, 20, 25, 25}
	};

	Rectangle bbox = bounding_box(rects5);

	return 0;
}


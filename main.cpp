#include "CList.h"

int main()
{
	CList a{ 1, 3, 5 };
	a.push_back(7);
	a.push_back(9);
	a.push_front(2);
	a.push_front(4);
	a.push_front(6);
	a.insert(a.end(), 100);
	a.erase(--a.end());
	
	for (auto it = a.crbegin(); it != a.crend(); ++it)
	{
		std::cout << "[" << *it << "]";
	}

	return 0;
}
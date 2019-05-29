#pragma once
#include <memory>
#include "Node.h"
struct DistComparator {
	bool operator()(const std::shared_ptr<Node> &n1, const std::shared_ptr<Node> &n2) {
		return n1->getTentativeDist() > n2->getTentativeDist();
	}
};

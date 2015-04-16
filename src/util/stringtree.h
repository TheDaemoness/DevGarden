#ifndef STRINGTREE_H
#define STRINGTREE_H

#include <string>
#include <cstring>
#include <vector>

template<typename Value>
class StringTree {
	template<typename Value>
	struct StringTreeNode {
		Value value;
		std::string key;
		std::vector<StringTreeNode<Value>*> branches;
		StringTree(const std::string& k, const Value& v) {
			key = k;
			value = v;
		}
		~StringTreeNode() {
			delete[] branches;
		}
	};
	std::vector<StringTreeNode<Value>*> branches;
public:
	StringTree(size_t prealloc = 8) {
		branches.reserve(prealloc);
	}
	void insert(char* key, const Value& val, char delim = '.') {
		const char* index = std::strchr(key, delim);
		if(index)
	}
};

#endif // STRINGTREE_H

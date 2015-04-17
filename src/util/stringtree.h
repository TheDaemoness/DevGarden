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
	bool insert(const char* key, const Value& val, char delim = '.') {
		const char* index = std::strchr(key, delim);
		return false;
	}
	bool remove(const char* key, Value* buffer = nullptr) {
		return true;
	}
	void clear() {
		for(auto v : branches)
			delete v;
		branches.clear();
	}
	Value* get(const char* key) {
		return nullptr;
	}
};

#endif // STRINGTREE_H

#ifndef TRIE_H
#define TRIE_H
#include <vector>
#include <string>
using namespace std;

class Trie
{
private:
	class element
	{
	public:
		bool isEnd;
		int prefixCount;
		int childrenIndex;
		element(){ isEnd = false; prefixCount = 0; childrenIndex = -1; }
	};
	element root;
	vector< vector<element> > children;
public:
	void insert(string s)
	{
		element *l = &root;
		for (int i = 0; i < s.size(); i++){
			if (l->childrenIndex == -1){
				children.push_back(vector<element>(26));
				l->childrenIndex = children.size() - 1;
			}
			l->prefixCount++;
			l = &children[l->childrenIndex][s[i] - 'a'];
		}
		l->prefixCount++; //
		l->isEnd = true;
	}
	bool search(string s)
	{
		element *l = &root;
		for (int i = 0; i < s.size(); i++){
			if (l->childrenIndex == -1)
				return false;
			l = &children[l->childrenIndex][s[i] - 'a'];
		}
		return l->isEnd;
	}
	int wordsWithPrefix(string s)
	{
		element *l = &root;
		for (int i = 0; i < s.size(); i++){
			if (l->childrenIndex == -1)
				return 0;
			l = &children[l->childrenIndex][s[i] - 'a'];
		}
		return l->prefixCount;
	}

};

#endif
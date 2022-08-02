#ifndef TRIE_H
#define TRIE_H

#include <stdexcept>
#include <vector>
#include <memory>
#include <unordered_map>

/*
* It turns out that the Trie structure is useless because it's slower than std::unordered_set
*/

template<class T>
using Dictionary = std::vector<T>;

template<class Key>
class TrieSet
{
public:
	/*
	* Key type must have defined ()++ operator
	*/
	TrieSet(const Key & startingKey, const std::size_t & dictionaryLength);
	explicit TrieSet(const Dictionary<Key> & dictionary);


	void Insert(const std::vector<Key> & key);
	void Insert(const Key key[], const std::size_t & length);

	bool Contains(const std::vector<Key> & key) const;
	bool Contains(const Key key[], const std::size_t& length) const;

	std::size_t GetSize() const;
private:
	TrieSet();

	struct Node
	{
		Node(const std::size_t & dictionarySize);

		std::vector<std::unique_ptr<Node>> m_children;
		bool m_isTerminal;
	};
	bool Contains(Node * node, const std::vector<Key>& key) const;
	bool Contains(Node* node, const Key key[], const std::size_t& length) const;

	void Insert(Node* node, const std::vector<Key> & value);
	void Insert(Node* node, const Key key[], const std::size_t & length);

	std::unique_ptr<Node> m_root;
	std::size_t m_dictionarySize;
	std::size_t m_size;

	std::unordered_map<Key, std::size_t> m_dictionary;
};

template<class Key>
inline TrieSet<Key>::TrieSet()
{
	m_size = 0;
}

template<class Key>
inline TrieSet<Key>::TrieSet(const Dictionary<Key>& dictionary)
{
	for (std::size_t i = 0; i < dictionary.size(); ++i)
	{
		m_dictionary[dictionary[i]] = i;
	}

	m_dictionarySize = m_dictionary.size();

	m_size = 0;
	m_root = std::make_unique<Node>(m_dictionarySize);
}

template<class Key>
inline TrieSet<Key>::TrieSet(const Key& startingKey, const std::size_t & dictionaryLength)
{
	Key k = startingKey;
	for (std::size_t i = 0; i < dictionaryLength; ++i)
	{
		m_dictionary[k] = i;
		k++;
	}

	m_dictionarySize = m_dictionary.size();

	m_size = 0;
	m_root = std::make_unique<Node>(m_dictionarySize);
}

template<class Key>
inline void TrieSet<Key>::Insert(const std::vector<Key> & key)
{
	Insert(m_root.get(), key);
}

template<class Key>
inline void TrieSet<Key>::Insert(const Key key[], const std::size_t& length)
{
	Insert(m_root.get(), key, length);
}

template<class Key>
inline void TrieSet<Key>::Insert(TrieSet<Key>::Node* node, const std::vector<Key> & key)
{
	if (node == nullptr)return;
	auto it = m_dictionary.end();

	for (const Key & k : key)
	{
		it = m_dictionary.find(k);

		if (it == m_dictionary.end())
		{
			throw std::out_of_range("Key is not in dictionary");
		}

		if (node->m_children[it->second] == nullptr) {
			node->m_children[it->second] = std::make_unique<Node>(m_dictionarySize);
		}
		node = node->m_children[it->second].get();
	}

	if(node->m_isTerminal == false)m_size++;

	node->m_isTerminal = true;
}

template<class Key>
inline void TrieSet<Key>::Insert(Node* node, const Key key[], const std::size_t& length)
{
	if (node == nullptr)return;
	auto it = m_dictionary.end();

	for (std::size_t i=0; i<length; ++i)
	{
		it = m_dictionary.find(key[i]);

		if (it == m_dictionary.end())
		{
			throw std::out_of_range("Key is not in dictionary");
		}

		if (node->m_children[it->second] == nullptr) {
			node->m_children[it->second] = std::make_unique<Node>(m_dictionarySize);
		}
		node = node->m_children[it->second].get();
	}

	if (node->m_isTerminal == false)m_size++;

	node->m_isTerminal = true;
}

template<class Key>
inline bool TrieSet<Key>::Contains(const std::vector<Key> & key) const
{
	return Contains(m_root.get(), key);
}

template<class Key>
inline bool TrieSet<Key>::Contains(const Key key[], const std::size_t& length) const
{
	return Contains(m_root.get(), key, length);
}

template<class Key>
inline std::size_t TrieSet<Key>::GetSize() const
{
	return m_size;
}

template<class Key>
inline bool TrieSet<Key>::Contains(Node* node, const std::vector<Key>& key) const
{
	if (node == nullptr)return false;
	auto it = m_dictionary.end();

	for (const Key& k : key)
	{
		it = m_dictionary.find(k);

		if (it == m_dictionary.end())
		{
			return false;
		}

		if (node->m_children[it->second] == nullptr)return false;
		else node = node->m_children[it->second].get();
	}

	return true;
}

template<class Key>
inline bool TrieSet<Key>::Contains(Node* node, const Key key[], const std::size_t& length) const
{
	if (node == nullptr)return false;
	auto it = m_dictionary.end();

	for (std::size_t i = 0; i < length; ++i)
	{
		it = m_dictionary.find(key[i]);

		if (it == m_dictionary.end())
		{
			return false;
		}

		if (node->m_children[it->second] == nullptr)return false;
		else node = node->m_children[it->second].get();
	}

	return true;
}

template<class Key>
inline TrieSet<Key>::Node::Node(const std::size_t & dictionarySize)
{
	m_children.resize(dictionarySize);
	m_isTerminal = false;
}

#endif
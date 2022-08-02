#ifndef TRIE_H
#define TRIE_H

#include <vector>
#include <memory>
/*
* It turns out that the Trie structure is useless because it's slower than std::unordered_set
*/

template<class Key>
class TrieSet
{
public:
	TrieSet(const std::size_t& size);

	constexpr void Insert(const Key & key);

	constexpr bool Contains(const Key & key);

	constexpr std::size_t GetSize() const;
private:
	struct Node
	{
		Node(const std::size_t & dictionarySize);

		std::vector<std::unique_ptr<Node>> m_children;
		bool m_isTerminal;
	};
	constexpr bool Contains(Node * node, const Key & key);
	constexpr void Insert(Node* node, const Key & value);

	std::unique_ptr<Node> m_root;
	std::size_t m_dictionarySize;
	std::size_t m_size;
};

template<class Key>
inline TrieSet<Key>::TrieSet(const std::size_t & size)
{
	m_dictionarySize = size;
	m_size = 0;
	m_root = std::make_unique<Node>(m_dictionarySize);
}

template<class Key>
inline constexpr void TrieSet<Key>::Insert(const Key& key)
{
	Insert(m_root.get(), key);
}

template<class Key>
constexpr inline void TrieSet<Key>::Insert(TrieSet<Key>::Node* node, const Key & key)
{
	if (node == nullptr)return;
	unsigned it = 0;
	for (const char& c : key)
	{
		it = c - 'a';
		if (node->m_children[it] == nullptr) {
			node->m_children[it] = std::make_unique<Node>(m_dictionarySize);
		}
		node = node->m_children[it].get();
	}

	if(node->m_isTerminal == false)m_size++;

	node->m_isTerminal = true;
}

template<class Key>
inline constexpr bool TrieSet<Key>::Contains(const Key & key)
{
	return Contains(m_root.get(), key);
}

template<class Key>
inline constexpr std::size_t TrieSet<Key>::GetSize() const
{
	return m_size;
}

template<class Key>
inline constexpr bool TrieSet<Key>::Contains(TrieSet<Key>::Node* node, const Key& key)
{
	if (node == nullptr)return false;
	unsigned it = 0;
	for (const char& c : key)
	{
		it = c - 'a';
		if (node->m_children[it] == nullptr)return false;
		else
			node = node->m_children[it].get();
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
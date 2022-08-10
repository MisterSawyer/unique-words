#ifndef TRIE_H
#define TRIE_H

#include <stdexcept>
#include <vector>
#include <memory>
#include <unordered_map>
#include <iterator>

template<class Key,
	class Hash = std::hash<Key>, 
	class SequenceContainer = std::vector<Key>,
	class KeyEqual = std::equal_to<Key>,
	class Allocator = std::allocator<std::pair<const Key, std::size_t>>>
class TrieSet
{
public:
	using allocator_type = Allocator;
	/*
	* To use 
	* Key type must have defined ()++ operator
	*/
	TrieSet(const Key & startingKey, const std::size_t & dictionarySize);
	explicit TrieSet(const SequenceContainer & dictorinary);

	virtual ~TrieSet();

	void Insert(const SequenceContainer& sequence);
	void Insert(const Key sequence[], const std::size_t & sequenceLength);

	bool Contains(const SequenceContainer& sequence) const;
	bool Contains(const Key sequence[], const std::size_t& sequenceLength) const;

	bool Empty() const;
	void Clear();

	std::size_t GetSize() const;

	//---
	allocator_type GetAllocator() const noexcept;
private:
	TrieSet();

	struct Node
	{
		Node(const std::size_t & dictionarySize);
		~Node();
		std::vector<std::unique_ptr<Node>> m_children;
		bool m_isTerminal;
	};

	bool Contains(Node * node, const SequenceContainer& sequence) const;
	bool Contains(Node* node, const Key sequence[], const std::size_t& sequenceLength) const;

	void Insert(Node* node, const SequenceContainer& value);
	void Insert(Node* node, const Key sequence[], const std::size_t & sequenceLength);

	Node * TryInsert(Node * node, const Key& value);

	std::unique_ptr<Node> m_root;
	std::size_t m_dictionarySize;
	std::size_t m_size;

	std::unordered_map<Key, std::size_t, Hash, KeyEqual, Allocator> m_dictionary;
};

/*
* Node
*/
template<class Key, class Hash, class SequenceContainer, class KeyEqual, class Allocator>
inline TrieSet<Key, Hash, SequenceContainer, KeyEqual, Allocator>::Node::Node(const std::size_t& dictionarySize)
{
	m_children.resize(dictionarySize);
	m_isTerminal = false;
}

template<class Key, class Hash, class SequenceContainer, class KeyEqual, class Allocator>
inline TrieSet<Key, Hash, SequenceContainer, KeyEqual, Allocator>::Node::~Node()
{
	m_children.clear();
	m_isTerminal = false;
}

template<class Key, class Hash, class SequenceContainer, class KeyEqual, class Allocator>
inline TrieSet<Key, Hash, SequenceContainer, KeyEqual, Allocator>::allocator_type TrieSet<Key, Hash, SequenceContainer, KeyEqual, Allocator>::GetAllocator() const noexcept
{
	return m_dictionary.get_allocator();
}

/*
* Trie
*/
template<class Key, class Hash, class SequenceContainer, class KeyEqual, class Allocator>
inline TrieSet<Key, Hash, SequenceContainer, KeyEqual, Allocator>::TrieSet()
{
	m_size = 0;
}

template<class Key, class Hash, class SequenceContainer, class KeyEqual, class Allocator>
inline TrieSet<Key, Hash, SequenceContainer, KeyEqual, Allocator>::TrieSet(const SequenceContainer& dictionary)
{
	for (std::size_t i = 0; i < dictionary.size(); ++i)
	{
		m_dictionary[dictionary[i]] = i;
	}

	m_dictionarySize = m_dictionary.size();

	m_size = 0;
	m_root = std::make_unique<Node>(m_dictionarySize);
}

template<class Key, class Hash, class SequenceContainer, class KeyEqual, class Allocator>
inline TrieSet<Key, Hash, SequenceContainer, KeyEqual, Allocator>::~TrieSet()
{
	Clear();
}

template<class Key, class Hash, class SequenceContainer, class KeyEqual, class Allocator>
inline TrieSet<Key, Hash, SequenceContainer, KeyEqual, Allocator>::TrieSet(const Key& startingKey, const std::size_t & dictionaryLength)
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

template<class Key, class Hash, class SequenceContainer, class KeyEqual, class Allocator>
inline void TrieSet<Key, Hash, SequenceContainer, KeyEqual, Allocator>::Insert(const SequenceContainer& sequence)
{
	Insert(m_root.get(), sequence);
}

template<class Key, class Hash, class SequenceContainer, class KeyEqual, class Allocator>
inline void TrieSet<Key, Hash, SequenceContainer, KeyEqual, Allocator>::Insert(const Key sequence[], const std::size_t& sequenceLength)
{
	Insert(m_root.get(), sequence, sequenceLength);
}

template<class Key, class Hash, class SequenceContainer, class KeyEqual, class Allocator>
inline void TrieSet<Key, Hash, SequenceContainer, KeyEqual, Allocator>::Insert(TrieSet<Key, Hash, SequenceContainer, KeyEqual, Allocator>::Node* node, const SequenceContainer & sequence)
{
	if (node == nullptr)return;

	for (const Key & k : sequence)
	{
		node = TryInsert(node, k);
	}

	if(node->m_isTerminal == false)m_size++;

	node->m_isTerminal = true;
}

template<class Key, class Hash, class SequenceContainer, class KeyEqual, class Allocator>
inline void TrieSet<Key, Hash, SequenceContainer, KeyEqual, Allocator>::Insert(Node* node, const Key sequence[], const std::size_t& sequenceLength)
{
	if (node == nullptr)return;

	for (std::size_t i = 0; i < sequenceLength; ++i)
	{
		node = TryInsert(node, sequence[i]);
	}

	if (node->m_isTerminal == false)m_size++;

	node->m_isTerminal = true;
}

template<class Key, class Hash, class SequenceContainer, class KeyEqual, class Allocator>
inline TrieSet<Key, Hash, SequenceContainer, KeyEqual, Allocator>::Node * TrieSet<Key, Hash, SequenceContainer, KeyEqual, Allocator>::TryInsert(Node * node, const Key & value)
{
	if (node == nullptr)return nullptr;

	auto it = m_dictionary.find(value);
	if (it == m_dictionary.end())
	{
		throw std::out_of_range("Object not in dictionary");
	}

	std::size_t positionInDictionarySequence = it->second;

	if (node->m_children[positionInDictionarySequence] == nullptr) {
		node->m_children[positionInDictionarySequence] = std::make_unique<Node>(m_dictionarySize);
	}

	return node->m_children[positionInDictionarySequence].get();
}

template<class Key, class Hash, class SequenceContainer, class KeyEqual, class Allocator>
inline bool TrieSet<Key, Hash, SequenceContainer, KeyEqual, Allocator>::Contains(const SequenceContainer& sequence) const
{
	return Contains(m_root.get(), sequence);
}

template<class Key, class Hash, class SequenceContainer, class KeyEqual, class Allocator>
inline bool TrieSet<Key, Hash, SequenceContainer, KeyEqual, Allocator>::Contains(const Key sequence[], const std::size_t& sequenceLength) const
{
	return Contains(m_root.get(), sequence, sequenceLength);
}

template<class Key, class Hash, class SequenceContainer, class KeyEqual, class Allocator>
inline bool TrieSet<Key, Hash, SequenceContainer, KeyEqual, Allocator>::Empty() const
{
	return GetSize() == 0;
}

template<class Key, class Hash, class SequenceContainer, class KeyEqual, class Allocator>
inline void TrieSet<Key, Hash, SequenceContainer, KeyEqual, Allocator>::Clear()
{
	m_root.reset();
	m_dictionary.clear();

	m_dictionarySize = 0;
	m_size = 0;
}

template<class Key, class Hash, class SequenceContainer, class KeyEqual, class Allocator>
inline std::size_t TrieSet<Key, Hash, SequenceContainer, KeyEqual, Allocator>::GetSize() const
{
	return m_size;
}

template<class Key, class Hash, class SequenceContainer, class KeyEqual, class Allocator>
inline bool TrieSet<Key, Hash, SequenceContainer, KeyEqual, Allocator>::Contains(Node* node, const  SequenceContainer& sequence) const
{
	if (node == nullptr)return false;
	auto it = m_dictionary.end();

	for (const Key& k : sequence)
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

template<class Key, class Hash, class SequenceContainer, class KeyEqual, class Allocator>
inline bool TrieSet<Key, Hash, SequenceContainer, KeyEqual, Allocator>::Contains(Node* node, const Key sequence[], const std::size_t& sequenceLength) const
{
	if (node == nullptr)return false;
	auto it = m_dictionary.end();

	for (std::size_t i = 0; i < sequenceLength; ++i)
	{
		it = m_dictionary.find(sequence[i]);

		if (it == m_dictionary.end())
		{
			return false;
		}

		if (node->m_children[it->second] == nullptr)return false;
		else node = node->m_children[it->second].get();
	}

	return true;
}
#endif
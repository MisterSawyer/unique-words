#ifndef  CONCURENT_SET_H
#define CONCURENT_SET_H

#include <unordered_map>
#include <unordered_set>
#include <mutex>

template<class T>
class ConcurentSet
{
public:
	ConcurentSet(std::size_t buckets);
	void Insert(const T& obj);
	std::size_t GetSize() const;
private:

	class Bucket
	{
	public:
		Bucket() = default;

		void Insert(const std::size_t& obj);
		void Insert(std::size_t&& obj);

		std::size_t GetSize() const;

	private:
		std::mutex m_mutex;
		std::unordered_set<std::size_t> m_hashes;
	};

	/*
	Returns a pair which first element is bucket hash and second is hash itself
	*/
	std::pair<std::size_t, std::size_t> CalculateHash(const T& obj) const;


	std::size_t m_buckets;
	std::unordered_map <std::size_t, Bucket> m_bucketTable;
};

template<class T>
inline ConcurentSet<T>::ConcurentSet(std::size_t buckets)
{
	if (buckets == 0)buckets = 1;

	m_buckets = buckets;
	for (std::size_t bucketID = 0; bucketID < m_buckets; ++bucketID)
	{
		m_bucketTable.try_emplace(bucketID);
	}
}

template<class T>
inline void ConcurentSet<T>::Insert(const T& obj)
{
	const auto hashPair = CalculateHash(obj);
	m_bucketTable[hashPair.first].Insert(hashPair.second);
}

template<class T>
inline std::size_t ConcurentSet<T>::GetSize() const
{
	std::size_t size = 0;
	for (const auto & bucket : m_bucketTable)
	{
		size += bucket.second.GetSize();
	}

	return size;
}

template<class T>
inline std::pair<std::size_t, std::size_t> ConcurentSet<T>::CalculateHash(const T & obj) const
{
	const std::size_t hash = std::hash<T>{}(obj);
	const std::size_t bucketHash = hash % m_buckets;

	return std::make_pair(std::move(bucketHash), std::move(hash));
}

template<class T>
inline void ConcurentSet<T>::Bucket::Insert(const std::size_t& obj)
{
	std::lock_guard lock(m_mutex);
	m_hashes.insert(obj);
}

template<class T>
inline void ConcurentSet<T>::Bucket::Insert(std::size_t&& obj)
{
	std::lock_guard lock(m_mutex);
	m_hashes.insert(std::move(obj));
}

template<class T>
inline std::size_t ConcurentSet<T>::Bucket::GetSize() const
{
	return m_hashes.size();
}
#endif // ! CONCURENT_SET_H
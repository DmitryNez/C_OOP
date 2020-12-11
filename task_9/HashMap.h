#pragma once
#include <memory>
#include <vector>
#include <algorithm>
#include <initializer_list>
#include <optional>
#include "bucket.h"

template<typename Key, typename Value, typename Hash = std::hash<Key>, typename TAllocator = std::allocator<bucket<Key,Value>>>
class HashMap
{
public:
	using node_type = typename std::pair<Key, Value>;

	class iterator {
	private:
		typename bucket<Key, Value>::bucket_iterator list_it;
		typename std::vector<bucket<Key, Value>, TAllocator>::iterator vector_it;
		typename std::vector<bucket<Key, Value>, TAllocator>::iterator end;
		size_t shift;
	public:
		iterator() {
			end = buckets.end();
			vector_it = std::find_if(++vector_it, end, [](bucket<Key, Value> it) {return !it.empty(); });
			list_it = vector_it->begin();
		}
		iterator(const typename bucket<Key, Value>::bucket_iterator& list_it,const typename std::vector<bucket<Key, Value>, TAllocator>::iterator& vector_it,
			const typename std::vector<bucket<Key, Value>, TAllocator>::iterator& vector_end) {
			this->list_it = list_it;
			this->vector_it = vector_it;
			this->end = vector_end;
		}

		iterator& operator++() {

			if (vector_it->size() <= 1 || list_it == --vector_it->end()) {
				auto tmp = vector_it;
				vector_it = std::find_if(++vector_it, end, [](bucket<Key, Value> it) {return !it.empty(); });
				if (vector_it == end) {
					vector_it = tmp;
					vector_it++;
				}
				list_it = vector_it->begin();
			} 
			else {
				list_it++;
			}
			return *this;
		}

		typename HashMap<Key, Value, Hash, TAllocator>::node_type& operator*() {
			return *list_it;
		}

		bool operator == (const iterator& other) {
			if (vector_it != other.vector_it) {
				return false;
			}
			else if (list_it != other.list_it) {
				return false;
			}
			return true;
		}

		bool operator != (const iterator& other) {
			if (vector_it != other.vector_it){
				return true;
			} 
			else if(list_it != other.list_it) {
				return true;
			}
			return false;
		}
	};

	class const_iterator {
	private:
		typename bucket<Key, Value>::const_bucket_iterator list_it;
		typename std::vector<bucket<Key, Value>, TAllocator>::const_iterator vector_it;
		typename std::vector<bucket<Key, Value>, TAllocator>::const_iterator end;
		size_t shift;
	public:
		const_iterator() {
			end = buckets.end();
			vector_it = std::find_if(++vector_it, end, [](bucket<Key, Value> it) {return !it.empty(); });
			list_it = vector_it->begin();
		}
		const_iterator(const typename bucket<Key, Value>::const_bucket_iterator& list_it, const typename std::vector<bucket<Key, Value>, TAllocator>::const_iterator& vector_it,
			const typename std::vector<bucket<Key, Value>, TAllocator>::const_iterator& vector_end) {
			this->list_it = list_it;
			this->vector_it = vector_it;
			this->end = vector_end;
		}
		const_iterator(const_iterator && other) {
			this->list_it = other.list_it;
			this->vector_it = other.vector_it;
			this->end = other.end;
		}

		const_iterator& operator++() {

			if (vector_it->size() <= 1 or list_it == --vector_it->end()) {
				auto tmp = vector_it;
				vector_it = std::find_if(++vector_it, end, [](bucket<Key, Value> it) {return !it.empty(); });
				if (vector_it == end) {
					vector_it = tmp;
					vector_it++;
				}
				list_it = vector_it->begin();
			}
			else {
				list_it++;
			}
			return *this;
		}

		const typename HashMap<Key, Value, Hash, TAllocator>::node_type& operator*() {
			return *list_it;
		}

		bool operator == (const_iterator& other) {
			if (vector_it != other.vector_it) {
				return false;
			}
			else if (list_it != other.list_it) {
				return false;
			}
			return true;
		}

		bool operator != (const_iterator& other) {
			if (vector_it != other.vector_it) {
				return true;
			}
			else if (list_it != other.list_it) {
				return true;
			}
			return false;
		}

		bool operator != (iterator& other) {
			if (vector_it != other.vector_it) {
				return true;
			}
			else if (list_it != other.list_it) {
				return true;
			}
			return false;
		}
	}; 

	explicit HashMap(const size_t& capacity = default_capacity);
	HashMap(std::initializer_list<node_type>);
	HashMap(const HashMap<Key, Value, Hash, TAllocator>&);
	HashMap(HashMap<Key, Value, Hash, TAllocator>&&) noexcept;

	size_t size() const noexcept;
	size_t GetCapacity() const noexcept;
	bool empty() const noexcept;

	HashMap& operator=(const HashMap<Key, Value, Hash, TAllocator>& right);
	HashMap& operator=(HashMap<Key, Value, Hash, TAllocator>&& right);

	const Value& operator[](const Key &) const;
	const Value& operator[](Key&& ) const;

	Value& operator[](Key && );
	Value& operator[](const Key&);

	template <typename K, typename V>
	iterator insert(K && k, V && value);
	iterator insert(node_type && node);
	iterator insert(const node_type& node);

	template <class... Args>
	std::pair<iterator, bool> emplace(Args&&... args);

	void clear();

	iterator find(const Key& key);
	const_iterator find(const Key& key) const;

	iterator begin();
	iterator end();
	const_iterator begin() const;
	const_iterator end() const;

	void swap(HashMap<Key, Value, Hash, TAllocator>&);

	std::optional<Value> get(const Key&);
	node_type extract(const_iterator&) const;
	node_type extract(const Key&);

private:
	std::vector<bucket<Key,Value>, TAllocator> buckets;
	std::shared_mutex mutex;
	Hash hasher;
	const static size_t default_capacity = 20;
	size_t capacity;
	size_t count;
	iterator insert_(Key, Value );
	void Rehash();
};


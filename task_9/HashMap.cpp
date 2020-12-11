#include "HashMap.h"
#include "bucket.cpp"
#include <iostream>

template <typename Key, typename Value, typename Hash, typename TAllocator>
HashMap<Key, Value, Hash, TAllocator>::HashMap(const size_t& capacity) {
	count = 0;
	this->capacity = capacity;
	buckets.resize(capacity, bucket<Key, Value>());
}

template <typename Key, typename Value, typename Hash, typename TAllocator>
HashMap<Key, Value, Hash, TAllocator>::HashMap(const HashMap<Key, Value,Hash,TAllocator>& other) {
	//std::shared_lock<std::shared_mutex> lock(other.mutex);
	this->capacity = other.capacity;
	this->count = other.count;
	buckets = other.buckets;
}

template <typename Key, typename Value, typename Hash, typename TAllocator>
HashMap<Key, Value, Hash, TAllocator>::HashMap(HashMap<Key, Value, Hash, TAllocator>&& other) noexcept {
	std::shared_lock<std::shared_mutex> lock(other.mutex);
	this->capacity = other.capacity;
	this->count = other.count;
	buckets = other.buckets;
}

template <typename Key, typename Value, typename Hash, typename TAllocator>
HashMap<Key, Value, Hash, TAllocator>::HashMap(std::initializer_list<node_type> list) {
	this->capacity = default_capacity;
	buckets.resize(capacity, bucket<Key, Value>());
	for (auto it : list) {
		this->insert(it.first, it.second);
	}
}

template <typename Key, typename Value, typename Hash, typename TAllocator>
typename HashMap<Key, Value, Hash, TAllocator>::iterator HashMap<Key, Value, Hash, TAllocator>::begin() {
	auto vec_it = std::find_if(buckets.begin(), buckets.end(), [](bucket<Key, Value> it) {return !it.empty(); });
	if (vec_it == buckets.end()) {
		iterator it(std::list<node_type>::iterator(), vec_it, buckets.end());
		return it;
	}
	return iterator(vec_it->begin(), vec_it, buckets.end());
}

template <typename Key, typename Value, typename Hash, typename TAllocator>
typename HashMap<Key, Value, Hash, TAllocator>::iterator HashMap<Key, Value, Hash, TAllocator>::end() {
	auto vec_it = std::find_if(buckets.rbegin(), buckets.rend(), [](bucket<Key, Value> &it) {return !it.empty(); });
	size_t dist = std::distance(vec_it,buckets.rend());
	if (dist == 0) {
		return iterator(std::list<node_type>::iterator(), buckets.end(), buckets.end());
	}
	return iterator((buckets.begin() + dist)->begin(), buckets.begin() + dist, buckets.end());
}

template <typename Key, typename Value, typename Hash, typename TAllocator>
typename HashMap<Key, Value, Hash, TAllocator>::const_iterator HashMap<Key, Value, Hash, TAllocator>::begin() const {
	auto vec_it = std::find_if(buckets.begin(), buckets.end(), [](bucket<Key, Value> it) {return !it.empty(); });
	if (vec_it == buckets.end()) {
		return const_iterator(std::list<node_type>::iterator(), vec_it, buckets.end());
	}
	return const_iterator(vec_it->begin(), vec_it, buckets.end());
}

template <typename Key, typename Value, typename Hash, typename TAllocator>
typename HashMap<Key, Value, Hash, TAllocator>::const_iterator HashMap<Key, Value, Hash, TAllocator>::end() const {
	auto vec_it = std::find_if(buckets.rbegin(), buckets.rend(), [](bucket<Key, Value>& it) {return !it.empty(); });
	size_t dist = std::distance(vec_it, buckets.rend());
	if (dist == 0) {
		return const_iterator(std::list<node_type>::iterator(), buckets.end(), buckets.end());
	}

	return const_iterator((buckets.begin() + dist)->begin(), buckets.begin() + dist, buckets.end());
}

template <typename Key, typename Value, typename Hash, typename TAllocator>
typename HashMap<Key, Value, Hash, TAllocator>::iterator HashMap<Key, Value, Hash, TAllocator>::find(const Key& key) {
	//std::shared_lock<std::shared_mutex> lock(mutex);
	size_t bucket_index = hasher(key) % (buckets.size() - 1);
	if (buckets[bucket_index].empty()) {
		return this->end();
	}
	iterator it(buckets[bucket_index].find(key), (buckets.begin() + bucket_index), buckets.end());
	return it;
}

template <typename Key, typename Value, typename Hash, typename TAllocator>
typename HashMap<Key, Value, Hash, TAllocator>::const_iterator HashMap<Key, Value, Hash, TAllocator>::find(const Key& key) const{

	//std::shared_lock<std::shared_mutex> lock(mutex);

	size_t bucket_index = hasher(key) % (buckets.size() - 1);

	if (buckets[bucket_index].empty()) {
		return this->end();
	}

	const_iterator it(buckets[bucket_index].find(key), (buckets.begin() + bucket_index), buckets.end());

	return it;
}

template <typename Key, typename Value, typename Hash, typename TAllocator>
void typename HashMap<Key, Value, Hash, TAllocator>::Rehash() {
	std::unique_lock<std::shared_mutex> lock(mutex);
	HashMap<Key,Value,Hash,TAllocator> tmp_map(*this);
	capacity = capacity * 2;
	buckets.clear();
	count = 0;
	buckets.resize(capacity, bucket<Key, Value>());
	for (auto& it : tmp_map) {
		this->insert(it.first, it.second);
	}
	lock.unlock();
}

template <typename Key, typename Value, typename Hash, typename TAllocator>
typename HashMap<Key, Value, Hash, TAllocator>::iterator HashMap<Key, Value, Hash, TAllocator>::insert_(Key key, Value value) {
	size_t bucket_index = hasher(key) % (buckets.size() - 1);
	bool flag = buckets[bucket_index].AddOrUpdate(key, value);
	iterator it(this->find(key));
	if (flag == true) {
		count++;
	}
	return it;
}

template <typename Key, typename Value, typename Hash, typename TAllocator>
template <typename K, typename V>
typename HashMap<Key, Value, Hash, TAllocator>::iterator HashMap<Key, Value, Hash, TAllocator>::insert(K&& key, V&& value) {
	if (count >= 0.8 * capacity) {
		this->Rehash();
	}
	return insert_(std::forward<Key>(key), std::forward<Value>(value));
}

template <typename Key, typename Value, typename Hash, typename TAllocator>
typename HashMap<Key, Value, Hash, TAllocator>::iterator HashMap<Key, Value, Hash, TAllocator>::insert(node_type && node) {
	if (count >= 0.8 * capacity) {
		this->Rehash();
	}
	return insert_(node.first, node.second);
}

template <typename Key, typename Value, typename Hash, typename TAllocator>
typename HashMap<Key, Value, Hash, TAllocator>::iterator HashMap<Key, Value, Hash, TAllocator>::insert(const node_type& node) {
	if (count >= 0.8 * capacity) {
		this->Rehash();
	}
	return insert_(node.first, node.second);
}

template <typename Key, typename Value, typename Hash, typename TAllocator>
template <class ...Args>
std::pair<typename HashMap<Key, Value, Hash, TAllocator>::iterator, bool> HashMap<Key, Value, Hash, TAllocator>::emplace(Args&&...args) {
	if (count >= 0.8 * capacity) {
		this->Rehash();
	}
	size_t tmp_counter = count;
	iterator it = this->insert(std::make_pair(args...));
	if (count == tmp_counter) {
		return std::make_pair(it,false);
	}
	return std::make_pair(it, true);
}

template <typename Key, typename Value, typename Hash, typename TAllocator>
size_t HashMap<Key, Value, Hash, TAllocator>::size() const noexcept{
	//std::shared_lock<std::shared_mutex> lock(mutex);
	return count;
}

template <typename Key, typename Value, typename Hash, typename TAllocator>
size_t HashMap<Key, Value, Hash, TAllocator>::GetCapacity() const noexcept {
	//std::shared_lock<std::shared_mutex> lock(mutex);
	return capacity;
}

template <typename Key, typename Value, typename Hash, typename TAllocator>
bool HashMap<Key, Value, Hash, TAllocator>::empty() const noexcept {
	//std::shared_lock<std::shared_mutex> lock(mutex);
	return count == 0 ? true : false;
}

template <typename Key, typename Value, typename Hash, typename TAllocator>
void HashMap<Key, Value, Hash, TAllocator>::swap(HashMap<Key, Value, Hash, TAllocator>& other) {
	std::unique_lock<std::shared_mutex> lock(mutex);
	this->buckets.swap(other.buckets);
	std::swap(this->count, other.count);
}

template <typename Key, typename Value, typename Hash, typename TAllocator>
void HashMap<Key, Value, Hash, TAllocator>::clear() {
	std::unique_lock<std::shared_mutex> lock(mutex);
	buckets.clear();
	count = 0;
}

template <typename Key, typename Value, typename Hash, typename TAllocator>
HashMap<Key, Value, Hash, TAllocator>& HashMap<Key, Value, Hash, TAllocator>::operator=(const HashMap<Key, Value, Hash, TAllocator>& right) {
	std::unique_lock<std::shared_mutex> lock(mutex);
	this->buckets = right.buckets;
	this->count = right.count;
	this->capacity = right.capacity;
	return *this;
}

template <typename Key, typename Value, typename Hash, typename TAllocator>
HashMap<Key, Value, Hash, TAllocator>& HashMap<Key, Value, Hash, TAllocator>::operator=(HashMap<Key, Value, Hash, TAllocator>&& right) {
	std::unique_lock<std::shared_mutex> lock(mutex);
	this->buckets = right.buckets;
	this->count = right.count;
	this->capacity = right.capacity;
	return *this;
}

template <typename Key, typename Value, typename Hash, typename TAllocator>
const Value& HashMap<Key, Value, Hash, TAllocator>::operator[](const Key& key) const {
	std::shared_lock<std::shared_mutex> lock(mutex);
	return (*(this->find(key))).second;
}

template <typename Key, typename Value, typename Hash, typename TAllocator>
const Value& HashMap<Key, Value, Hash, TAllocator>::operator[](Key&& key) const {
	std::shared_lock<std::shared_mutex> lock(mutex);
	return (*(this->find(key))).second;
}

template <typename Key, typename Value, typename Hash, typename TAllocator>
Value& HashMap<Key, Value, Hash, TAllocator>::operator[](const Key& key){
	std::shared_lock<std::shared_mutex> lock(mutex);
	return (*(this->find(key))).second;
}

template <typename Key, typename Value, typename Hash, typename TAllocator>
Value& HashMap<Key, Value, Hash, TAllocator>::operator[](Key&& key){
	std::shared_lock<std::shared_mutex> lock(mutex);
	return (*(this->find(key))).second;
}

template <typename Key, typename Value, typename Hash, typename TAllocator>
typename HashMap<Key, Value, Hash, TAllocator>::node_type HashMap<Key, Value, Hash, TAllocator>::extract(HashMap<Key, Value, Hash, TAllocator>::const_iterator& pos) const {
	std::shared_lock<std::shared_mutex> lock(mutex);
	if (this->end() != pos) {
		return *pos;
	}
}

template <typename Key, typename Value, typename Hash, typename TAllocator>
typename HashMap<Key, Value, Hash, TAllocator>::node_type HashMap<Key, Value, Hash, TAllocator>::extract(const Key& key) {
	std::shared_lock<std::shared_mutex> lock(mutex);
	iterator pos = this->find(key);
	if (pos != this->end()) {
		return *pos;
	}
}

template <typename Key, typename Value, typename Hash, typename TAllocator>
std::optional<Value> HashMap<Key, Value, Hash, TAllocator>::get(const Key& key) {
	std::shared_lock<std::shared_mutex> lock(mutex);
	iterator it = this->find(key);
	if (it != this->end()) {
		return (*it).second;
	}
	return {};
}
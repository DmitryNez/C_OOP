#include "bucket.h"

template<typename Key, typename Value>
typename bucket<Key, Value>::bucket_iterator bucket<Key, Value>::find(const Key& key) {
	std::shared_lock<std::shared_mutex> lock(mutex);
	return std::find_if(data.begin(), data.end(),
		[&](const data_type& current_pair) {
		return current_pair.first == key;
	});
}
template<typename Key, typename Value>
typename bucket<Key, Value>::const_bucket_iterator bucket<Key, Value>::find(const Key& key) const {
	std::shared_lock<std::shared_mutex> lock(mutex);
	return std::find_if(data.begin(), data.end(),
		[&](const data_type& current_pair) {
		return current_pair.first == key;
	});
}
template<typename Key, typename Value>
bucket<Key, Value>::bucket() {
	data.assign(0,data_type());
}

template<typename Key, typename Value>
bucket<Key, Value>::bucket(const bucket<Key,Value> & other) {
	data = other.data;
}

template<typename Key, typename Value>
bucket<Key, Value>::bucket(Key && key,Value && value) {
	data.push_back(std::make_pair<Key, Value>(std::move(key), std::move(value)));
}

template<typename Key, typename Value>
Value bucket<Key, Value>::GetValue(const Key& key){
	bucket_iterator iterator = find(key);

	std::shared_lock<std::shared_mutex> lock(mutex);
	if (iterator == data.end()) {
		throw std::logic_error("Pair does not exist!");
	}

	return iterator->second;
}
template<typename Key,typename Value>
bucket<Key, Value>& bucket<Key, Value>::operator=(const bucket<Key, Value>& other) {
	std::unique_lock<std::shared_mutex> lock(mutex);
	this->data = other.data;
	lock.unlock();
	return *this;
}
template<typename Key, typename Value>
bool bucket<Key, Value>::AddOrUpdate(const Key& key, const Value& value) {

	bucket_iterator iterator = find(key);

	std::unique_lock<std::shared_mutex> lock(mutex);
	bool flag = false;
	if (iterator == data.end()) {
		data.push_back(data_type(key, value));
		flag = true;
	}
	else {
		iterator->second = value;
	}
	lock.unlock();

	return flag;
}

template<typename Key, typename Value>
void bucket<Key, Value>::Remove(const Key& key) {

	bucket_iterator iterator = find(key);

	std::unique_lock<std::shared_mutex> lock(mutex);

	if (iterator != data.end()) {
		data.erase(iterator);
	}

	lock.unlock();
}

template<typename Key, typename Value>
bool typename bucket<Key, Value>::empty() const {
	std::shared_lock<std::shared_mutex> lock(mutex);
	return data.empty();
}

template<typename Key, typename Value>
size_t typename bucket<Key, Value>::size() const {
	std::shared_lock<std::shared_mutex> lock(mutex);
	return data.size();
}


template<typename Key, typename Value>
typename bucket<Key, Value>::bucket_iterator bucket<Key, Value>::begin() {
	return data.begin();
}

template<typename Key, typename Value>
typename bucket<Key, Value>::bucket_iterator bucket<Key, Value>::end() {
	return data.end();
}

template<typename Key, typename Value>
typename bucket<Key, Value>::const_bucket_iterator bucket<Key, Value>::begin() const {
	return data.begin();
}

template<typename Key, typename Value>
typename bucket<Key, Value>::const_bucket_iterator bucket<Key, Value>::end() const {
	return data.end();
}
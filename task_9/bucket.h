#pragma once
#include <list>
#include <shared_mutex>

template<typename Key,typename Value>
class bucket
{
public:
	using data_type = typename std::pair<Key, Value>;

	using bucket_iterator = typename std::list<data_type>::iterator;

	using const_bucket_iterator = typename std::list<data_type>::const_iterator;

	bucket();

	bucket(Key &&, Value &&);

	bucket(const bucket &);

	bucket_iterator find(const Key&);

	const_bucket_iterator find(const Key&) const;

	bucket& operator=(const bucket&);

	Value GetValue(const Key&);

	bool AddOrUpdate(const Key&, const Value&);

	void Remove(const Key &);

	size_t size() const;

	bool empty() const;

	bucket_iterator begin();

	bucket_iterator end();

	const_bucket_iterator begin() const;

	const_bucket_iterator end() const;

private:
	std::list<data_type> data;
	mutable std::shared_mutex mutex;
};


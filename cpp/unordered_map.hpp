#pragma once

#include <functional>
#include <vector>

template <typename Key, typename T, typename Hash = std::hash<Key>>
class UnorderedMap {
  public:
    UnorderedMap(size_t count = DEFAULT_BUCKET_COUNT, double loadFactor = DEFAULT_LOAD_FACTOR)
        : size_(0), maxLoadFactor_(loadFactor), buckets_(count) {}

    ~UnorderedMap() { clear(); }

    UnorderedMap(const UnorderedMap& other) : size_(0), maxLoadFactor_(other.maxLoadFactor_) {
        buckets_.resize(other.buckets_.size(), nullptr);
        for (size_t i = 0; i < other.buckets_.size(); ++i) {
            Node* curr = other.buckets_[i];
            while (curr) {
                insert(curr->key, curr->value);
                curr = curr->next;
            }
        }
    }

    UnorderedMap(UnorderedMap&& other) noexcept
        : size_(other.size_), maxLoadFactor_(other.maxLoadFactor_), buckets_(std::move(other.buckets_)) {
        other.size_ = 0;
    }

    UnorderedMap& operator=(const UnorderedMap& other) {
        if (this != &other) {
            clear();
            buckets_.resize(other.buckets_.size(), nullptr);
            maxLoadFactor_ = other.maxLoadFactor_;
            for (size_t i = 0; i < other.buckets_.size(); ++i) {
                Node* curr = other.buckets_[i];
                while (curr) {
                    insert(curr->key, curr->value);
                    curr = curr->next;
                }
            }
        }
        return *this;
    }

    UnorderedMap& operator=(UnorderedMap&& other) noexcept {
        if (this != &other) {
            clear();
            size_ = other.size_;
            maxLoadFactor_ = other.maxLoadFactor_;
            buckets_ = std::move(other.buckets_);
            other.size_ = 0;
        }
        return *this;
    }

    T& operator[](const Key& key) {
        if (static_cast<double>(size_ + 1) / buckets_.size() > maxLoadFactor_) { rehash(buckets_.size() * 2); }
        size_t idx = getBucketIndex(key);
        Node* curr = buckets_[idx];
        while (curr) {
            if (curr->key == key) { return curr->value; }
            curr = curr->next;
        }
        Node* node = new Node(key, T());
        node->next = buckets_[idx];
        buckets_[idx] = node;
        ++size_;
        return node->value;
    }

    bool insert(const Key& key, const T& value) {
        if (static_cast<double>(size_ + 1) / buckets_.size() > maxLoadFactor_) { rehash(buckets_.size() * 2); }
        size_t idx = getBucketIndex(key);
        Node* curr = buckets_[idx];
        while (curr) {
            if (curr->key == key) {
                curr->value = value;
                return false;
            }
            curr = curr->next;
        }
        Node* node = new Node(key, value);
        node->next = buckets_[idx];
        buckets_[idx] = node;
        ++size_;
        return true;
    }

    template <typename... Args>
    void emplace(Args&&... args) {}

    T* find(const Key& key) {
        size_t idx = getBucketIndex(key);
        Node* curr = buckets_[idx];
        while (curr) {
            if (curr->key == key) { return &(curr->value); }
            curr = curr->next;
        }
        return nullptr;
    }

    bool erase(const Key& key) {
        size_t idx = getBucketIndex(key);
        Node *curr = buckets_[idx], *prev = nullptr;
        while (curr) {
            if (curr->key == key) {
                if (!prev) {
                    buckets_[idx] = curr->next;
                } else {
                    prev->next = curr->next;
                }
                delete curr;
                --size_;
                return true;
            }
            prev = curr;
            curr = curr->next;
        }
        return false;
    }

    void rehash(size_t bucketCount) {
        std::vector<Node*> newBuckets;
        newBuckets.resize(bucketCount, nullptr);
        size_t newSize = 0;
        for (size_t i = 0; i < buckets_.size(); ++i) {
            Node* curr = buckets_[i];
            while (curr) {
                Node* nxt = curr->next;
                size_t newIdx = hashFunc_(curr->key) % newBuckets.size();
                curr->next = newBuckets[newIdx];
                newBuckets[newIdx] = curr;
                ++newSize;
                curr = nxt;
            }
        }
        buckets_ = std::move(newBuckets);
        size_ = newSize;
    }

    void clear() {
        for (size_t i = 0; i < buckets_.size(); ++i) {
            Node* curr = buckets_[i];
            while (curr) {
                Node* tmp = curr;
                curr = curr->next;
                delete tmp;
            }
            buckets_[i] = nullptr;
        }
        size_ = 0;
    }

    bool empty() const { return size_ == 0; }

    size_t size() const { return size_; }

  private:
    size_t getBucketIndex(const Key& key) const { return hashFunc_(key) % buckets_.size(); }

    struct Node {
        Key key;
        T value;
        Node* next;

        Node(const Key& k, const T& v) : key(k), value(v), next(nullptr) {}
        Node(Key&& k, T&& v) : key(std::move(k)), value(std::move(v)), next(nullptr) {}
    };

    static constexpr size_t DEFAULT_BUCKET_COUNT = 16;
    static constexpr double DEFAULT_LOAD_FACTOR = 0.75;

    Hash hashFunc_;
    size_t size_;
    double maxLoadFactor_;
    std::vector<Node*> buckets_;
};

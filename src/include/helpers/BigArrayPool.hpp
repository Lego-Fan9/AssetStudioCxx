#ifndef BIG_ARRAY_POOL_HPP
#define BIG_ARRAY_POOL_HPP

#include <vector>
#include <deque>
#include <mutex>
#include <algorithm>

template<typename T>
class BigArrayPool {
public:
    static std::vector<T> Rent(size_t size) {
        std::lock_guard<std::mutex> lock(mutex_);

        auto it = std::find_if(pool_.begin(), pool_.end(),
                               [size](const std::vector<T>& v){ return v.size() >= size; });

        if (it != pool_.end()) {
            std::vector<T> result = std::move(*it);
            pool_.erase(it);
            return result;
        }

        return std::vector<T>(size);
    }

    static void Return(std::vector<T>& arr) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (pool_.size() < maxPoolSize_) {
            pool_.emplace_back(std::move(arr));
        }
    }

    static void SetMaxPoolSize(size_t maxPoolSize) {
        maxPoolSize_ = maxPoolSize;
    }

private:
    static std::deque<std::vector<T>> pool_;
    static std::mutex mutex_;
    static size_t maxPoolSize_;
};

template<typename T> std::deque<std::vector<T>> BigArrayPool<T>::pool_;
template<typename T> std::mutex BigArrayPool<T>::mutex_;
template<typename T> size_t BigArrayPool<T>::maxPoolSize_ = 5;

#endif

#include <mutex>
#include <queue>

namespace TSEngine {
    template<typename T>
    class SwapQueue {
    public:

        // 禁用拷贝构造和赋值
        SwapQueue(const SwapQueue&) = delete;
        SwapQueue& operator=(const SwapQueue&) = delete;

        // 推送数据到队列
       void push(T& value) {
        std::lock_guard<std::mutex> lock(mutex_);
        left_.push(value);
        }

    void push(T&& value) {
        std::lock_guard<std::mutex> lock(mutex_);
        left_.push(std::move(value));
    }

    bool pop(T& value) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (left_.empty()) {
            return false;
        }
        value = std::move(left_.front());
        left_.pop();
        return true;
    }

    bool try_pop(T& value) {
        std::unique_lock<std::mutex> lock(mutex_, std::try_to_lock);
        if (!lock.owns_lock() || left_.empty()) {
            return false;
        }
        value = std::move(left_.front());
        left_.pop();
        return true;
    }

    std::queue<T> swap() {
        std::lock_guard<std::mutex> lock(mutex_, std::adopt_lock);
        right_ = std::queue<T>();
        std::swap(left_, right_);
        return right_;
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return left_.empty();
    }

    size_t size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return left_.size();
    }

    void clear() {
        std::lock_guard<std::mutex> lock(mutex_);
        while (!left_.empty()) {
            left_.pop();
        }
    }

    private:
        std::mutex mutex_;
        std::queue<T> left_;
        std::queue<T> right_;
    };
}

#pragma once

#include <vector>
#include <thread>
#include <mutex>
#include <queue>
#include <utility>
#include <functional>
#include <tuple>
#include <condition_variable>

#include "Table.hpp"

template<typename Func, typename... Args>
class ThreadPool {
public:
    ThreadPool(size_t  thread_num, Func&& aF) : f(std::forward<Func>(aF)) {
        for (size_t  i = 0; i < thread_num; ++i) {
            workers.emplace_back(&ThreadPool::worker, this);
        }
    }


    ~ThreadPool() {
        cond.notify_all();
        for (auto& worker : workers) {
            worker.join();
        }
    }

    void enqueue(Args&&... args) {
        {
            std::unique_lock<std::mutex> x(queue_mtx);
            filenames_q.emplace(std::forward<Args>(args)...);
        }
        cond.notify_one();
    }

private:
    std::vector<std::thread> workers;
    std::mutex queue_mtx;
    std::queue<std::tuple<Args...>> filenames_q;
    std::condition_variable cond;
    Func f;

    void worker() {
        while (true) {
            std::unique_lock<std::mutex> x(queue_mtx);
            cond.wait(x, [this](){ return !this->filenames_q.empty(); });
            if (filenames_q.empty()) {
                continue;
            }
            std::tuple<Args...> curr_task = filenames_q.front();
            filenames_q.pop();
            x.unlock();
            if (std::get<0>(curr_task) == nullptr) {
                return;
            } else {
                std::apply(f, curr_task);
            }
        }
    }
};
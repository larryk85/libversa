#pragma once

#include <cstdint>
#include <functional>
#include <future>
#include <mutex>
#include <thread>

#include "../frozen/traits.hpp"
#include "../utils/misc.hpp"

namespace versa::async {
   template <typename F>
   class executor {
      public:
         using function_type = decltype(std::function(std::declval<F>()));

         inline executor(F&& func)
            : _func(std::forward<F>(func)),
              _mutex() {
         }

         inline executor(const executor&) = delete;
         inline executor(executor&&) = delete;
         inline executor& operator=(const executor&) = delete;
         inline executor& operator=(executor&&) = delete;
         inline ~executor() = default;

         template <typename... Args>
         inline auto exec(Args&&... args) {
            std::lock_guard<std::mutex> lock(_mutex);
            return std::async(std::launch::async, _func, std::forward<Args>(args)...);
         }

         template <typename... Args>
         inline auto operator()(Args&&... args) { return exec(std::forward<Args>(args)...); }

         template <typename... Args>
         inline auto exec_sync(Args&&... args) {
            std::lock_guard<std::mutex> lock(_mutex);
            return _func(std::forward<Args>(args)...);
         }



      private:
         function_type _func;
         std::mutex    _mutex;
   };
};
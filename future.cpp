#include "future.hpp"

template <>
std::unique_ptr<std::optional<std::any>> Future<void>::Get() {
    return std::make_unique<std::optional<std::any>>();
}
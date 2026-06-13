#pragma once

#include <stddef.h>

struct TypeErasedCallable {
  using Callback = void (*)(void *);

  void *context = nullptr;
  Callback callback = nullptr;

  void operator()() const {
    if (callback != nullptr) {
      callback(context);
    }
  }

  explicit operator bool() const { return callback != nullptr; }

  bool operator==(const TypeErasedCallable &other) const {
    return context == other.context && callback == other.callback;
  }

  template <typename Callable>
    requires requires(Callable c) { c(); }
  static auto Bind(Callable &callable) -> TypeErasedCallable {
    return TypeErasedCallable{
        .context = &callable,
        .callback = [](void *ctx) { (*static_cast<Callable *>(ctx))(); },
    };
  }
};
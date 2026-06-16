#pragma once

template <typename stateEnum> class IState {
public:
#ifndef __AVR__
  virtual ~IState() = default;
#endif
  virtual auto GetState() -> stateEnum = 0;

  virtual void Enter() = 0;
  virtual auto Do() -> stateEnum = 0;
  virtual void Exit() = 0;
};
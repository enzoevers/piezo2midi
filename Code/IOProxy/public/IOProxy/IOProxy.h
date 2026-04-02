#pragma once

template <typename RegisterEnum, typename RegisterType, typename DefineEnum,
          typename ValueType>
class IOProxy {
public:
  virtual void WriteRegister(RegisterEnum registerName,
                             RegisterType newValue) = 0;
  virtual auto ReadRegister(RegisterEnum registerName) -> RegisterType = 0;
  virtual auto ReadDefine(DefineEnum defineName) -> ValueType = 0;
};
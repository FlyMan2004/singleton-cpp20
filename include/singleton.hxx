#pragma once
#include "utils.hxx"

// See `example/example.hxx` for usage

// The `export_symbol$` on the type is a must to ensure that
// exactly one instance is created across binary boundaries,
// because types in C++ have linkage as well.
template <typename DataType>
struct export_symbol$ ISingleton
{
  export_symbol$ constexpr static auto
  instance() noexcept -> DataType &
  {
    // The instance itself must be exported to ensure it is unique
    export_symbol$ static DataType s_instance;
    return s_instance;
  }

  // Must export the dtor to ensure that it is always identical across
  // binary boundaries. Otherwise, each binary may generate their own version.
  export_symbol$ constexpr
  ~ISingleton() = default;

protected:
  // Ctor should be protected to avoid direct instantiation and exported
  export_symbol$ constexpr
  ISingleton() noexcept = default;
};

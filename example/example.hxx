#pragma once
#include "utils.hxx"
#include "singleton.hxx"

#include <string_view>
#include <print>

#include <dlfcn.h>
#include <elf.h>

namespace example
{
  struct export_symbol$ Good
  {
    std::string_view str{ "Hello, Singleton!" };
    int value{ 42 };
    float decimal{ 3.14f };

    export_symbol$ constexpr
    ~Good() = default;
  };

  // Use inheritance to explicitly declare the singleton
  struct export_symbol$ GoodSingleton
    : public Good
    , public ISingleton<GoodSingleton>
  {
    export_symbol$ constexpr
    ~GoodSingleton() = default;
  };

  struct export_symbol$ Correct
  {
    std::string_view str{ "Correct Singleton!" };
    int value{ 1 };
    float decimal{ 2.71f };

    export_symbol$ constexpr
    ~Correct() = default;
  };

  // Type may not be of default visibility, so we may see multiple instances
  struct Wrong
  {
    std::string_view str{ "Wrong Singleton!" };
    int value{ -1 };
    float decimal{ -3.14f };

    export_symbol$ constexpr
    ~Wrong() = default;
  };
} // namespace example

namespace static_lib
{
  extern void foo() noexcept;
  extern void bar() noexcept;
  extern void baz() noexcept;
} // namespace static_lib

namespace dylib
{
  namespace a
  {
    extern void foo() noexcept;
    extern void bar() noexcept;
    extern void baz() noexcept;
  } // namespace a

  namespace b
  {
    extern void foo() noexcept;
    extern void bar() noexcept;
    extern void baz() noexcept;
  } // namespace b

  namespace dlopen
  {
    export_symbol$ constexpr std::string_view so_name{ "libfrom_dlopen.so" };
  } // namespace dlopen
} // namespace dylib

namespace utils
{
  template <std::string_view const &sym_name>
  struct [[clang::trivial_abi]] export_symbol$ dlsym_wrapper_t
  {
    using function_type = void () noexcept;
    void * _Nullable so_handle = nullptr;
    function_type * _Nullable symbol = nullptr;

    export_symbol$ static function_type * _Nullable
    check_symbol(void * _Nonnull symbol_addr) noexcept
    {
      Elf64_Sym const *sym;
      Dl_info info;
      int const errc = ::dladdr1(symbol_addr, &info, (void **)&sym, RTLD_DL_SYMENT);
      // `symbol_addr` is not located in any loaded shared object
      if (errc == 0) return nullptr;
      // `symbol_addr` does not point to the start of a symbol
      if (info.dli_saddr == nullptr || info.dli_sname == nullptr) return nullptr;
      // `symbol_addr` points to a symbol that is not a function
      if (ELF64_ST_TYPE(sym->st_info) != STT_FUNC) return nullptr;
      return reinterpret_cast<function_type *>(info.dli_saddr);
    }

    export_symbol$ function_type * _Nullable
    resolve() noexcept
    {
      if (this->so_handle != nullptr) return this->symbol;
      this->so_handle = ::dlopen(dylib::dlopen::so_name.data(), RTLD_LAZY | RTLD_GLOBAL);
      if (this->so_handle == nullptr) return nullptr;
      return check_symbol(::dlsym(this->so_handle, sym_name.data()));
    }

    export_symbol$ void
    operator()() noexcept
    {
      this->symbol = this->symbol ?: this->resolve();
      if (this->symbol != nullptr) {
        (*this->symbol)();
      } else {
        std::println("Failed to resolve symbol '{:s}' in {:s}", sym_name, dylib::dlopen::so_name);
        std::terminate();
      }
    }

    export_symbol$
    ~dlsym_wrapper_t()
    {
      if (this->so_handle != nullptr)
        (void)::dlclose(this->so_handle);
    }
  };
} // namespace utils

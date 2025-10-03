#include "example.hxx"
#include <print>

namespace dylib::dlopen
{
  constexpr static std::string_view module_name{ "dylib::dlopen" };

  extern "C" export_symbol$ void
  foo() noexcept
  {
    auto const * const p_obj = std::addressof(example::GoodSingleton::instance());
    std::println("In module \"{:s}\"", module_name);
    std::println("  {:>8s}: {:p}", "address", (void const *)p_obj);
    std::println("  {:>8s}: {:s}", "str", p_obj->str);
    std::println("  {:>8s}: {:d}", "value", p_obj->value);
    std::println("  {:>8s}: {:g}", "decimal", p_obj->decimal);
  }

  // Correct:
  extern "C" export_symbol$ void
  bar() noexcept
  {
    auto const * const p_obj = std::addressof(ISingleton<example::Correct>::instance());
    std::println("In module \"{:s}\"", module_name);
    std::println("  {:>8s}: {:p}", "address", (void const *)p_obj);
    std::println("  {:>8s}: {:s}", "str", p_obj->str);
    std::println("  {:>8s}: {:d}", "value", p_obj->value);
    std::println("  {:>8s}: {:g}", "decimal", p_obj->decimal);
  }

  // Wrong:
  extern "C" export_symbol$ void
  baz() noexcept
  {
    auto const * const p_obj = std::addressof(ISingleton<example::Wrong>::instance());
    std::println("In module \"{:s}\"", module_name);
    std::println("  {:>8s}: {:p}", "address", (void const *)p_obj);
    std::println("  {:>8s}: {:s}", "str", p_obj->str);
    std::println("  {:>8s}: {:d}", "value", p_obj->value);
    std::println("  {:>8s}: {:g}", "decimal", p_obj->decimal);
  }

} // namespace dylib::dlopen

#include "example.hxx"
#include <print>

namespace static_lib
{
  constexpr static std::string_view module_name{ "static_lib" };

  // Good:
  export_symbol$ void
  foo() noexcept
  {
    std::println("In module \"{:s}\"", module_name);
    auto const * const p_obj = std::addressof(example::GoodSingleton::instance());
    std::println("  {:>8s}: {:p}", "address", (void const *)p_obj);
    std::println("  {:>8s}: {:s}", "str", p_obj->str);
    std::println("  {:>8s}: {:d}", "value", p_obj->value);
    std::println("  {:>8s}: {:g}", "decimal", p_obj->decimal);
  }

  // Correct:
  export_symbol$ void
  bar() noexcept
  {
    std::println("In module \"{:s}\"", module_name);
    auto const * const p_obj = std::addressof(ISingleton<example::Correct>::instance());
    std::println("  {:>8s}: {:p}", "address", (void const *)p_obj);
    std::println("  {:>8s}: {:s}", "str", p_obj->str);
    std::println("  {:>8s}: {:d}", "value", p_obj->value);
    std::println("  {:>8s}: {:g}", "decimal", p_obj->decimal);
  }

  // Wrong:
  export_symbol$ void
  baz() noexcept
  {
    std::println("In module \"{:s}\"", module_name);
    auto const * const p_obj = std::addressof(ISingleton<example::Wrong>::instance());
    std::println("  {:>8s}: {:p}", "address", (void const *)p_obj);
    std::println("  {:>8s}: {:s}", "str", p_obj->str);
    std::println("  {:>8s}: {:d}", "value", p_obj->value);
    std::println("  {:>8s}: {:g}", "decimal", p_obj->decimal);
  }
} // namespace stlib::c

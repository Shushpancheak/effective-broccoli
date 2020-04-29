#include "error.hpp"

namespace { // anonymous namespace

  struct ErrorCategory : std::error_category {
    [[nodiscard]] const char* name() const noexcept override;
    [[nodiscard]] std::string message(int ev) const override;
  };

  const char* ErrorCategory::name() const noexcept {
    return "effective-broccoli";
  }

  std::string ErrorCategory::message(int ev) const {
    return GetErrorMsg(ev);
  }

  const ErrorCategory effectiveBroccoliErrorCategory {}; // using const to avoid static initialization order fiasco.

}

std::error_code make_error_code(ErrorCode e)
{
  return {static_cast<int>(e), effectiveBroccoliErrorCategory};
}
#include "error.hpp"

namespace { // anonymous namespace

    struct ErrorCategory : std::error_category {
      [[nodiscard]] const char* name() const noexcept override;
      [[nodiscard]] std::string message(int ev) const override;
    };

    const char* ErrorCategory::name() const noexcept {
      return "effective-broccoli";
    }

    std::string ErrorCategory::message(int ev) const
    {
      switch (static_cast<ErrorCode>(ev))
      {
        case ErrorCode::FALSE_TYPE:
          return "Types don't match.";

        case ErrorCode::ALLOC_FAILED:
          return "Allocation fail.";

        case ErrorCode::ALREADY_DELETED:
          return "Chunk has already been deleted.";

        case ErrorCode::CTOR_FAILED:
          return "Unable to construct chunk.";

        case ErrorCode::OUT_OF_BOUNDS:
          return "Buffer out of bounds.";

        case ErrorCode::OBJECT_NOT_PRESENT:
          return "Object is not present.";

        case ErrorCode::NOT_FOUND:
          return "Object was not found.";

        case ErrorCode::BAD_PTR:
          return "Invalid pointer.";

        default:
          return "Unknown error.";
      }
    }

    const ErrorCategory effectiveBroccoliErrorCategory {}; // using const to avoid static initialization order fiasco.

}

std::error_code make_error_code(ErrorCode e)
{
  return {static_cast<int>(e), effectiveBroccoliErrorCategory};
}
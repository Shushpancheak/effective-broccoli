#ifndef EFFECTIVE_BROCOLLI_ERROR_CODES
#define EFFECTIVE_BROCOLLI_ERROR_CODES

#include <system_error>
#include <iostream>

#define CHECK_ERROR(res)                                \
  if ((res).HasError()) {                               \
    return make_result::PropagateError(res);            \
  }

#define REPORT_IF_ERROR(res)                       \
  if ((res).HasError()) {                          \
    std::cout << "error: line: " << __LINE__ << ", file: " << __FILE__ << ", "  << GetErrorMsg((res).Error().value());             \
    return;                                        \
  }

enum ErrorCode : int {
  FALSE_TYPE = 1,
  CTOR_FAILED,
  BAD_PTR,
  ALREADY_DELETED,
  OUT_OF_BOUNDS,
  OBJECT_NOT_PRESENT,
  NOT_FOUND,
  ALLOC_FAILED,
  LOAD_ERROR,
  IS_FULL,
  SUBSCRIPTION_NOT_FOUND
};

inline std::string GetErrorMsg(const int errc) {
  switch (errc) {
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

  case ErrorCode::LOAD_ERROR:
    return "Unable to load from given file.";

  case ErrorCode::IS_FULL:
    return "Container overflow.";

  case ErrorCode::SUBSCRIPTION_NOT_FOUND:
    return "Subscription not found.";

  default:
    return "Unknown error.";
  }
}

template <>
struct std::is_error_code_enum<ErrorCode> : true_type {};

std::error_code make_error_code(ErrorCode);

#endif
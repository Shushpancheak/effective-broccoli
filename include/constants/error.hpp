#ifndef EFFECTIVE_BROCOLLI_ERROR_CODES
#define EFFECTIVE_BROCOLLI_ERROR_CODES
#include <system_error>

#define CHECK_ERROR(res) \
if (res.HasError()) {\
  return res; \
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
  IS_FULL
};
template <>
struct std::is_error_code_enum<ErrorCode > : true_type {};

std::error_code make_error_code(ErrorCode);

#endif
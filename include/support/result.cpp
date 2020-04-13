#include "result.hpp"

namespace make_result {

    Status Ok() {
      return Status::Ok();
    }

    detail::Failure Fail(std::error_code error) {
      return detail::Failure{error};
    }

    Status ToStatus(std::error_code error) {
      if (error) {
        return Fail(error);
      } else {
        return Ok();
      }
    }

}  // namespace make_result


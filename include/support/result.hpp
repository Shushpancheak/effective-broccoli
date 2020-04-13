// Source : https://gitlab.com/Lipovsky/tiny-support/-/tree/master

#ifndef EFFECTIVE_BROCOLLI_RESULT_HPP
#define EFFECTIVE_BROCOLLI_RESULT_HPP
#pragma once

#include <system_error>

#include <type_traits>
#include <utility>

namespace detail {
    template <typename T>
    class ValueStorage {
      using Storage = typename std::aligned_storage<sizeof(T), alignof(T)>::type;

    public:
      template <typename... Arguments>
      void Construct(Arguments&&... arguments) {
        new (&storage_) T(std::forward<Arguments>(arguments)...);
      }

      void MoveConstruct(T&& that) {
        new (&storage_) T(std::move(that));
      }

      void CopyConstruct(const T& that) {
        new (&storage_) T(that);
      }

      T* PtrUnsafe() {
        return reinterpret_cast<T*>(&storage_);
      }

      const T* PtrUnsafe() const {
        return reinterpret_cast<const T*>(&storage_);
      }

      void Destroy() {
        PtrUnsafe()->~T();
      }

    private:
      Storage storage_;
    };

}


template <typename T>
class [[nodiscard]] Result {
public:
  static_assert(!std::is_reference<T>::value,
                "Reference types are not supported");

  // Static constructors

  template <typename... Arguments>
  static Result Ok(Arguments&&... arguments) {
    Result result;
    result.value_.Construct(std::forward<Arguments>(arguments)...);
    return result;
  }

  static Result Ok(T&& value) {
    return Result(std::move(value));
  }

  static Result Fail(std::error_code error) {
    return Result(error);
  }

  // Moving

  Result(Result&& that) {
    MoveImpl(std::move(that));
  }

  Result& operator=(Result&& that) {
    DestroyValueIfExist();
    MoveImpl(std::move(that));
    return *this;
  }

  // Copying

  Result(const Result& that) {
    CopyImpl(that);
  }

  Result& operator=(const Result& that) {
    DestroyValueIfExist();
    CopyImpl(that);
    return *this;
  }

  // Dtor

  ~Result() {
    DestroyValueIfExist();
  }

  // Testing

  bool HasError() const {
    return (bool)error_;
  }

  bool IsOk() const {
    return !HasError();
  }

  bool HasValue() const {
    return !HasError();
  }

  // Remove?
  explicit operator bool() const {
    return IsOk();
  }

  void ThrowIfError() const {
    if (HasError()) {
      throw std::system_error(error_);
    }
  }

  // Ignore result, just check for error
  void ExpectOk() {
    ThrowIfError();
  }

  // ErrorCode accessors

  bool MatchErrorCode(int expected) const {
    return error_.value() == expected;
  }

  std::error_code Error() const {
    return error_;
  }

  // Value accessors

  // Unsafe value getters, use only after IsOk
  // Behavior is undefined if Result does not contain a value

  T& ValueUnsafe() {
    return *value_.PtrUnsafe();
  }

  const T& ValueUnsafe() const {
    return *value_.PtrUnsafe();
  }

  // Safe value getters
  // Throws if Result does not contain a value

  T& Value() {
    ThrowIfError();
    return *value_.PtrUnsafe();
  }

  const T& Value() const {
    ThrowIfError();
    return *value_.PtrUnsafe();
  }

  // operator * overloads
  // Unsafe: behavior is undefined if Result does not contain a value

  T& operator*() {
    return ValueUnsafe();
  }

  const T& operator*() const {
    return ValueUnsafe();
  }

  // operator -> overloads
  // Unsafe: behavior is undefined if Result does not contain a value

  T* operator->() {
    return value_.PtrUnsafe();
  }

  const T* operator->() const {
    return value_.PtrUnsafe();
  }

  // Unwrap rvalue Result automatically
  // Do we need this?

  operator T&&() && {
    return std::move(Value());
  }

private:
  Result() {
  }

  Result(T&& value) {
    value_.MoveConstruct(std::move(value));
  }

  Result(const T& value) {
    value_.CopyConstruct(value);
  }

  Result(std::error_code error) : error_(error) {
  }

  void MoveImpl(Result&& that) {
    error_ = std::move(that.error_);
    if (that.HasValue()) {
      value_.MoveConstruct(std::move(that.ValueUnsafe()));
    }
  }

  void CopyImpl(const Result& that) {
    error_ = std::move(that.error_);
    if (that.HasValue()) {
      value_.CopyConstruct(that.ValueUnsafe());
    }
  }

  void DestroyValueIfExist() {
    if (IsOk()) {
      value_.Destroy();
    }
  }

private:
  detail::ValueStorage<T> value_;
  std::error_code error_;
};

////////////////////////////////////////////////////////////

template <>
class [[nodiscard]] Result<void> {
public:
  static Result Ok() {
    return Result{};
  }

  static Result Fail(std::error_code error) {
    return Result(error);
  }

  Result(std::error_code error): error_(error) {
  }

  // Testing

  bool HasError() const {
    return (bool)error_;
  }

  bool IsOk() const {
    return !HasError();
  }

  explicit operator bool() const {
    return IsOk();
  }

  void ThrowIfError() {
    if (HasError()) {
      throw std::system_error(error_);
    }
  }

  void ExpectOk() {
    ThrowIfError();
  }

  std::error_code Error() const {
    return error_;
  }

  bool MatchErrorCode(int expected) const {
    return error_.value() == expected;
  }

private:
  Result() = default;

private:
  std::error_code error_;
};

using Status = Result<void>;

////////////////////////////////////////////////////////////

namespace detail {

    class Failure {
    public:
      explicit Failure(std::error_code& error)
          : error_(error) {
      }

      Failure(const Failure&) = delete;
      Failure& operator =(const Failure&) = delete;

      Failure(Failure&&) = delete;
      Failure& operator =(Failure&&) = delete;

      template <typename T>
      operator Result<T>() {
        return Result<T>::Fail(error_);
      }

    private:
      std::error_code error_;
    };

}  // namespace detail

////////////////////////////////////////////////////////////

namespace make_result {

    template <typename T>
    Result<T> Ok(T&& value) {
      return Result<T>::Ok(std::move(value));
    }

    template <typename T>
    Result<T> Ok(T& value) {
      return Result<T>::Ok(value);
    }

    template <typename T>
    Result<T> Ok(const T& value) {
      return Result<T>::Ok(value);
    }

// Usage: make_result::Ok()
    Status Ok();

    // Usage: make_result::Fail(error)
    detail::Failure Fail(std::error_code error);

    // Precondition: result.HasError()
    template <typename T>
    detail::Failure PropagateError(Result<T> result) {
      return Fail(result.Error());
    }

// Convert status code (error or success) to Result
    Status ToStatus(std::error_code error);

    template <typename T>
    Status JustStatus(Result<T> result) {
      if (result.IsOk()) {
        return Ok();
      }
      return Status::Fail(result.Error());
    }

}  // namespace make_result

#endif //EFFECTIVE_BROCOLLI_RESULT_HPP

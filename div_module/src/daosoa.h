#ifndef DAOSOA_H_
#define DAOSOA_H_

#include <vector>
#include <type_traits>
#include "backend.h"

namespace bdm {

//, template<typename Element> class Container = std::vector>
template <template <typename> class T, typename Backend=VcBackend>
class daosoa {
 public:
  // soa of type T
  using value_type = T<Backend>;
  using iterator = typename std::vector<value_type>::iterator;
  using const_iterator = typename std::vector<value_type>::const_iterator;

  daosoa() {}
  daosoa(size_t scalar_elements) : data_(scalar_elements / Backend::kVecLen + (scalar_elements % Backend::kVecLen ? 1 : 0)) {
    for ( auto& value : data_ ) {
      value.SetInitialized(false);
    }
  }

  explicit daosoa(const value_type& cell) {
    data_.push_back(cell);
  }

//  size_t elements() const {
//    auto s = vectors();
//    return (s - 1) * VecLength + data_[s - 1].vectors();
//  }

  /// \brief returns the number of SOA elements in this container
  size_t vectors() const { return data_.size(); }

  /// this function assumes that only the last vector may not be fully
  /// initialized
  size_t elements() const {
    if(vectors() != 0) {
      return (vectors() - 1) * Backend::kVecLen + data_[vectors() - 1].Size(); // fixme Size vectors
    } else {
      return 0;
    }
  }

  // todo improve this comment
  /// \brief adds `value` to the vector
  /// only gets compiled if T == T1
  template <typename T1>
  typename std::enable_if<std::is_same<value_type, T1>::value>::type push_back(
      const T1& value) {
    data_.push_back(value);
    // fixme broken: what if i first push_back a scalar element and then
    // a vector one??
  }

  // todo improve this comment
  /// \brief adds `scalar value` to the vector
  /// only gets compiled if T != T1 && T1 == ScalarBackend
  template <typename T1>
  typename std::enable_if<is_scalar<T1>::value &&
                          !std::is_same<value_type, T1>::value>::type
  push_back(const T1& value) {
    // this implementation is only allowed if T != T1 && T1 == ScalarBackend
    static_assert(is_scalar<T1>::value && !std::is_same<value_type, T1>::value,
                  "push_back of a non scalar type on a scalar type is not supported");
    if (data_.size() == 0) {
      value_type v;
      v.SetInitialized(false);
      data_.push_back(v);
    }
    auto last = &data_[data_.size() - 1];
    if (last->is_full()) {
      value_type v1;
      v1.SetInitialized(false);
      data_.push_back(v1);
      last = &data_[data_.size() - 1];
    }
    last->Append(value);
  }

  void Gather(const std::vector<int> indexes, daosoa<T, Backend>* ret) const {
    assert(ret->data_.size() * Backend::kVecLen >= indexes.size());
    size_t counter = 0;
    for (int idx : indexes) {
      const auto& scalar_element = GetScalar(idx);
//      ret->push_back(scalar_element);
      ret->SetScalar(counter++, scalar_element);
    }
  }

  /// \brief returns scalar representation of element at index
  T<ScalarBackend> GetScalar(size_t index) const {
    // fixme if this is ScalarBackend
    size_t vector_idx = index / Backend::kVecLen;
    size_t vec_el_idx = index % Backend::kVecLen;
    return data_[vector_idx].Get(vec_el_idx);
  }

  void SetScalar(size_t index, const T<ScalarBackend>& value) {
    // fixme if this is ScalarBackend
    size_t vector_idx = index / Backend::kVecLen;
    size_t vec_el_idx = index % Backend::kVecLen;
    return data_[vector_idx].Set(vec_el_idx, value);
  }

  value_type& operator[](std::size_t index) { return data_[index]; }
  const value_type& operator[](std::size_t index) const { return data_[index]; }

  iterator begin() { return data_.begin(); }
  iterator end() { return data_.end(); }

  const_iterator begin() const { return data_.cbegin(); }
  const_iterator end() const { return data_.cend(); }

//  void merge() {
//    // todo implement
//  }

 private:
  std::vector<value_type> data_;
};

}  // namespace bdm

#endif  // DAOSOA_H_
#pragma once

#include <vector>

#include "lazy_tensors/shape.h"

namespace torch_lazy_tensors {

class ShapeBuilder {
 public:
  explicit ShapeBuilder(c10::ScalarType type) : type_(type) {}

  ShapeBuilder& Add(const lazy_tensors::Shape& shape, int64_t dim);

  ShapeBuilder& Add(const lazy_tensors::Shape& shape,
                    c10::ArrayRef<int64_t> dimensions);

  ShapeBuilder& Add(int64_t size);

  lazy_tensors::Shape Build() const;

 private:
  struct ShapeDim {
    const lazy_tensors::Shape* shape = nullptr;
    int64_t dim_or_size = -1;
  };

  c10::ScalarType type_;
  std::vector<ShapeDim> dims_;
};

}  // namespace torch_lazy_tensors

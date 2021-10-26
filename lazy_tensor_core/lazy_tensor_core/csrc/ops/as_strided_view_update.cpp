#include "lazy_tensor_core/csrc/ops/as_strided_view_update.h"

#include "lazy_tensor_core/csrc/ops/as_strided.h"
#include "lazy_tensor_core/csrc/ops/ltc_ops.h"
#include "lazy_tensor_core/csrc/tensor_util.h"
#include "lazy_tensor_core/csrc/torch_util.h"
#include "lazy_tensors/shape_util.h"

namespace torch_lazy_tensors {
namespace ir {
namespace ops {

AsStridedViewUpdate::AsStridedViewUpdate(
    const torch::lazy::Value& target, const torch::lazy::Value& input,
    std::vector<int64_t> size,
    std::vector<int64_t> stride, int64_t storage_offset)
    : TsNode(ltc_as_strided_view_update, {target, input},
           [&]() {
             return lazy_tensors::ShapeUtil::MakeShape(
                 ir::GetShapeFromTsValue(target).at_element_type(), size);
           },
           /*num_outputs=*/1,
           torch::lazy::MHash(size, stride, storage_offset)),
      size_(std::move(size)),
      stride_(std::move(stride)),
      storage_offset_(storage_offset) {}

std::string AsStridedViewUpdate::ToString() const {
  std::stringstream ss;
  ss << TsNode::ToString() << ", size=(" << c10::Join(", ", size_)
     << "), stride=(" << c10::Join(", ", stride_)
     << "), storage_offset=" << storage_offset_;
  return ss.str();
}

NodePtr AsStridedViewUpdate::Clone(OpList operands) const {
  return torch::lazy::MakeNode<AsStridedViewUpdate>(operands.at(0), operands.at(1), size_,
                                       stride_, storage_offset_);
}

}  // namespace ops
}  // namespace ir
}  // namespace torch_lazy_tensors

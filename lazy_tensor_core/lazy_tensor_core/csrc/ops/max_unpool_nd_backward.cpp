#include "lazy_tensor_core/csrc/ops/max_unpool_nd_backward.h"

#include "lazy_tensor_core/csrc/compiler/node_lowering.h"
#include "lazy_tensors/computation_client/debug_macros.h"

namespace torch_lazy_tensors {
namespace ir {
namespace ops {
namespace {

c10::Symbol MaxUnpoolNdBackwardSymbol(int64_t spatial_dim_count) {
  switch (spatial_dim_count) {
    case 2:
      return at::aten::max_unpool2d_backward;
    case 3:
      return at::aten::max_unpool3d_backward;
    default:
      LTC_ERROR() << "Invalid number of spatial dimensions: "
                  << spatial_dim_count;
  }
}

}  // namespace

MaxUnpoolNdBackward::MaxUnpoolNdBackward(
    const torch::lazy::Value& grad_output, const torch::lazy::Value& input, const torch::lazy::Value& indices,
    std::vector<int64_t> output_size)
    : TsNode(torch::lazy::OpKind(MaxUnpoolNdBackwardSymbol(output_size.size())),
           {grad_output, input, indices},
           /*num_outputs=*/1, torch::lazy::MHash(output_size)),
      output_size_(std::move(output_size)) {
  SetShapeDeferred(
      [&]() { return compiler::NodeLowering::Get()->Infer(this); });
}

NodePtr MaxUnpoolNdBackward::Clone(OpList operands) const {
  return torch::lazy::MakeNode<MaxUnpoolNdBackward>(operands.at(0), operands.at(1),
                                       operands.at(2), output_size_);
}

std::string MaxUnpoolNdBackward::ToString() const {
  std::stringstream ss;
  ss << TsNode::ToString() << ", output_size=(" << c10::Join(", ", output_size_)
     << ")";
  return ss.str();
}

}  // namespace ops
}  // namespace ir
}  // namespace torch_lazy_tensors

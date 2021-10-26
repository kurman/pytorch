#include "lazy_tensor_core/csrc/ops/convolution_overrideable.h"

#include "lazy_tensor_core/csrc/compiler/node_lowering.h"
#include "lazy_tensors/computation_client/debug_macros.h"

namespace torch_lazy_tensors {
namespace ir {
namespace ops {

ConvolutionOverrideable::ConvolutionOverrideable(
    const torch::lazy::Value& input, const torch::lazy::Value& weight, const torch::lazy::Value& bias,
    std::vector<int64_t> stride,
    std::vector<int64_t> padding,
    std::vector<int64_t> dilation, bool transposed,
    std::vector<int64_t> output_padding, int64_t groups)
    : TsNode(torch::lazy::OpKind(at::aten::convolution_overrideable),
           {input, weight, bias},
           /*num_outputs=*/1,
           torch::lazy::MHash(stride, padding, dilation, transposed,
                                     output_padding, groups)),
      stride_(std::move(stride)),
      padding_(std::move(padding)),
      dilation_(std::move(dilation)),
      output_padding_(std::move(output_padding)),
      transposed_(transposed),
      groups_(groups) {
  SetShapeDeferred(
      [&]() { return compiler::NodeLowering::Get()->Infer(this); });
}

ConvolutionOverrideable::ConvolutionOverrideable(
    const torch::lazy::Value& input, const torch::lazy::Value& weight,
    std::vector<int64_t> stride,
    std::vector<int64_t> padding,
    std::vector<int64_t> dilation, bool transposed,
    std::vector<int64_t> output_padding, int64_t groups)
    : TsNode(torch::lazy::OpKind(at::aten::convolution_overrideable), {input, weight},
           /*num_outputs=*/1,
           torch::lazy::MHash(stride, padding, dilation, transposed,
                                     output_padding, groups)),
      stride_(std::move(stride)),
      padding_(std::move(padding)),
      dilation_(std::move(dilation)),
      output_padding_(std::move(output_padding)),
      transposed_(transposed),
      groups_(groups) {
  SetShapeDeferred(
      [&]() { return compiler::NodeLowering::Get()->Infer(this); });
}

NodePtr ConvolutionOverrideable::Clone(OpList operands) const {
  return operands.size() == 3
             ? torch::lazy::MakeNode<ConvolutionOverrideable>(
                   operands.at(0), operands.at(1), operands.at(2), stride_,
                   padding_, dilation_, transposed_, output_padding_, groups_)
             : torch::lazy::MakeNode<ConvolutionOverrideable>(
                   operands.at(0), operands.at(1), stride_, padding_, dilation_,
                   transposed_, output_padding_, groups_);
}

std::string ConvolutionOverrideable::ToString() const {
  std::stringstream ss;
  ss << TsNode::ToString() << ", stride=(" << c10::Join(", ", stride_)
     << "), padding=(" << c10::Join(", ", padding_) << "), dilation=("
     << c10::Join(", ", dilation_) << "), transpose=" << transposed_
     << ", output_padding=(" << c10::Join(", ", output_padding_)
     << "), groups=" << groups_;
  return ss.str();
}

}  // namespace ops
}  // namespace ir
}  // namespace torch_lazy_tensors

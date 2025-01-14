/*
NATTEN1D-AV TORCH EXTENSION (CUDA)

This source code is licensed under the license found in the
LICENSE file in the root directory of this source tree.
*/
#include <torch/extension.h>
#include <vector>

// CUDA forward declarations
torch::Tensor natten1dav_cuda_forward(
    const torch::Tensor &attn,
    const torch::Tensor &value,
    const int dilation);

torch::Tensor natten1dav_cuda_forward_fp16(
    const torch::Tensor &attn,
    const torch::Tensor &value,
    const int dilation);

// CUDA backward declarations
std::vector<torch::Tensor> natten1dav_cuda_backward(
    const torch::Tensor &d_out,
    const torch::Tensor &attn,
    const torch::Tensor &value,
    const int dilation);

std::vector<torch::Tensor> natten1dav_cuda_backward_fp16(
    const torch::Tensor &d_out,
    const torch::Tensor &attn,
    const torch::Tensor &value,
    const int dilation);

// C++ interface
#define CHECK_CUDA(x) TORCH_CHECK(x.device().is_cuda(), #x " must be a CUDA tensor")
#define CHECK_CONTIGUOUS(x) TORCH_CHECK(x.is_contiguous(), #x " must be contiguous")
#define CHECK_INPUT(x) CHECK_CUDA(x); CHECK_CONTIGUOUS(x)

torch::Tensor natten1dav_forward(
    const torch::Tensor &attn,
    const torch::Tensor &value,
    const int dilation) {
    CHECK_INPUT(attn);
    CHECK_INPUT(value);
    bool half = ::detail::scalar_type(value.scalar_type()) == at::ScalarType::Half;
    if (half)
        return natten1dav_cuda_forward_fp16(attn, value, dilation);
    return natten1dav_cuda_forward(attn, value, dilation);
}

std::vector<torch::Tensor> natten1dav_backward(
    const torch::Tensor &d_out,
    const torch::Tensor &attn,
    const torch::Tensor &value,
    const int dilation) {
    CHECK_INPUT(d_out);
    CHECK_INPUT(attn);
    CHECK_INPUT(value);
    bool half = ::detail::scalar_type(value.scalar_type()) == at::ScalarType::Half;
    if (half)
        return natten1dav_cuda_backward_fp16(d_out, attn, value, dilation);
    return natten1dav_cuda_backward(d_out, attn, value, dilation);
}


PYBIND11_MODULE(TORCH_EXTENSION_NAME, m) {
  m.def("forward", &natten1dav_forward, "NATTEN1DAV forward (CUDA)");
  m.def("backward", &natten1dav_backward, "NATTEN1DAV backward (CUDA)");
}

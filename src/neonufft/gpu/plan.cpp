#include "neonufft/config.h"
//---

#include "neonufft/gpu/plan.hpp"
#include "neonufft/gpu/plan_impl.hpp"
#include "neonufft/gpu/plan_t3_impl.hpp"
#include <memory>

namespace neonufft {
namespace gpu {

template <typename T, std::size_t DIM>
Plan<T, DIM>::Plan(Options opt, int sign, IntType num_nu, std::array<const T*, DIM> loc,
                   std::array<IntType, DIM> modes, StreamType stream,
                   std::shared_ptr<Allocator> device_alloc)
    : impl_(decltype(impl_)(
          new gpu::PlanImpl<T, DIM>(opt, sign, num_nu, loc, modes, stream, std::move(device_alloc)),
          [](void* ptr) {
            if (ptr) {
              delete reinterpret_cast<gpu::PlanImpl<T, DIM>*>(ptr);
            }
          })) {}

template <typename T, std::size_t DIM>
void Plan<T, DIM>::transform_type_1(const ComplexType<T> *in,
                                    ComplexType<T> *out,
                                    std::array<IntType, DIM> out_strides) {
  reinterpret_cast<gpu::PlanImpl<T, DIM> *>(impl_.get())
      ->transform_type_1(in, out, out_strides);
}

template <typename T, std::size_t DIM>
void Plan<T, DIM>::transform_type_2(const ComplexType<T> *in,
                                    std::array<IntType, DIM> in_strides,
                                    ComplexType<T> *out) {
  reinterpret_cast<gpu::PlanImpl<T, DIM> *>(impl_.get())
      ->transform_type_2(in, in_strides, out);
}

template <typename T, std::size_t DIM>
void Plan<T, DIM>::set_points(IntType num_nu,
                                       std::array<const T *, DIM> loc) {
  reinterpret_cast<gpu::PlanImpl<T, DIM> *>(impl_.get())
      ->set_points(num_nu, loc);
}

template class Plan<float, 1>;
template class Plan<float, 2>;
template class Plan<float, 3>;

template class Plan<double, 1>;
template class Plan<double, 2>;
template class Plan<double, 3>;



template <typename T, std::size_t DIM>
std::uint64_t PlanT3<T, DIM>::grid_memory_size(const Options &opt,
                                               std::array<T, DIM> input_min,
                                               std::array<T, DIM> input_max,
                                               std::array<T, DIM> output_min,
                                               std::array<T, DIM> output_max) {
  return PlanT3Impl<T, DIM>::grid_memory_size(opt, input_min, input_max,
                                              output_min, output_max);
}

template <typename T, std::size_t DIM>
PlanT3<T, DIM>::PlanT3(Options opt, int sign, IntType num_in,
                       std::array<const T*, DIM> input_points, IntType num_out,
                       std::array<const T*, DIM> output_points, StreamType stream,
                       IntType batch_size, std::shared_ptr<Allocator> device_alloc)
    : impl_(decltype(impl_)(new PlanT3Impl<T, DIM>(batch_size, opt, sign, num_in, input_points,
                                                   num_out, output_points, stream, device_alloc),
                            [](void* ptr) {
                              if (ptr) {
                                delete reinterpret_cast<PlanT3Impl<T, DIM>*>(ptr);
                              }
                            })) {}

template <typename T, std::size_t DIM>
PlanT3<T, DIM>::PlanT3(Options opt, int sign, std::array<T, DIM> input_min,
                       std::array<T, DIM> input_max, std::array<T, DIM> output_min,
                       std::array<T, DIM> output_max, StreamType stream, IntType batch_size,
                       std::shared_ptr<Allocator> device_alloc)
    : impl_(decltype(impl_)(new PlanT3Impl<T, DIM>(batch_size, opt, sign, input_min, input_max,
                                                   output_min, output_max, stream, device_alloc),
                            [](void* ptr) {
                              if (ptr) {
                                delete reinterpret_cast<PlanT3Impl<T, DIM>*>(ptr);
                              }
                            })) {}

template <typename T, std::size_t DIM>
void PlanT3<T, DIM>::transform( ComplexType<T> *out, IntType bdist) {
  reinterpret_cast<PlanT3Impl<T, DIM>*>(impl_.get())->transform(out, bdist);
}

template <typename T, std::size_t DIM>
void PlanT3<T, DIM>::add_input(const ComplexType<T> *in, IntType bdist) {
  reinterpret_cast<PlanT3Impl<T, DIM>*>(impl_.get())->add_input(in, bdist);
}

template <typename T, std::size_t DIM>
  void PlanT3<T, DIM>::set_input_points(IntType num_in,
                              std::array<const T *, DIM> input_points){
  reinterpret_cast<PlanT3Impl<T, DIM> *>(impl_.get())
      ->set_input_points(num_in, input_points);
}

template <typename T, std::size_t DIM>
void PlanT3<T, DIM>::set_output_points(
    IntType num_out, std::array<const T *, DIM> output_points) {
  reinterpret_cast<PlanT3Impl<T, DIM> *>(impl_.get())
      ->set_output_points(num_out, output_points);
}

template <typename T, std::size_t DIM>
void PlanT3<T, DIM>::reset() {
  reinterpret_cast<PlanT3Impl<T, DIM>*>(impl_.get())->reset();
}

template class PlanT3<float, 1>;
template class PlanT3<float, 2>;
template class PlanT3<float, 3>;

template class PlanT3<double, 1>;
template class PlanT3<double, 2>;
template class PlanT3<double, 3>;


} // namespace gpu
} // namespace neonufft

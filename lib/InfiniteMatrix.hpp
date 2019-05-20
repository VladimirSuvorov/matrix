#ifndef MATRIX_PROJECT_API_INFINITE_MATRIX_HPP
#include "InfiniteMatrixTraits.hpp"
#include "IndexBuilder.hpp"
#define MATRIX_PROJECT_API_INFINITE_MATRIX_HPP
namespace matrix_project::api {
	template<typename InfiniteMatrixTraits>
	struct InfiniteMatrix : private InfiniteMatrixTraits::value_source_t {
		using traits = InfiniteMatrixTraits;
		using base = typename traits::value_source_t;
		using complex_index_t = typename traits::complex_index_t;
		using last_index_type = typename traits::last_index_type;
		using size_type = typename traits::size_type;

		using base_value_at_result = decltype(std::declval<base>().value_at(std::declval<complex_index_t>()));

		static constexpr bool use_indexators = (traits::last_arity > 1);
		using indexing_result = std::conditional_t<
			use_indexators,
			implementation::IndexBuilder<
				traits::last_arity,
				traits
			>,
			base_value_at_result
		>;

		indexing_result operator[](last_index_type index) {
			if constexpr (traits::last_arity > 1) {
				return { index, *this };
			}
			else {
				complex_index_t complex_index;
				traits::add_index_to<traits::last_arity>(complex_index, index);
				return traits::value_at(*this, complex_index);
			}
		}
		size_type size()const {
			return traits::size(*this);
		}


		auto begin() {
			return traits::begin(*this);
		}
		auto end() {
			return traits::end(*this);
		}
	};
}
#endif
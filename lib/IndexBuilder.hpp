#ifndef MATRIX_PROJECT_IMPELEMENTATION_INDEX_BUILDER_HPP
#define MATRIX_PROJECT_IMPELEMENTATION_INDEX_BUILDER_HPP
namespace matrix_project::implementation {
	///IndexBuilder takes current arity as 1st argument, N for N-dimentional space,
	///it provides N-1 indices, therefore, for N dimentional IndexBuilder you have to invoke operator[] (N-1) times.
	///Note: this is done, because another one invokation of the operator[] you have to do on the container. 
	///
	///
	///such invokation sequence (pseudocode):
	///
	///			n-th indexing	(n-1)-th indexing								last indexing
	///Container<N>[]->IndexBuilder<N>[]->IndexBuilder<N-1>[]->...->IndexBuilder<2>[]->value
	///
	///You should notice, that last index builder is IndexBuilder<2> (index builder for 2-dimentional space is minimal).
	///It is done so, because you have no need to use any indexer, for 1-dimentional container,
	///because you can just invoke operator[] on the container, therefore all index builders are useful for 2 dimentional containers at least.
	template<size_t CurrentArity, typename InfiniteMatrixTraits>
	struct IndexBuilder {
		static_assert(CurrentArity > 2);
		static constexpr bool is_not_last = (InfiniteMatrixTraits::last_arity > CurrentArity);
		static constexpr size_t NextArity = (CurrentArity - 1);
		static constexpr size_t PreviousArity = (CurrentArity + 1);

		using current_index_type = typename InfiniteMatrixTraits::template index_type_for_dimention<CurrentArity>;
		using next_index_type = typename InfiniteMatrixTraits::template index_type_for_dimention<NextArity>;
		using complex_index_t = typename InfiniteMatrixTraits::complex_index_t;
		using value_source_t = typename InfiniteMatrixTraits::value_source_t;
		using previous_type = std::conditional_t<
			is_not_last,
			IndexBuilder<PreviousArity, InfiniteMatrixTraits>,
			value_source_t
		>;
		using next_type = IndexBuilder<NextArity, InfiniteMatrixTraits>;

		IndexBuilder(const current_index_type & index, previous_type & previous) :
			index_(index), previous_(previous)
		{}


		std::add_lvalue_reference_t<current_index_type> operator*() {
			return index_;
		}
		std::add_lvalue_reference_t<std::add_const_t<current_index_type>>  operator*()const {
			return index_;
		}

		next_type operator[](next_index_type index) {
			return { index, *this };
		}

	private:
		complex_index_t& addIndex(complex_index_t& complex_index) {
			if constexpr (is_not_last) {
				return previous_.addIndex(InfiniteMatrixTraits::add_index_to<CurrentArity>(index_, complex_index));
			}
			else return InfiniteMatrixTraits::add_index_to<CurrentArity>(index_, complex_index);
		}

		auto& getValue(const complex_index_t& complex_index) {
			if constexpr (is_not_last) {
				return previous_.getValue(complex_index);
			}
			else return InfiniteMatrixTraits::value_at(previous_, complex_index);
		}
	private:
		previous_type& previous_;
		current_index_type index_;
	private:
		friend struct next_type;
	};


	template<typename InfiniteMatrixTraits>
	struct IndexBuilder<2, InfiniteMatrixTraits> {
		static constexpr size_t CurrentArity = 2;
		static constexpr size_t NextArity = (CurrentArity - 1);
		static constexpr size_t PreviousArity = (CurrentArity + 1);
		static constexpr bool is_not_last = (InfiniteMatrixTraits::last_arity > CurrentArity);
		using current_index_type = typename InfiniteMatrixTraits::template index_type_for_dimention<CurrentArity>;
		using next_index_type = typename InfiniteMatrixTraits::template index_type_for_dimention<NextArity>;
		using complex_index_t = typename InfiniteMatrixTraits::complex_index_t;
		using value_source_t = typename InfiniteMatrixTraits::value_source_t;


		using previous_type = std::conditional_t<
			is_not_last,
			IndexBuilder<PreviousArity, InfiniteMatrixTraits>,
			value_source_t
		>;
		IndexBuilder(const current_index_type & index, previous_type & previous) :
			index_(index), previous_(previous)
		{}



		std::add_lvalue_reference_t<current_index_type> operator*() {
			return index_;
		}
		std::add_lvalue_reference_t<std::add_const_t<current_index_type>>  operator*()const {
			return index_;
		}

		auto& operator[](next_index_type next_index) {
			return getValue(next_index);
		}

	private:
		auto& getValue(next_index_type next_index) {
			if constexpr (is_not_last) {
				return previous_.getValue(buildIndex(next_index));
			}
			else {
				return InfiniteMatrixTraits::value_at(previous_, buildIndex(next_index));
			}
		}

		complex_index_t buildIndex(next_index_type next_index) {
			complex_index_t complex_index;
			InfiniteMatrixTraits::add_index_to<NextArity>(next_index, complex_index);
			InfiniteMatrixTraits::add_index_to<CurrentArity>(index_, complex_index);
			if constexpr (is_not_last) {
				return previous_.addIndex(complex_index);
			}
			else return complex_index;
		}

	private:
		previous_type& previous_;
		current_index_type index_;
	};
}
#endif
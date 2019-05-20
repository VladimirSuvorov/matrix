#ifndef MATRIX_PROJECT_UTILITY_TUPLE_EXTENSIONS_HPP
#include <tuple>
#define MATRIX_PROJECT_UTILITY_TUPLE_EXTENSIONS_HPP
namespace matrix_project::utility {


	///\brief assignment function for tuples.
	///
	///
	///\param begin_index - assign from this index of TupleFrom and (begin_index+offset) of TupleTo (inclusive)
	///\param end_index - assign to this index of TupleFrom and (end_index+offset) of TupleTo (exclusive)
	///\param offset - offset of TupleTo, used to assign not only from begin_index.
	///\param TupleFrom - tuple assign from
	///\param TupleTo - tuple assign to
	template<size_t begin_index, size_t end_index, size_t offset = 0, typename TupleFrom, typename TupleTo>
	void assign_tuple(TupleFrom&& from, TupleTo&& to);
	namespace {
		template<size_t begin_index, size_t end_index, size_t offset = 0, bool not_complete = (begin_index != end_index)>
		struct assign_tuple_implementation {
			template<typename TupleFrom, typename TupleTo>
			static constexpr void assign(TupleFrom&& from, TupleTo&& to) {
				using namespace std;
				if constexpr (!is_const_v<decltype(get<(begin_index + offset)>(to))>) {
					get<(begin_index + offset)>(to) = get<begin_index>(from);
				}
				assign_tuple_implementation<(begin_index + 1), end_index, offset>
					::assign(forward<TupleFrom>(from), forward<TupleTo>(to));
			}
		};
		template<size_t begin_index, size_t end_index, size_t offset>
		struct assign_tuple_implementation<begin_index, end_index, offset, false> {
			template<typename TupleFrom, typename TupleTo>
			static constexpr void assign(TupleFrom&& from, TupleTo&& to) {}
		};
	}

	template<size_t begin_index, size_t end_index, size_t offset, typename TupleFrom, typename TupleTo>
	void assign_tuple(TupleFrom&& from, TupleTo&& to) {
		using namespace std;
		assign_tuple_implementation<begin_index, end_index, offset>
			::assign(forward<TupleFrom>(from), forward<TupleTo>(to));
	}

	namespace tuple_comparsion {
		template<size_t index, typename TupleT, typename TupleU>
		constexpr static bool less_at(TupleT&& lhs, TupleU&& rhs) {
			using namespace std;
			return (get<index>(lhs) < get<index>(rhs));
		}
		template<size_t index, typename TupleT, typename TupleU>
		constexpr static bool equals_at(TupleT&& lhs, TupleU&& rhs) {
			using namespace std;
			return (get<index>(lhs) == get<index>(rhs));
		}
		template<size_t index, typename TupleT, typename TupleU>
		constexpr static bool different_at(TupleT && lhs, TupleU && rhs) {
			using namespace std;
			return (get<index>(lhs) != get<index>(rhs));
		}
		template<size_t index, typename TupleT, typename TupleU>
		constexpr static bool greater_at(TupleT && lhs, TupleU && rhs) {
			using namespace std;
			return (get<index>(lhs) > get<index>(rhs));
		}
		template<size_t index>
		struct compare_same_sized_tuples_less_implementation {
			using base =  compare_same_sized_tuples_less_implementation<(index - 1)>;
			template<typename TupleT, typename TupleU>
			constexpr static bool less(TupleT&& lhs, TupleU&& rhs) {
				using namespace std;
				if (different_at<index>(std::forward<TupleT>(lhs), std::forward<TupleU>(rhs)))
					return less_at<index>(std::forward<TupleT>(lhs), std::forward<TupleU>(rhs));
				return base::less(std::forward<TupleT>(lhs), std::forward<TupleU>(rhs));
			}
		};
		template<>
		struct compare_same_sized_tuples_less_implementation<0> {
			template<typename TupleT, typename TupleU>
			constexpr static bool less(TupleT&& lhs, TupleU&& rhs) {
				using namespace std;
				return less_at<0>(forward<TupleT>(lhs), forward<TupleU>(rhs));
			}
		};
		template<typename TupleT, typename TupleU, size_t size = std::tuple_size_v<std::decay_t<TupleT>>>
		constexpr bool is_less(TupleT && lhs, TupleU && rhs) {
			using namespace std;
			using TupleT_core_type = std::decay_t<TupleT>;
			using TupleU_core_type = std::decay_t<TupleU>;

			constexpr bool different_size(tuple_size_v<TupleT_core_type> != tuple_size_v<TupleU_core_type>);
			if constexpr (different_size) {
				constexpr bool result(tuple_size_v<TupleT_core_type> < tuple_size_v<TupleU_core_type>);
				return result;
			}
			else
				return compare_same_sized_tuples_less_implementation<(size - 1)>::less(forward<TupleT>(lhs), forward<TupleU>(rhs));
		}
	}
	namespace {
		template<typename T, typename U>
		constexpr bool is_decaied_same_v = std::is_same_v<std::decay_t<T>, std::decay_t<U>>;
		template<typename PatternType, typename...Ts>
		using type_match =
			std::enable_if_t<(
				is_decaied_same_v<Ts, PatternType>
				&&
				...
				),
			PatternType>;
	}
	template<typename Tuple>
	struct TupleComparator {
		template<
			typename T,
			typename U,
			typename = type_match<Tuple, T, U>
		>
			constexpr bool operator()(T&& lhs, U&& rhs)const {
			using namespace std;
			return tuple_comparsion::is_less(forward<T>(lhs), forward<U>(rhs));
		}
	};
}
#endif
#ifndef MATRIX_PROJECT_UTILITY_AT_HPP
#include "types_count.hpp"
#define MATRIX_PROJECT_UTILITY_AT_HPP
namespace matrix_project::utility {
	namespace {
		//Takes the first element of any given typesequence,
		//e.g. it will take T0 as result_t for T0, T1, T2 ... TN typesequence; 
		//for each PopFront::pop_next it will return PopFront instance with same typesequence, 
		//except without first one type.
		//Therefore e.g. PopFront<T0...TN>::pop_next::pop_next::pop_next::result_t = T3.
		template<typename...Ts>
		struct PopFront;
		template<typename T, typename...Ts>
		struct PopFront<T, Ts...> {
			using pop_next = PopFront<Ts...>;
			using result_t = T;
		};
		template<>
		struct PopFront<> {};

		//At<index, T0 ... T[N-1]>::result_t = T<index>
		//At does not check for an index to be less than count of a given types (T0 ... T[N-1]),
		//therefore you should avoid to use it directly.
		template<size_t index, typename...Ts>
		struct At {
			using previous = typename At<(index - 1), Ts...>::previous_t;//temp for g++ only
			using previous_t = typename previous::pop_next;
			using result_t = typename previous_t::result_t;
		};
		//Check_At Provides a different specializations for a given index and typesequence,
		//depending on if a given index less than count of a given types: T0 ... T[N-1],
		//to prevent a VERY LONG error message to appear if an index wouldn't be less than count of a given types (T0 ... T[N-1]).
		template<size_t index, bool indexAtBounds, typename...Ts>
		struct Check_At;
		//true-case (for case, when index in bounds) spesialization of Check_At, provides the At::result_t as is.	
		template<size_t index, typename...Ts>
		struct Check_At<index, true, Ts...> {
			using result_t = typename At<index, Ts...>::result_t;
		};
		//false-case (for case, when index out of bounds) spesialization of Check_At, provides readable error message	
		template<size_t index, typename...Ts>
		struct Check_At<index, false, Ts...> {
			static_assert((index < types_count_v<Ts...>), "Index out of bounds exception. Won't compile!");
		};

		//Provides result_t = Check_At specialization, depending on if index in bounds
		template<size_t index, typename...Ts>
		struct Checked_At {
			using result_t = typename Check_At<index, (index < types_count_v<Ts...>), Ts...>::result_t;
		};

		//Special cases of At:
		template<typename...Ts>
		struct At<1, Ts...> {
			using previous_t = typename PopFront<Ts...>::pop_next;
			using result_t = typename previous_t::result_t;
		};
		template<typename...Ts>
		struct At<0, Ts...> {
			using result_t = typename PopFront<Ts...>::result_t;
		};
	}

	///\brief a type of typesequence Ts, at a given index
	template<size_t index, typename...Ts>
	using at_t = typename Checked_At<index, Ts...>::result_t;
}
#endif

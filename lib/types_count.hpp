#ifndef MATRIX_PROJECT_UTILITY_TYPES_COUNT_HPP
#define MATRIX_PROJECT_UTILITY_TYPES_COUNT_HPP
namespace matrix_project::utility {
	namespace {
		template<typename...Ts>
		struct types_count;
		template<typename T, typename...Ts>
		struct types_count<T, Ts...> {
			static constexpr size_t value = (1 + types_count<Ts...>::value);
		};
		template<>
		struct types_count<> {
			static constexpr size_t value = 0;
		};
	}

	//encounts types at typesequence
	template<typename...Ts>
	constexpr size_t types_count_v = types_count<Ts...>::value;
}
#endif
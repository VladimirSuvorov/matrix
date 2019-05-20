#ifndef MATRIX_PROJECT_API_INFINITE_MATRIX_TRAITS_HPP
#include <map>
#include "at.hpp"
#include "tuple_extensions.hpp"
#include "ContainerProxy.hpp"
#define MATRIX_PROJECT_API_INFINITE_MATRIX_TRAITS_HPP
namespace matrix_project::api {


	///\brief Minimal implementation of typewrapper
	template<typename T, T Value, typename R = T>
	struct ConstantDefaultValue {
		using value_type = R;
		static constexpr value_type DefaultValue() {
			return Value;
		}
		static constexpr bool IsDefaultValue(const value_type& value) {
			return (value == Value);
		}
	};
	///\brief Possible implementation of InfiniteMatrixTraits
	///		NOTE: Constants, aliases and static methods marked with "\interface" are required to be implemented in
	///		any implementation of the traits. Of course, you can use other types, constants and methods you wish,
	///		but you should follow the expectations of interface: if something is a static method of N-params it should be
	///		such method anyway, as well as any constant must be defined as a constant and any alias must be defined as well
	///	
	///		NOTE: Anything marked with "\utility" is not required to be implemented and is used and therefore required
	///		only within this implementation.
	///
	///
	///\param DefaultValueT - typewrapper for a default value. 
	///			Is used to pass any default value: not only const and not only value passable as template parameter.
	///\param Is - index types.
	///
	///e.g. int, string, int, int, string 
	///will lead to the container indexed with [string][int][int][string][int] - in reverse order to given one
	template<typename DefaultValueT, typename...Is>
	struct InfiniteMatrixTraits {
		///\interface
		static constexpr size_t TotalArity = utility::types_count_v<Is...>;
		///\interface
		static constexpr auto last_arity = TotalArity;
		///\interface
		using complex_index_t = std::tuple<Is...>;

		///\interface
		using value_type =  typename DefaultValueT::value_type;

		///\interface
		using indexable_container_t = std::map<complex_index_t, value_type, utility::TupleComparator<complex_index_t>>;
		///\interface
		using value_source_t =  implementation::ContainerProxy<InfiniteMatrixTraits>;

		///\utility
		template<size_t CurrentArity>
		static constexpr size_t arity_to_index = (TotalArity - CurrentArity);
		///\interface
		template<size_t CurrentArity>
		using index_type_for_dimention = utility::at_t<arity_to_index<CurrentArity>, Is...>;
		///\interface
		using last_index_type = index_type_for_dimention<last_arity>;

		///\interface
		static constexpr value_type DefaultValue() {
			return DefaultValueT::DefaultValue();
		}
		///\interface
		static constexpr bool IsDefaultValue(const value_type& value) {
			return DefaultValueT::IsDefaultValue(value);
		}
		///\brief indexing
		///\interface
		template<size_t CurrentArity>
		static complex_index_t& add_index_to(const index_type_for_dimention<CurrentArity>& index, complex_index_t& complex_index) {
			std::get<arity_to_index<CurrentArity>>(complex_index) = index;
			return complex_index;
		}
		///\interface
		static auto& value_at(value_source_t& valueSource, const complex_index_t& complex_index) {
			return valueSource.value_at(complex_index);
		}
		///\interface
		template<typename T>
		static auto set_value(indexable_container_t& container, const complex_index_t& complex_index, T&& value)
			->decltype(container.insert_or_assign(complex_index, std::forward<T>(value))) {
			return container.insert_or_assign(complex_index, std::forward<T>(value));
		}


		///\brief size 
		///\interface
		using size_type = typename indexable_container_t::size_type;
		static size_type size(const indexable_container_t& container) {
			return std::size(container);
		}
		///\interface
		static size_type size(const value_source_t& valueSource) {
			return std::size(valueSource);
		}

		///\brief iterators
		///\interface
		static auto find(indexable_container_t& container, const complex_index_t& index) {
			return container.find(index);
		}
		///\interface
		static auto begin(indexable_container_t& container) {
			return std::begin(container);
		}
		///\interface
		static auto end(indexable_container_t& container) {
			return std::end(container);
		}
		///\interface
		static auto begin(value_source_t& valueSource) {
			return std::begin(valueSource);
		}
		///\interface
		static auto end(value_source_t& valueSource) {
			return std::end(valueSource);
		}
		///\utility
		template<typename T>
		struct transparent_wrapper {
			//Note: the wrapper is a complete workaround, that behave almost as type T.
			transparent_wrapper(T& t) noexcept : data_(&t) {}
			transparent_wrapper(T* t = nullptr) noexcept : data_(t) {}
			constexpr T& operator=(T& t) { return  *(data_ = &t); }

			//this is used for map (NOTE: the workaround)
			constexpr const T& operator=(const T & t) { return  *(data_ = const_cast<T*>(&t)); }
			constexpr T* operator=(T * t) noexcept { return data_ = t; }
			constexpr operator T& () { return *data_; }
			constexpr operator T* () noexcept { return data_; }
			constexpr operator const T& ()const { return *data_; }
			constexpr operator const T* ()const noexcept { return data_; }
		private:
			T* data_;
		};

		///\utility
		using iterator_value_type = std::tuple<transparent_wrapper<Is>..., transparent_wrapper<value_type>>;

		///\interface
		template<typename ContainerIterator>
		static iterator_value_type get_value(ContainerIterator container_iterator) {
			using namespace std;
			auto& [key, value] = *container_iterator;
			iterator_value_type result;
			constexpr size_t value_index = tuple_size_v<decay_t<decltype(key)>>;
			//Note: does same as (assume std): get<X>(result) = get<X>(key) where X is in range [0, value_index)
			matrix_project::utility::assign_tuple<0, value_index>(key, result);
			get<value_index>(result) = value;
			return result;
		}
	};
}
#endif
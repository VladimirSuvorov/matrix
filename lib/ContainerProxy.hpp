#ifndef MATRIX_PROJECT_IMPLEMENTATION_CONTAINER_PROXY_HPP
#include <tuple>
#include <optional>
#define MATRIX_PROJECT_IMPLEMENTATION_CONTAINER_PROXY_HPP

namespace matrix_project::implementation {
	///\brief Encapsulates the way how the data is stored, handles any data changes
	template<typename InfiniteMatrixTraits>
	struct ContainerProxy {
		using complex_index_t = typename InfiniteMatrixTraits::complex_index_t;
		using indexable_container_t = typename InfiniteMatrixTraits::indexable_container_t;
		using value_type =  typename InfiniteMatrixTraits::value_type;
		using size_type =  typename InfiniteMatrixTraits::size_type;
		using temporary_value_holder_t = std::optional<std::tuple<complex_index_t, value_type>>;

		value_type& value_at(const complex_index_t& index) {
			using namespace std;
			on_access();
			auto found = InfiniteMatrixTraits::find(container_, index);
			auto value =
				(found != InfiniteMatrixTraits::end(container_)) ?
				found->second :
				InfiniteMatrixTraits::DefaultValue();
			return get<1>(value_holder_.emplace(make_tuple(index, move(value))));
		}

		size_type size()const {
			on_access();
			return InfiniteMatrixTraits::size(container_);
		}
		template<typename ContainerIterator>
		struct Iterator {
			Iterator(ContainerIterator container_iterator) :
				iterator_(std::move(container_iterator))
			{}
			auto operator*() {
				return InfiniteMatrixTraits::get_value(iterator_);
			}
			const auto operator*()const {
				return InfiniteMatrixTraits::get_value(iterator_);
			}
			Iterator operator++() {
				return { ++iterator_ };
			}
			Iterator operator++(int) {
				return { iterator_++ };
			}
			Iterator operator--() {
				return { --iterator_ };
			}
			Iterator operator--(int) {
				return { iterator_-- };
			}
			bool operator==(Iterator& oth)const {
				return (iterator_ == oth.iterator_);
			}
			bool operator!=(Iterator & oth)const {
				return !(*this == oth);
			}


		private:
			ContainerIterator iterator_;
		};

		auto begin() {
			on_access();
			return Iterator(InfiniteMatrixTraits::begin(container_));
		}
		auto end() {
			on_access();
			return Iterator(InfiniteMatrixTraits::end(container_));
		}

	private:

		//on any access to the container, even if use iterator
		void on_access()const {
			if (value_holder_.has_value()) {
				auto& [complex_index, value] = *value_holder_;
				if (!InfiniteMatrixTraits::IsDefaultValue(value))//assign value at actual map index
					InfiniteMatrixTraits::set_value(container_, complex_index, value);
				else {
					auto found(InfiniteMatrixTraits::find(container_, complex_index));
					if (found != InfiniteMatrixTraits::end(container_))
						container_.erase(complex_index);//remove value from the map if the value == DefaultValue
				}
				value_holder_.reset();
			}
		}
	private:
		mutable temporary_value_holder_t value_holder_;
		mutable indexable_container_t container_;
	};
}
#endif // !MATRIX_PROJECT_IMPLEMENTATION_CONTAINER_PROXY_HPP
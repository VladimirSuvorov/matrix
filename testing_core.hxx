#include <sstream>
#include <limits.h>
#include "lib/InfiniteMatrix.hpp"

template<typename T, T value>
using Matrix = matrix_project::api::InfiniteMatrix<
	matrix_project::api::InfiniteMatrixTraits<
	matrix_project::api::ConstantDefaultValue<T, value>,
	size_t, size_t
	>
>;




#define assert(condition) if(!(condition))return false//!THIS IS FOR TESTING PURPOSES! Dont use C assert instead



struct FloatDV {
	inline static float Value = 0.6653;

	using value_type = float;
	static constexpr value_type DefaultValue() {
		return Value;
	}
	static constexpr bool IsDefaultValue(const value_type& value) {
		return (value == Value);
	}
};
bool is_homework_correct() {
	using namespace std;
	stringstream cout;//!THIS IS FOR TESTING PURPOSES!

	Matrix<int, -1> matrix; // бесконечная матрица int заполнена значениями -1
	assert(matrix.size() == 0); // все ячейки свободны
	auto a = matrix[0][0];
	assert(a == -1);
	assert(matrix.size() == 0);
	matrix[100][100] = 314;
	assert(matrix[100][100] == 314);
	assert(matrix.size() == 1);
	// выведется одна строка
	// 100100314
	for (auto c : matrix)
	{
		int x;
		int y;
		int v;
		std::tie(x, y, v) = c;
		cout << x << y << v << std::endl;
	}
	string printed;
	cout >> printed;
	assert(printed == "100100314");




	Matrix<int, 0> m;//Note: at = index builder, *at = internal index from index builder
	for (auto at = m[0]; *at <= 9; ++ * at) {
		at[*at] = *at;
		at[9 - *at] = (9 - *at);
	}
	for (auto at = m[1]; *at < 9; ++ * at) {
		for (size_t j(1); j < 9; ++j) {
			if (j == *at)
				assert(at[j] == *at);
			else if (j == (9 - *at))
				assert(at[j] == (9 - *at));
			else
				assert(decltype(m)::traits::IsDefaultValue(at[j]));
		}
	}
	assert(m.size() == 18);


	cout << (((matrix[100][100] = 314) = 0) = 217);
	cout >> printed;
	assert(printed == "217");


	using ExtraM3x = matrix_project::api::InfiniteMatrix<
		matrix_project::api::InfiniteMatrixTraits<FloatDV, size_t, float, string, void*>
	>;
	ExtraM3x m3x;
	assert(m3x.size() == 0);
	auto b = m3x[3.14][numeric_limits<float>::infinity()]["one"][nullptr];
	assert(FloatDV::IsDefaultValue(b));
	assert(m3x.size() == 0);
	m3x[100][numeric_limits<float>::signaling_NaN()]["hundred"][&m3x] = 3.14f;
	//
	assert(m3x[100][numeric_limits<float>::signaling_NaN()]["hundred"][&m3x] == 3.14f);
	assert(m3x.size() == 1);
	for (auto c : m3x) {
		size_t x;
		float y;
		string z;
		void* a;
		float v;
		std::tie(x, y, z, a, v) = c;
		cout.clear(0, true);
		cout << "[" << x << "][" << y << "][" << z << "] = " << v;
		std::getline(cout, printed);
		assert(printed == "[100][nan][hundred] = 3.14");
	}
	return true;
}
#include <iostream>
#include <limits.h>
#include <assert.h>
#include "lib/InfiniteMatrix.hpp"







struct FloatDV {
	static constexpr float Value = 0.6653;

	using value_type = float;
	static constexpr value_type DefaultValue() {
		return Value;
	}
	static constexpr bool IsDefaultValue(const value_type& value) {
		return (value == Value);
	}
};
template<typename T, T value>
using Matrix = matrix_project::api::InfiniteMatrix<
	matrix_project::api::InfiniteMatrixTraits<
	matrix_project::api::ConstantDefaultValue<T, value>,
	size_t, size_t
	>
>;


void print_info(std::string_view info) {
	using namespace std;
	constexpr auto delimiter_line = "=======================================================";
	cout 
		<< delimiter_line << endl
		<< info << endl
		<< delimiter_line << endl
		<< endl;
}

int main(int argc, char* argv[]) {
	using namespace std;

	print_info("OTUS homework 6 example stuff:");
	Matrix<int, -1> matrix; // áåñêîíå÷íàÿ ìàòðèöà int çàïîëíåíà çíà÷åíèÿìè -1
	assert(matrix.size() == 0); // âñå ÿ÷åéêè ñâîáîäíû
	auto a = matrix[0][0];
	assert(a == -1);
	assert(matrix.size() == 0);
	matrix[100][100] = 314;
	assert(matrix[100][100] == 314);
	assert(matrix.size() == 1);
	// âûâåäåòñÿ îäíà ñòðîêà
	// 100100314
	for (auto c : matrix)
	{
		int x;
		int y;
		int v;
		std::tie(x, y, v) = c;
		std::cout << x << y << v << std::endl;
	}

	print_info("OTUS homework 6 stuff:");

	Matrix<int, 0> m;//Note: at = index builder, *at = internal index from index builder
	for (auto at = m[0]; *at <= 9; ++* at) {
		at[*at] = *at;
		at[9 - *at] = (9 - *at);
	}
	for (auto at = m[1]; *at < 9; ++*at) {
		for (size_t j(1); j < 9; ++j)
			cout << at[j] << " ";
		cout << endl;
	}
	cout << "actual size = " << m.size() << endl;
	for (auto c : m) {
		int x;
		int y;
		int v;
		tie(x, y, v) = c;
		cout<<"[" << x <<"]["<< y << "] = "<< v << endl;
	}
	cout << "'(((matrix[100][100] = 314) = 0) = 217)' = " << (((matrix[100][100] = 314) = 0) = 217) << endl;
	

	print_info("Extra stuff:");	
	using ExtraM3x = 
		matrix_project::api::InfiniteMatrix<
			matrix_project::api::InfiniteMatrixTraits<FloatDV, size_t, float, string, void*>
		>;
	ExtraM3x m3x;
	assert(m3x.size() == 0);
	auto b = m3x[3.14][numeric_limits<float>::infinity()]["one"][nullptr];
	assert(FloatDV::IsDefaultValue(b));
	assert(m3x.size() == 0);
	m3x[100][numeric_limits<float>::signaling_NaN()]["hundred"][&m3x] = 3.14f;
	assert(m3x[100][numeric_limits<float>::signaling_NaN()]["hundred"][&m3x] == 3.14f);
	assert(m3x.size() == 1);
	for (auto c : m3x) {

		size_t x;
		float y;
		string z;
		void* a;
		float v;
		std::tie(x, y, z, a, v) = c;
		std::cout<<"[" << x << "][" << y << "][" << z << "][" << a << "] = " << v << std::endl;
	}

	system("PAUSE");
}

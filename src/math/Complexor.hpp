/*
 * Complexor.h
 *
 *  Created on: 2017年5月8日
 *      Author: Peter
 */

#ifndef COMPLEXOR_H_
#define COMPLEXOR_H_

#include <iostream>
#include <iomanip>
#include <cstring>
#include "Matrix.hpp"
#include "../except_C++0x.hpp"

namespace complexor
{
	using namespace std;
	using namespace matrix;

	//友元函数声明
	template <class Type> class Complexor;

	template <class Type> const Complexor<Type> operator+(const Complexor<Type> &a, const Complexor<Type> &b) throw (invalid_argument);
	template <class Type> const Complexor<Type> operator-(const Complexor<Type> &a, const Complexor<Type> &b) throw (invalid_argument);

	template <class Type> const Complexor<Type> TransposeOf(const Complexor<Type> &src);

	template <class Type> const Complexor<Type> operator*(const Matrix &M, const Complexor<Type> &V) throw (invalid_argument);
	//矩阵乘以列向量, 返回向量
	template <class Type> const Complexor<Type> operator*(const Complexor<Type> &V, const Matrix &M) throw (invalid_argument);
	//行向量乘以矩阵, 返回向量
	template <class Type> const Type dot_product(const Complexor<Type> &a, const Complexor<Type> &b) throw (invalid_argument);
	//向量点积, 返回实数
	template <class Type> const Matrix operator*(const Complexor<Type> &a, const Complexor<Type> &b) throw (invalid_argument);
	//向量乘以向量, 返回矩阵
	template <class Type> const Complexor<Type> operator*(const double &k, const Complexor<Type> &b); //数k乘以向量
	template <class Type> const Complexor<Type> operator*(const Complexor<Type> &b, const double &k); //向量乘以数k
	template <class Type> Type abs(const Complexor<Type> &src); //向量的模
	template <class Type> Type angle(const Complexor<Type> &a, const Complexor<Type> &b) throw (invalid_argument);
	//向量夹角

	//类声明
	template <class Type>
	class Complexor
	{
		protected:
			Type *p;
			int num;
			bool vertical; //默认列向量

			void mem_init()
			{
				try {
					p = new Type[num];
				} catch (const bad_alloc &exct) {
					num = 0;
					throw;
				}
			}

			void resize(int new_num);

			void test_index(int num_test) const throw (out_of_range)
			{
				if (num_test < 0 || num_test >= num) {
					throw out_of_range("the complexor doesn't have the index");
				}
			}

		public:
//			Complexor(const int num = 0, const bool if_set0 = true, const bool vertical = true) throw (bad_alloc);
			Complexor(const int num = 0) throw (bad_alloc);
			Complexor(const int num, const Type &val, const bool vertical = true) throw (bad_alloc);
			Complexor(const Complexor &src) throw (bad_alloc); //拷贝构造函数
			Complexor(const Matrix &src, int index = 0, const bool vertical = true) throw (bad_alloc);
			Complexor(const Type src[], const int num, const bool vertical = true) throw (bad_alloc);
			Complexor(Type (*function)(), const int num, const bool vertical = true) throw (bad_alloc);
			Complexor(Type (*function)(int), const int num, const bool vertical = true) throw (bad_alloc);

#if __cplusplus >= 201103L //C++0x
			//Complexor 为 C++ 11 准备的新特性: 利用初始化列表进行构造
			Complexor(initializer_list<Type> src);

			//Complexor 为 C++ 11 准备的新特性: 转移构造函数
			Complexor(Complexor &&src);
#endif //C++0x

			operator Matrix();
			virtual ~Complexor();
			inline bool is_const();
			inline bool is_const() const;

			inline bool empty() const;
			inline void clear();

			const Complexor call(Type (*__pf)(Type));

			inline void set_element(int index, const Type &value) throw (out_of_range);
			inline Type& get_element(int index) throw (out_of_range);
			inline const Type& get_element(int index) const throw (out_of_range);
			inline int get_num() const;
			inline bool is_vertical() const;

			inline const Type* get_data() const;

			inline Type* const begin() const;
			inline Type* const end() const;

			inline size_t get_digit_size() const;

			virtual void print() const;

			Complexor& operator=(const Complexor &src);
#if __cplusplus >= 201103L //C++0x
			const Complexor& operator=(Complexor &&src); //转移赋值运算符
#endif //C++0x

			Type& operator[](int index) throw (out_of_range);
			const Type& operator[](int index) const throw (out_of_range);

			friend const Complexor operator+<>(const Complexor &a, const Complexor &b) throw (invalid_argument);
			//向量加法, 方向不同一律返回列向量
			friend const Complexor operator-<>(const Complexor &a, const Complexor &b) throw (invalid_argument);
			//向量减法, 方向不同一律返回列向量

			Complexor& operator+=(const Complexor &with) throw (invalid_argument);  //向量加法
			Complexor& operator-=(const Complexor &with) throw (invalid_argument);  //向量加法

			const Complexor operator+() const; //返回向量的反向同长向量
			const Complexor operator-() const; //返回向量的反向同长向量

			Complexor& doTranspose();
			friend const Complexor TransposeOf<>(const Complexor &src);

			friend const Complexor operator*<>(const Matrix &M, const Complexor &V) throw (invalid_argument);
			//矩阵乘以列向量, 返回向量
			friend const Complexor operator*<>(const Complexor &V, const Matrix &M) throw (invalid_argument);
			//行向量乘以矩阵, 返回向量
			friend const Type dot_product<>(const Complexor &a, const Complexor &b) throw (invalid_argument);
			//向量点积, 返回实数
			friend const Matrix operator*<>(const Complexor &a, const Complexor &b) throw (invalid_argument);
			//向量乘以向量, 返回矩阵
			friend const Complexor operator*<>(const double &k, const Complexor &b); //数k乘以向量
			friend const Complexor operator*<>(const Complexor &b, const double &k); //向量乘以数k

			friend Type abs<>(const Complexor &src); //向量的模

//			friend void print_array_to_file(const Complexor &src, string separator, string file_name, bool if_output_number =
//					false) throw (invalid_argument);

			friend Type angle<>(const Complexor &a, const Complexor &b) throw (invalid_argument);
			//向量夹角
//			friend const Complexor<Type> eqution(Matrix m) throw (invalid_argument);
	};
#if __cplusplus >= 201103L //C++0x
	template <class Type1, class Type2>
	auto operator+(const Complexor<Type1> &a, const Complexor<Type2> &b) throw (invalid_argument) ->const Complexor<decltype(a[0]+b[0])>;
	//两个不同类型向量的加法, 方向不同一律返回列向量, 自动推导返回类型, 例: Complexor<int> + Complexor<double> = Complexor<double>

	template <class Type1, class Type2>
	auto operator-(const Complexor<Type1> &a, const Complexor<Type2> &b) throw (invalid_argument)->const Complexor<decltype(a[0]-b[0])>;
//两个不同类型向量的减法, 方向不同一律返回列向量, 自动推导返回类型, 例: Complexor<int> - Complexor<double> = Complexor<double>

	template <class Type1, class Type2>
	auto dot_product(const Complexor<Type1> &a, const Complexor<Type2> &b) throw (invalid_argument) ->const decltype(a[0]*b[0]);
//向量点积
#endif //c++0x

} /* namespace complexor */

#endif /* COMPLEXOR_H_ */

#include "Complexor_base.hpp" /* 实现 */

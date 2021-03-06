/**
 * @file       static_vector.hpp
 * @brief
 * @date       2018-4-28
 * @author     Peter
 * @copyright
 *      Peter of [ThinkSpirit Laboratory](http://thinkspirit.org/)
 *   of [Nanjing University of Information Science & Technology](http://www.nuist.edu.cn/)
 *   all rights reserved
 */

#ifndef KERBAL_CONTAINER_STATIC_VECTOR_HPP
#define KERBAL_CONTAINER_STATIC_VECTOR_HPP

#include <kerbal/algorithm/sequence_compare.hpp>
#include <kerbal/assign/ilist.hpp>
#include <kerbal/compatibility/constexpr.hpp>
#include <kerbal/compatibility/noexcept.hpp>
#include <kerbal/container/static_container_exception.hpp>
#include <kerbal/iterator/iterator_traits.hpp>
#include <kerbal/iterator/reverse_iterator.hpp>
#include <kerbal/type_traits/array_traits.hpp>
#include <kerbal/type_traits/enable_if.hpp>

#include <cstddef>

#if __cplusplus >= 201103L
#	include <initializer_list>
#endif

#include <kerbal/container/detail/static_vector_base.hpp>
#include <kerbal/container/detail/static_vector_iterator.hpp>

namespace kerbal
{

	namespace container
	{

		/**
		 * @brief Array with flexible length that stored on automatic storage duration
		 * @details The class is an encapsulation class of array that could be stored on
		 *          automatic storage duration. It is more effective than std::vector
		 *          (especially when type parameter Tp is trivial type) because std::vector
		 *          store elements on heap storage duration (that's required memory allocation
		 *          and deallocation). Meanwhile the static_vector has more flexibility
		 *          than std::array for the reason that std::array has fixed length and
		 *          doesn't support operations like `insert` and `erase`. For further,
		 *          std::array and c-style array must call default constructor for initialization
		 *          when it was declared. However, as we all know, sometimes the default construct
		 *          operations are unnecessary.
		 * @tparam Tp Type of the elements.
		 * @tparam N The maximum number of elements that the array can hold.
		 */
		template <typename Tp, size_t N>
		class static_vector: protected kerbal::container::detail::static_vector_base<Tp, N>
		{
			private:
				typedef kerbal::container::detail::static_vector_base<Tp, N> super;

			public:
				typedef Tp							value_type;
				typedef const value_type			const_type;
				typedef value_type&					reference;
				typedef const value_type&			const_reference;
				typedef value_type*					pointer;
				typedef const value_type*			const_pointer;

#		if __cplusplus >= 201103L
				typedef value_type&&				rvalue_reference;
				typedef const value_type&&			const_rvalue_reference;
#		endif

				typedef std::size_t					size_type;
				typedef std::ptrdiff_t				difference_type;

				/// @brief 与该 static_vector 所等价的 C 风格数组的类型, 即 value_type[N]
				typedef value_type				equal_c_array[N];
				typedef equal_c_array&			equal_c_array_reference;
				typedef const equal_c_array&	equal_const_c_array_reference;
				typedef const value_type		const_equal_c_array[N];
				typedef const_equal_c_array&	const_equal_c_array_reference;

			private:
				typedef typename kerbal::type_traits::remove_all_extents<value_type>::type remove_all_extents_t;

				typedef typename super::storage_type	storage_type;

			public:

				/// @brief Iterator to static_vector.
				typedef detail::__stavec_iter<value_type>					iterator;
				/// @brief Constant iterator to static_vector.
				typedef detail::__stavec_kiter<value_type>					const_iterator;
				/// @brief Reverse iterator to static_vector.
				typedef kerbal::iterator::reverse_iterator<iterator>		reverse_iterator;
				/// @brief Constant reverse iterator to static_vector.
				typedef kerbal::iterator::reverse_iterator<const_iterator>	const_reverse_iterator;

			public:
				KERBAL_CONSTEXPR
				/** @brief Empty container constructor (Default constructor) */
				static_vector() KERBAL_NOEXCEPT;

				/**
				 * @brief Copy constructor
				 * @param src Another static_vector object of the same type (must have the same template arguments type and N)
				 */
				KERBAL_CONSTEXPR14
				static_vector(const static_vector & src);

#		if __cplusplus >= 201103L

				/**
				 * @brief Construct the array by coping the contents in initializer list
				 * @param src An initializer list
				 * @warning Compile terminate if the length of the initializer list large than the arg N of the static_vector
				 * @warning The constructor only be provided under the environment of C++11 standard
				 */
				KERBAL_CONSTEXPR14
				static_vector(std::initializer_list<value_type> src);
#		else

				static_vector(const kerbal::assign::assign_list<value_type> & src);

#		endif

#		if __cplusplus >= 201103L

				KERBAL_CONSTEXPR14
				static_vector(static_vector && src);

#		endif

				KERBAL_CONSTEXPR14
				explicit static_vector(size_type n);

				KERBAL_CONSTEXPR14
				static_vector(size_type n, const_reference val);

			private:

				template <typename InputIterator>
				KERBAL_CONSTEXPR14
				void __range_copy_constructor(InputIterator first, InputIterator last, std::input_iterator_tag) KERBAL_NOEXCEPT;

				template <typename RandomAccessIterator>
				KERBAL_CONSTEXPR14
				void __range_copy_constructor(RandomAccessIterator first, RandomAccessIterator last, std::random_access_iterator_tag) KERBAL_NOEXCEPT;

			public:

				/**
				 * @brief Range constructor
				 * @param begin the iterator that points to the range begin
				 * @param end the iterator that points to the range end
				 * @tparam InputIterator An input iterator type that points to elements of a type
				 * @warning If the range contains elements more than N, only the first N elements
				 *          will be used. The others will be ignored.
				 */
				template <typename InputIterator>
				KERBAL_CONSTEXPR14
				static_vector(InputIterator first, InputIterator last,
						typename kerbal::type_traits::enable_if<
								kerbal::iterator::is_input_compatible_iterator<InputIterator>::value
								, int
						>::type = 0
				);

				KERBAL_CONSTEXPR14
				static_vector& operator=(const static_vector & src);

#		if __cplusplus >= 201103L

				/**
				 * @brief Assign the array by using the content of an initializer list
				 * @param src the initializer list
				 * @return the reference to the array be assigned
				 */
				KERBAL_CONSTEXPR14
				static_vector& operator=(std::initializer_list<value_type> src);

#		else

				KERBAL_CONSTEXPR14
				static_vector& operator=(const kerbal::assign::assign_list<value_type> & src);

#		endif

			private:

				KERBAL_CONSTEXPR14
				void __assign(size_type new_size, const_reference val, kerbal::type_traits::false_type enable_optimization);

				KERBAL_CONSTEXPR14
				void __assign(size_type new_size, const_reference val, kerbal::type_traits::true_type enable_optimization);

			public:

				/**
				 * @brief Assign the array by using n value(s).
				 * @param new_size numbers of the value(s)
				 * @param val value
				 */
				KERBAL_CONSTEXPR14
				void assign(size_type new_size, const_reference val);

				/**
				 * @brief Assign the array by using a range of elements.
				 * @param first the iterator that points to the range begin
				 * @param last the iterator that points to the range end
				 * @tparam InputIterator An input iterator type that points to elements of a type
				 * @warning 若区间长度超出 static_vector 所能存放的最大元素数目, 超过部分将自动截断
				 */
				template <typename InputIterator>
				KERBAL_CONSTEXPR14
				typename kerbal::type_traits::enable_if<
						kerbal::iterator::is_input_compatible_iterator<InputIterator>::value
				>::type
				assign(InputIterator first, InputIterator last);

#		if __cplusplus >= 201103L

				/**
				 * @brief Assign the array by using the content of an initializer list
				 * @param src the initializer list
				 */
				KERBAL_CONSTEXPR14
				void assign(std::initializer_list<value_type> src);

#		else

				KERBAL_CONSTEXPR14
				void assign(const kerbal::assign::assign_list<value_type> & src);

#		endif

				/** @brief 返回指向数组首元素的迭代器 */
				KERBAL_CONSTEXPR14
				iterator begin() KERBAL_NOEXCEPT;

				/** @brief 返回指向数组末尾元素的后一个元素位置的迭代器 */
				KERBAL_CONSTEXPR14
				iterator end() KERBAL_NOEXCEPT;

				KERBAL_CONSTEXPR14
				const_iterator begin() const KERBAL_NOEXCEPT;

				/** @brief 返回指向数组末尾元素的后一个元素位置的迭代器 */
				KERBAL_CONSTEXPR14
				const_iterator end() const KERBAL_NOEXCEPT;

				KERBAL_CONSTEXPR
				const_iterator cbegin() const KERBAL_NOEXCEPT;

				KERBAL_CONSTEXPR
				const_iterator cend() const KERBAL_NOEXCEPT;

				KERBAL_CONSTEXPR14
				reverse_iterator rbegin() KERBAL_NOEXCEPT;

				KERBAL_CONSTEXPR14
				reverse_iterator rend() KERBAL_NOEXCEPT;

				KERBAL_CONSTEXPR14
				const_reverse_iterator rbegin() const KERBAL_NOEXCEPT;

				KERBAL_CONSTEXPR14
				const_reverse_iterator rend() const KERBAL_NOEXCEPT;

				KERBAL_CONSTEXPR
				const_reverse_iterator crbegin() const KERBAL_NOEXCEPT;

				KERBAL_CONSTEXPR
				const_reverse_iterator crend() const KERBAL_NOEXCEPT;

				KERBAL_CONSTEXPR14
				iterator nth(size_type index)
				{
					return this->begin() + index;
				}

				KERBAL_CONSTEXPR14
				const_iterator nth(size_type index) const
				{
					return this->cbegin() + index;
				}

				KERBAL_CONSTEXPR14
				size_type index_of(iterator it)
				{
					return it - this->begin();
				}

				KERBAL_CONSTEXPR
				size_type index_of(const_iterator it) const
				{
					return it - this->cbegin();
				}

				/**
				 * @brief Count the number of the elements that the array has contained.
				 * @return the number of the elements that the array has contained
				 */
				KERBAL_CONSTEXPR
				size_type size() const KERBAL_NOEXCEPT;

				/**
				 * @brief Returns the size() of the largest possible static_vector.
				 */
				KERBAL_CONSTEXPR
				size_type max_size() const KERBAL_NOEXCEPT
				{
					return N;
				}

				/**
				 * @brief Judge whether the array is empty.
				 * @return If the array is empty, return true, otherwise return false
				 */
				KERBAL_CONSTEXPR
				bool empty() const KERBAL_NOEXCEPT;

				void alert_empty() const
				{
					if (empty()) {
						kerbal::container::static_container_empty_exception::throw_this_exception();
					}
				}

				/**
				 * @brief Judge whether the array has been full.
				 * @return If the array has been full, return true, otherwise return false
				 */
				KERBAL_CONSTEXPR
				bool full() const KERBAL_NOEXCEPT;

				void alert_full() const
				{
					if (full()) {
						kerbal::container::static_container_full_exception::throw_this_exception(N);
					}
				}

				KERBAL_CONSTEXPR14
				reference operator[](size_type index) KERBAL_NOEXCEPT;

				KERBAL_CONSTEXPR14
				const_reference operator[](size_type index) const KERBAL_NOEXCEPT;

				reference at(size_type index);
				const_reference at(size_type index) const;

				/**
				 * @brief Get the reference of the element at the beginning of the array.
				 * @return the reference of the element at the beginning of the array.
				 */
				KERBAL_CONSTEXPR14
				reference front();

				/**
				 * @brief Get the const_reference of the element at the beginning of the array.
				 * @return the const_reference of the element at the beginning of the array.
				 */
				KERBAL_CONSTEXPR14
				const_reference front() const;

				/**
				 * @brief Get the reference of the element at the end of the array.
				 * @return the reference of the element at the end of the array.
				 */
				KERBAL_CONSTEXPR14
				reference back();

				/**
				 * @brief Get the const_reference of the element at the end of the array.
				 * @return the const_reference of the element at the end of the array.
				 */
				KERBAL_CONSTEXPR14
				const_reference back() const;

				/**
				 * @brief 返回与该 static_vector 所等价的 C 风格数组类型的引用, 方便与专门为 C 风格数组类型设计的 API 交互
				 * @return 与该 static_vector 所等价的 C 风格数组类型的引用
				 * @warning 必须保证数组元素存满时才可调用此方法
				 * @throw std::exception Throw this exception when call this method while the array is not full
				 */
				equal_c_array_reference c_arr();
				const_equal_c_array_reference c_arr() const;
				const_equal_c_array_reference const_c_arr() const;

				const_pointer data() const;

				/**
				 * @brief 在数组末尾插入参数 src 指定的元素
				 * @param src
				 */
				KERBAL_CONSTEXPR14
				void push_back(const_reference src);

#		if __cplusplus >= 201103L

				/**
				 * @brief 在数组末尾插入参数 src 指定的元素
				 * @param src
				 */
				KERBAL_CONSTEXPR14
				void push_back(rvalue_reference src);

#		endif

#		if __cplusplus >= 201103L

				template <typename ... Args>
				KERBAL_CONSTEXPR14
				reference emplace_back(Args&& ...args);

#		else

				reference emplace_back();

				template <typename Arg0>
				reference emplace_back(const Arg0& arg0);

				template <typename Arg0, typename Arg1>
				reference emplace_back(const Arg0& arg0, const Arg1& arg1);

				template <typename Arg0, typename Arg1, typename Arg2>
				reference emplace_back(const Arg0& arg0, const Arg1& arg1, const Arg2& arg2);

#		endif

				/**
				 * @brief 移除数组末尾的元素
				 */
				KERBAL_CONSTEXPR14
				void pop_back();

			private:
				KERBAL_CONSTEXPR14
				void __shrink_back_to(const_iterator to, kerbal::type_traits::false_type);

				KERBAL_CONSTEXPR14
				void __shrink_back_to(const_iterator to, kerbal::type_traits::true_type) KERBAL_NOEXCEPT;

			public:
				KERBAL_CONSTEXPR14
				void shrink_back_to(const_iterator to);


				KERBAL_CONSTEXPR14
				iterator insert(const_iterator pos, const_reference val);

				KERBAL_CONSTEXPR14
				iterator insert(size_type count, const_reference val);

				template <typename InputIterator>
				KERBAL_CONSTEXPR14
				iterator insert(const_iterator pos, const_reference first, const_reference last);

#		if __cplusplus >= 201103L

				KERBAL_CONSTEXPR14
				iterator insert(const_iterator pos, rvalue_reference val);

				KERBAL_CONSTEXPR14
				iterator insert(const_iterator pos, std::initializer_list<Tp> ilist);

#		endif

				KERBAL_CONSTEXPR14
				iterator emplace(const_iterator pos, const_reference src);

#		if __cplusplus >= 201103L

				template <typename ... Args>
				KERBAL_CONSTEXPR14
				iterator emplace(const_iterator pos, Args&& ...args);

#		else

				iterator emplace(const_iterator pos);

				template <typename Arg0>
				iterator emplace(const_iterator pos, const Arg0& arg0);

				template <typename Arg0, typename Arg1>
				iterator emplace(const_iterator pos, const Arg0& arg0, const Arg1& arg1);

				template <typename Arg0, typename Arg1, typename Arg2>
				iterator emplace(const_iterator pos, const Arg0& arg0, const Arg1& arg1, const Arg2& arg2);

#		endif

				KERBAL_CONSTEXPR14
				iterator erase(const_iterator pos);

				KERBAL_CONSTEXPR14
				iterator erase(const_iterator begin, const_iterator end);

				/**
				 * @brief Swap the array with another one.
				 * @param with another array to be swaped with
				 */
				KERBAL_CONSTEXPR14
				void swap(static_vector & with);

				/**
				 * @brief Clear all the elements in the array.
				 */
				KERBAL_CONSTEXPR14
				void clear();

				/**
				 * @brief Fill all the blank positions at the end of array by using default constructor of the element type.
				 */
				KERBAL_CONSTEXPR14
				void fill();

				/**
				 * @brief Fill all the blank positions at the end of array by copying the argument val.
				 * @param val
				 */
				KERBAL_CONSTEXPR14
				void fill(const_reference val);

				template <size_t M>
				KERBAL_CONSTEXPR14
				static_vector<Tp, N + M>
				operator+(const static_vector<Tp, M> & rhs) const;

				template <size_t M>
				KERBAL_CONSTEXPR14
				static_vector<Tp, N>
				operator*(const static_vector<Tp, M> & rhs) const;

			private:

#		if __cplusplus < 201103L

				void __construct_at(iterator);

				template <typename T0>
				void __construct_at(iterator, const T0&);

				template <typename T0, typename T1>
				void __construct_at(iterator, const T0&, const T1&);

				template <typename T0, typename T1, typename T2>
				void __construct_at(iterator, const T0&, const T1&, const T2&);

				template <typename T0, typename T1, typename T2, typename T3>
				void __construct_at(iterator, const T0&, const T1&, const T2&, const T3&);

				template <typename T0, typename T1, typename T2, typename T3, typename T4>
				void __construct_at(iterator, const T0&, const T1&, const T2&, const T3&, const T4&);


#		else

				template <typename ... Args>
				KERBAL_CONSTEXPR14
				void __construct_at(iterator, Args&& ...);

#		endif

				KERBAL_CONSTEXPR14
				void __destroy_at(iterator);

		};

		template <typename Tp, size_t M, size_t N>
		KERBAL_CONSTEXPR14
		bool operator==(const static_vector<Tp, M> & lhs, const static_vector<Tp, N> & rhs)
		{
			return kerbal::algorithm::sequence_equal_to(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
		}

		template <typename Tp, size_t M, size_t N>
		KERBAL_CONSTEXPR14
		bool operator!=(const static_vector<Tp, M> & lhs, const static_vector<Tp, N> & rhs)
		{
			return kerbal::algorithm::sequence_not_equal_to(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
		}

		template <typename Tp, size_t M, size_t N>
		KERBAL_CONSTEXPR14
		bool operator<(const static_vector<Tp, M> & lhs, const static_vector<Tp, N> & rhs)
		{
			return kerbal::algorithm::sequence_less(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
		}

		template <typename Tp, size_t M, size_t N>
		KERBAL_CONSTEXPR14
		bool operator<=(const static_vector<Tp, M> & lhs, const static_vector<Tp, N> & rhs)
		{
			return kerbal::algorithm::sequence_less_equal(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
		}

		template <typename Tp, size_t M, size_t N>
		KERBAL_CONSTEXPR14
		bool operator>(const static_vector<Tp, M> & lhs, const static_vector<Tp, N> & rhs)
		{
			return kerbal::algorithm::sequence_greater(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
		}

		template <typename Tp, size_t M, size_t N>
		KERBAL_CONSTEXPR14
		bool operator>=(const static_vector<Tp, M> & lhs, const static_vector<Tp, N> & rhs)
		{
			return kerbal::algorithm::sequence_greater_equal(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
		}

	} // namespace container

} // namespace kerbal

#include <kerbal/container/impl/static_vector.impl.hpp>

#endif // KERBAL_CONTAINER_STATIC_VECTOR_HPP

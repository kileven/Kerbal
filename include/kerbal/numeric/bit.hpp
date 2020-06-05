/**
 * @file       bit.hpp
 * @brief
 * @date       2019-9-30
 * @author     Peter
 * @copyright
 *      Peter of [ThinkSpirit Laboratory](http://thinkspirit.org/)
 *   of [Nanjing University of Information Science & Technology](http://www.nuist.edu.cn/)
 *   all rights reserved
 */

#ifndef KERBAL_NUMERIC_BIT_HPP
#define KERBAL_NUMERIC_BIT_HPP

#include <climits>

#include <kerbal/compatibility/constexpr.hpp>
#include <kerbal/compatibility/noexcept.hpp>
#include <kerbal/container/array.hpp>
#include <kerbal/type_traits/integral_constant.hpp>
#include <kerbal/type_traits/sign_deduction.hpp>

#include <climits>

namespace kerbal
{

	namespace numeric
	{

		template <typename Tp>
		KERBAL_CONSTEXPR14
		kerbal::container::array<bool, sizeof(Tp) * CHAR_BIT>
		bitarray(Tp x) KERBAL_NOEXCEPT
		{
			typedef kerbal::type_traits::integral_constant<size_t, sizeof(Tp) * CHAR_BIT> BIT_ARRAY_LEN;
			kerbal::container::array<bool, BIT_ARRAY_LEN::value> r;
			for (int i = 0; i < BIT_ARRAY_LEN::value; ++i) {
				r[BIT_ARRAY_LEN::value - 1 - i] = ((x >> i) & 1);
			}
			return r;
		}

		template <typename Tp>
		KERBAL_CONSTEXPR14
		kerbal::container::array<char, sizeof(Tp) * CHAR_BIT / 3 + (sizeof(Tp) * CHAR_BIT % 3 != 0)>
		octarray(Tp x) KERBAL_NOEXCEPT
		{
			typedef kerbal::type_traits::integral_constant<size_t, sizeof(Tp) * CHAR_BIT / 3 + (sizeof(Tp) * CHAR_BIT % 3 != 0)> OCT_ARRAY_LEN;
			kerbal::container::array<char, OCT_ARRAY_LEN::value> r;
			for (int i = 0; i < OCT_ARRAY_LEN::value; ++i) {
				char current = (x >> (i * 3)) & 7;
				current += '0';
				r[OCT_ARRAY_LEN::value - 1 - i] = current;
			}
			return r;
		}

		template <typename Tp>
		KERBAL_CONSTEXPR14
		kerbal::container::array<char, sizeof(Tp) * CHAR_BIT / 4 + (sizeof(Tp) * CHAR_BIT % 4 != 0)>
		hexarray(Tp x) KERBAL_NOEXCEPT
		{
			typedef kerbal::type_traits::integral_constant<size_t, sizeof(Tp) * CHAR_BIT / 4 + (sizeof(Tp) * CHAR_BIT % 4 != 0)> HEX_ARRAY_LEN;
			kerbal::container::array<char, HEX_ARRAY_LEN::value> r;
			for (int i = 0; i < HEX_ARRAY_LEN::value; ++i) {
				char current = (x >> (i * 4)) & 0xf;
				if (current < 10) {
					current += '0';
				} else {
					current += 'a' - 10;
				}
				r[HEX_ARRAY_LEN::value - 1 - i] = current;
			}
			return r;
		}



		template <typename Unsigned>
		KERBAL_CONSTEXPR14
		bool __popcount(Unsigned x, kerbal::type_traits::false_type) KERBAL_NOEXCEPT
		{
			int cnt = 0;
			while (x) {
				x &= x - 1;
				++cnt;
			}
			return cnt;
		}

		template <typename Signed>
		KERBAL_CONSTEXPR14
		bool __popcount(Signed x, kerbal::type_traits::true_type) KERBAL_NOEXCEPT
		{
			typedef typename kerbal::type_traits::make_unsigned<Signed>::type unsigned_t;
			unsigned_t u = static_cast<unsigned_t>(x);
			return __popcount(u, kerbal::type_traits::false_type());
		}

		/**
		 * Counts the number of 1 bits in the value of x.
		 */
		template <typename Tp>
		KERBAL_CONSTEXPR14
		int popcount(Tp x) KERBAL_NOEXCEPT
		{
			return kerbal::numeric::__popcount(x, kerbal::type_traits::is_signed<Tp>());
		}



		template <typename Unsigned>
		KERBAL_CONSTEXPR
		bool __ispow2(Unsigned x, kerbal::type_traits::false_type) KERBAL_NOEXCEPT
		{
			return x != 0 && ((x & (x - 1)) == 0);
		}

		template <typename Signed>
		KERBAL_CONSTEXPR
		bool __ispow2(Signed x, kerbal::type_traits::true_type) KERBAL_NOEXCEPT
		{
			return x > 0 && ((x & (x - 1)) == 0);
		}

		template <typename Tp>
		KERBAL_CONSTEXPR
		bool ispow2(Tp x) KERBAL_NOEXCEPT
		{
			return __ispow2(x, kerbal::type_traits::is_signed<Tp>());
		}



		template <typename Unsigned>
		KERBAL_CONSTEXPR
		bool __has_single_bit(Unsigned x, kerbal::type_traits::false_type) KERBAL_NOEXCEPT
		{
			return x != 0 && ((x & (x - 1)) == 0);
		}

		template <typename Signed>
		KERBAL_CONSTEXPR14
		bool __has_single_bit(Signed x, kerbal::type_traits::true_type) KERBAL_NOEXCEPT
		{
			typedef typename kerbal::type_traits::make_unsigned<Signed>::type unsigned_t;
			unsigned_t u = static_cast<unsigned_t>(x);
			return __has_single_bit(u, kerbal::type_traits::false_type());
		}

		template <typename Tp>
		KERBAL_CONSTEXPR
		bool has_single_bit(Tp x) KERBAL_NOEXCEPT
		{
			return __has_single_bit(x, kerbal::type_traits::is_signed<Tp>());
		}



		/**
		 * Generate 00...0011...11 (n 1 right)
		 *
		 * @warning Undefined behaviour if n < 0 or n > sizeof(Tp) * CHAR_BIT.
		 */
		template <typename Tp>
		KERBAL_CONSTEXPR
		Tp mask(int n) KERBAL_NOEXCEPT
		{
			typedef typename kerbal::type_traits::make_unsigned<Tp>::type unsigned_t;
			return n == sizeof(unsigned_t) * CHAR_BIT ?
					~static_cast<unsigned_t>(0) :
					~(~static_cast<unsigned_t>(0) << n);
		}

		template <typename Tp>
		KERBAL_CONSTEXPR
		Tp flip(Tp x, int pos) KERBAL_NOEXCEPT
		{
			typedef typename kerbal::type_traits::make_unsigned<Tp>::type unsigned_t;
			return x ^ (static_cast<unsigned_t>(1) << pos);
		}

		template <typename Tp>
		KERBAL_CONSTEXPR
		Tp set0(Tp x, int pos) KERBAL_NOEXCEPT
		{
			typedef typename kerbal::type_traits::make_unsigned<Tp>::type unsigned_t;
			return x & ~(static_cast<unsigned_t>(1) << pos);
		}

		template <typename Tp>
		KERBAL_CONSTEXPR
		Tp set1(Tp x, int pos) KERBAL_NOEXCEPT
		{
			typedef typename kerbal::type_traits::make_unsigned<Tp>::type unsigned_t;
			return x | (static_cast<unsigned_t>(1) << pos);
		}

		template <typename Tp>
		KERBAL_CONSTEXPR
		bool get(Tp x, int pos) KERBAL_NOEXCEPT
		{
			return (x >> pos) & 1;
		}



		template <typename Signed>
		KERBAL_CONSTEXPR
		Signed __rotl(Signed x, int s, kerbal::type_traits::true_type) KERBAL_NOEXCEPT
		{
			typedef typename kerbal::type_traits::make_unsigned<Signed>::type unsigned_t;
			return s == 0 ?
					x :
					s > 0 ?
					(static_cast<unsigned_t>(x) << s | (static_cast<unsigned_t>(x) >> ((sizeof(Signed) * CHAR_BIT) - s))) :
					(static_cast<unsigned_t>(x) >> -s | (static_cast<unsigned_t>(x) << ((sizeof(Signed) * CHAR_BIT) - (-s)))); // rotr(x, -s);
		}

		template <typename Unsigned>
		KERBAL_CONSTEXPR
		Unsigned __rotl(Unsigned x, int s, kerbal::type_traits::false_type) KERBAL_NOEXCEPT
		{
			return s == 0 ?
					 x :
					 (x << s | (x >> ((sizeof(Unsigned) * CHAR_BIT) - s)));
		}

		template <typename Tp>
		KERBAL_CONSTEXPR
		Tp rotl(Tp x, int s) KERBAL_NOEXCEPT
		{
			return __rotl(x, s % (sizeof(Tp) * CHAR_BIT), kerbal::type_traits::is_signed<Tp>());
		}

		template <typename Signed>
		KERBAL_CONSTEXPR
		Signed __rotr(Signed x, int s, kerbal::type_traits::true_type) KERBAL_NOEXCEPT
		{
			typedef typename kerbal::type_traits::make_unsigned<Signed>::type unsigned_t;
			return s == 0 ?
					x :
					s > 0 ?
					(static_cast<unsigned_t>(x) >> s | (static_cast<unsigned_t>(x) << ((sizeof(Signed) * CHAR_BIT) - s))) :
					(static_cast<unsigned_t>(x) << -s | (static_cast<unsigned_t>(x) >> ((sizeof(Signed) * CHAR_BIT) - (-s)))); // rotl(x, -s);
		}

		template <typename Unsigned>
		KERBAL_CONSTEXPR
		Unsigned __rotr(Unsigned x, int s, kerbal::type_traits::false_type) KERBAL_NOEXCEPT
		{
			return s == 0 ?
					 x :
					 (x >> s | (x << ((sizeof(Unsigned) * CHAR_BIT) - s)));
		}

		template <typename Tp>
		KERBAL_CONSTEXPR
		Tp rotr(Tp x, int s) KERBAL_NOEXCEPT
		{
			return __rotr(x, s % (sizeof(Tp) * CHAR_BIT), kerbal::type_traits::is_signed<Tp>());
		}

	} // namespace numeric

} // namespace kerbal

#endif // KERBAL_NUMERIC_BIT_HPP
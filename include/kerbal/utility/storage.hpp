/*
 * memory_storage.hpp
 *
 *  Created on: 2018年6月18日
 *      Author: peter
 */

#ifndef INCLUDE_KERBAL_UTILITY_STORAGE_HPP_
#define INCLUDE_KERBAL_UTILITY_STORAGE_HPP_

#include <chrono>
#include <stdint.h>
#include <kerbal/type_traits/cv_deduction.hpp>
#include <kerbal/type_traits/enable_if.hpp>
#include <kerbal/type_traits/integral_constant.hpp>

namespace kerbal
{
	namespace utility
	{
		template <typename size_type, typename Ratio = std::ratio<1, 1>>
		class storage : public std::chrono::duration<size_type, Ratio>
		{
			public:
				typedef std::chrono::duration<size_type, Ratio> super;
				typedef typename super::rep rep;
				typedef typename super::period period;

				using super::duration;
				using super::count;

				storage() :
						super()
				{
				}

				storage(const super & src) :
						super(src)
				{
				}
		};

		typedef std::ratio<1024, 1> kibi;
		typedef std::ratio<1024 * 1024, 1> mebi;
		typedef std::ratio<1024 * 1024 * 1024, 1> gibi;

		typedef storage<std::int64_t> Byte;
		typedef storage<std::int64_t, std::kilo> KB;
		typedef storage<std::int64_t, std::mega> MB;
		typedef storage<std::int64_t, std::giga> GB;

		typedef storage<std::int64_t, kibi> KiB;
		typedef storage<std::int64_t, mebi> MiB;
		typedef storage<std::int64_t, gibi> GiB;

		constexpr Byte operator""_Byte(unsigned long long x)
		{
			return Byte(x);
		}

		constexpr KB operator""_KB(unsigned long long x)
		{
			return KB(x);
		}

		constexpr MB operator""_MB(unsigned long long x)
		{
			return MB(x);
		}

		constexpr GB operator""_GB(unsigned long long x)
		{
			return GB(x);
		}

		constexpr KiB operator""_KiB(unsigned long long x)
		{
			return KiB(x);
		}

		constexpr MiB operator""_MiB(unsigned long long x)
		{
			return MiB(x);
		}

		constexpr GiB operator""_GiB(unsigned long long x)
		{
			return GiB(x);
		}

		template <typename>
		struct __is_storage_helper: kerbal::type_traits::false_type
		{
		};

		template <typename size_type, typename Ratio>
		struct __is_storage_helper<storage<size_type, Ratio> > : kerbal::type_traits::true_type
		{
		};

		template <typename Type>
		struct is_storage: __is_storage_helper<typename kerbal::type_traits::remove_cvref<Type>::type>
		{
		};

		template <typename ToStor, typename size_type, typename Ratio>
		constexpr
		typename kerbal::type_traits::enable_if<is_storage<ToStor>::value, ToStor>::type
		storage_cast(const storage<size_type, Ratio>& __d)
		{
			return ToStor(std::chrono::duration_cast<typename ToStor::super>(__d));
		}
	}
}



#endif /* INCLUDE_KERBAL_UTILITY_STORAGE_HPP_ */

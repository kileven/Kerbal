/*
 * reference_base.hpp
 *
 *  Created on: 2018年6月7日
 *      Author: peter
 */

#ifndef INCLUDE_KERBAL_REDIS_REDIS_DATA_STRUCT_REFERENCE_BASE_HPP_
#define INCLUDE_KERBAL_REDIS_REDIS_DATA_STRUCT_REFERENCE_BASE_HPP_

#include <string>

#include <kerbal/redis/operation.hpp>
#include <kerbal/redis/redis_type_cast.hpp>

namespace kerbal
{
	namespace redis
	{

		template <typename Type>
		class ConstReferenceBase
		{
			protected:
				const Context * pToContext = nullptr;
				std::string key;

			public:
				ConstReferenceBase(const Context & conn, const char key[]) :
						pToContext(&conn), key(key)
				{
				}

				ConstReferenceBase(const Context & conn, const std::string & key) :
						pToContext(&conn), key(key)
				{
				}

				bool exists() const
				{
					static Operation opt;
					return opt.exists(*pToContext, key);
				}

				operator Type() const
				{
					RedisCommand command("get %%s");
					AutoFreeReply reply = command.execute(*pToContext, key);
					switch (reply.replyType()) {
						case RedisReplyType::STRING: {
							return redisTypeCast<Type>(reply->str);
						}
						case RedisReplyType::NIL:
							throw RedisNilException(key);
						default:
							throw RedisUnexpectedCaseException(reply.replyType());
					}
				}

				friend std::ostream& operator<<(std::ostream& out, const ConstReferenceBase & src)
				{
					return out << (Type) src;
				}
		};

		template<typename Type>
		class ReferenceBase: public ConstReferenceBase<Type>
		{
			protected:
				typedef ConstReferenceBase<Type> supper_t;

			public:
				ReferenceBase(const Context & conn, const char key[]) :
						supper_t(conn, key)
				{
				}

				ReferenceBase(const Context & conn, const std::string & key) :
						supper_t(conn, key)
				{
				}

				ReferenceBase& operator=(const Type & src)
				{
					static RedisCommand command(std::string("set %%s %") + redis_type_traits<Type>::placeholder);
					command.execute(*supper_t::pToContext, supper_t::key, src);
					return *this;
				}

				friend std::istream& operator>>(std::istream& in, ReferenceBase & src)
				{
					Type buff;
					in >> buff;
					src = buff;
					return in;
				}
		};

		template<typename Type>
		class RealReference: public ReferenceBase<Type>
		{
			protected:
				typedef ReferenceBase<Type> supper_t;

			public:
				RealReference(const Context & conn, const char key[]) :
						supper_t(conn, key)
				{
				}

				RealReference(const Context & conn, const std::string & key) :
						supper_t(conn, key)
				{
				}

				RealReference& operator=(const Type & src)
				{
					supper_t::operator=(src);
					return *this;
				}

				template <typename AnotherType>
				RealReference& operator+=(const AnotherType & with)
				{
					return *this = (Type) (*this) + with;
				}

				template <typename AnotherType>
				RealReference& operator-=(const AnotherType & with)
				{
					return *this = (Type) (*this) - with;
				}

				template <typename AnotherType>
				RealReference& operator*=(const AnotherType & with)
				{
					return *this = (Type) (*this) * with;
				}

				template <typename AnotherType>
				RealReference& operator/=(const AnotherType & with)
				{
					return *this = (Type) (*this) / with;
				}
		};

		template<typename Type>
		class IntegerReference: public RealReference<Type>
		{
			protected:
				typedef RealReference<Type> supper_t;

			public:
				IntegerReference(const Context & conn, const char key[]) :
						supper_t(conn, key)
				{
				}

				IntegerReference(const Context & conn, const std::string & key) :
						supper_t(conn, key)
				{
				}

				IntegerReference& operator=(const Type & src)
				{
					supper_t::operator=(src);
					return *this;
				}

				IntegerReference& operator++()
				{
					RedisCommand command("incr %%s");
					AutoFreeReply reply = command.execute(*this->pToContext, this->key);
					switch (reply.replyType()) {
						case RedisReplyType::INTEGER:
							return *this;
						case RedisReplyType::NIL:
							throw RedisNilException(this->key);
						default:
							throw RedisUnexpectedCaseException();
					}
				}

				const Type operator++(int)
				{
					Type tmp = *this;
					RedisCommand command("incr %%s");
					AutoFreeReply reply = command.execute(*this->pToContext, this->key);
					switch (reply.replyType()) {
						case RedisReplyType::INTEGER:
							return tmp;
						case RedisReplyType::NIL:
							throw RedisNilException(this->key);
						default:
							throw RedisUnexpectedCaseException();
					}
				}

				IntegerReference& operator--()
				{
					RedisCommand command("decr %%s");
					AutoFreeReply reply = command.execute(*this->pToContext, this->key);
					switch (reply.replyType()) {
						case RedisReplyType::INTEGER:
							return *this;
						case RedisReplyType::NIL:
							throw RedisNilException(this->key);
						default:
							throw RedisUnexpectedCaseException();
					}
				}

				const Type operator--(int)
				{
					Type tmp = *this;
					RedisCommand command("decr %%s");
					AutoFreeReply reply = command.execute(*this->pToContext, this->key);
					switch (reply.replyType()) {
						case RedisReplyType::INTEGER:
							return tmp;
						case RedisReplyType::NIL:
							throw RedisNilException(this->key);
						default:
							throw RedisUnexpectedCaseException();
					}
				}

				template <typename AnotherType>
				IntegerReference& operator%=(const AnotherType & with)
				{
					return *this = (Type) (*this) % with;
				}
		};

		template <typename Type>
		class Reference : public
			std::conditional<CheckIsIntegerType<Type>::value, IntegerReference<Type>,
			typename std::conditional<CheckIsRealType<Type>::value, RealReference<Type>,
																ReferenceBase<Type> >::type>::type
		{
			protected:
				typedef typename std::conditional<CheckIsIntegerType<Type>::value, IntegerReference<Type>,
								   typename std::conditional<CheckIsRealType<Type>::value, RealReference<Type>,
																			ReferenceBase<Type> >::type>::type
						supper_t;
			public:
				Reference(const Context & conn, const char key[]) :
						supper_t(conn, key)
				{
				}

				Reference(const Context & conn, const std::string & key) :
						supper_t(conn, key)
				{
				}

				Reference& operator=(const Type & src)
				{
					supper_t::operator=(src);
					return *this;
				}
		};

		template <typename Type>
		class ConstReference : public ConstReferenceBase<Type>
		{
			protected:
				typedef ConstReferenceBase<Type> supper_t;

			public:
				ConstReference(const Context & conn, const char key[]) :
						supper_t(conn, key)
				{
				}

				ConstReference(const Context & conn, const std::string & key) :
						supper_t(conn, key)
				{
				}
		};
	}
}


#endif /* INCLUDE_KERBAL_REDIS_REDIS_DATA_STRUCT_REFERENCE_BASE_HPP_ */
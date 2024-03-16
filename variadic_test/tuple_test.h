#ifndef TUPLE_TEST_H
#define TUPLE_TEST_H

template<typename...Ts>
class TestTuple
{
public:
	
	template<typename T, typename...Tr>
	struct value_type
	{
		T value;

		value_type<Tr...> item;
	};

	template<typename T>
	struct value_type<T>
	{
		T value;
	};

	value_type<Ts...> item;

	template<unsigned int N, unsigned int C, typename T, typename...Tr>
	Get() -> T
	{
		using value_type<T,Tr> 

		if (N == C)
		{
			return 
		}
		else
		{
			return Get
		}
	}

	template <unsigned int N, unsigned int C, bool M, typename T, typename...Tr>
	struct get_return_type
	{
		using return_type = get_return_type<N, C + 1, N == C + 1, Tr>::return_type;
	};

	template <unsigned int N, unsigned int C, typename T, typename...Tr>
	struct get_return_type<N,C,true,T,Tr...>
	{
		using return_type = T;
	};

	template<unsigned int N>
	struct get_return_type_start
	{
		using return_type = get_return_type<N, 0, N == 0, Ts>::return_type;
	};

	template<unsigned int N>
	auto Get() -> get_return_type_start<N>::return_type
	{
		return get_value<N>();
	}
};


#endif

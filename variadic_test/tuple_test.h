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

	// Used for first item in tuple when N != 0
	template <unsigned int N, unsigned int C, bool MATCH, typename CURRENT, typename...REMAINING>
	struct get_return_type
	{
		using return_type = typename get_return_type<N, C + 1, N == C, REMAINING...>::return_type;
	};

	// Used for first item in tuple when N==C
	template <unsigned int N, unsigned int C, typename PREV, typename...REMAINING>
	struct get_return_type<N, C, true, PREV, REMAINING...>
	{
		using return_type = typename PREV;
	};
	
	// Used for last item in tuple when N==C
	template <unsigned int N, unsigned int C, typename CURRENT>
	struct get_return_type<N, C, true, CURRENT>
	{
		using return_type = typename CURRENT;
	};

	template<unsigned int N>
	struct get_return_type_start
	{
		using return_type = typename get_return_type<N, 1, N == 0, Ts...>::return_type;
	};

	template<unsigned int N>
	auto Get() -> typename get_return_type_start<N>::return_type
	{
		return N;
	}
};


#endif

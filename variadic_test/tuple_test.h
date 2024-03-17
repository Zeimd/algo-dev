#ifndef TUPLE_TEST_H
#define TUPLE_TEST_H

template<typename...Ts>
class TestTuple
{
public:
	
	template<typename CURRENT, typename...REMAINING>
	struct value_type
	{
		CURRENT value;

		using container_type = value_type;

		using next_type = value_type<REMAINING...>;

		next_type item;
	};

	template<typename CURRENT>
	struct value_type<CURRENT>
	{
		using container_type = value_type;

		CURRENT value;
	};

	using first_item_type = value_type<Ts...>;

	first_item_type item;

	// Used when N != C
	template <unsigned int N, unsigned int C, bool MATCH, typename CURRENT, typename...REMAINING>
	struct get_return_type
	{
		using return_type = typename get_return_type<N, C + 1, N == C, REMAINING...>::return_type;
	};

	// Used when N==C
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


	// Used when N 
	template <unsigned int C, typename CURRENT, typename...REMAINING>
	struct get_tuple_length
	{
		static const constexpr unsigned int length = get_tuple_length<C + 1,REMAINING...>::length;
	};

	// Used for last item in tuple when N==C
	template <unsigned int C, typename CURRENT>
	struct get_tuple_length<C, CURRENT>
	{
		static const unsigned int length = C;
	};

	static const unsigned int length_from_struct_template = get_tuple_length<1,Ts...>::length;

	static const unsigned int length = sizeof...(Ts);

	// Used when N != C
	template<unsigned int N, unsigned int C, bool MATCH, typename CURRENT, typename ... REMAINING>
	struct Getter
	{
		static const auto Get(const value_type<CURRENT, REMAINING...>& item)
		{
			return Getter<N, C + 1, N == C, REMAINING...>::Get(item.item);
		}
	};

	// Used for intermediate item in tuple when N==C
	template<unsigned int N, unsigned int C, typename CURRENT, typename ...REMAINING>
	struct Getter<N, C, true, CURRENT, REMAINING...>
	{
		static const auto Get(const value_type<CURRENT, REMAINING...>& item)
		{
			return item.value;
		}
	};

	// Used for last item in tuple when N==C
	template<unsigned int N, unsigned int C, typename CURRENT>
	struct Getter<N, C, true, CURRENT>
	{
		static const auto Get(const value_type<CURRENT>& item)
		{
			return item.value;
		}
	};

	template<unsigned int N>
	const auto Get() 
	{
		return Getter<N,1,N==0,Ts...>::Get(item);
	}

};


#endif

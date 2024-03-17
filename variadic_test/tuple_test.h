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

	
	// Used when N != C
	template<typename SOURCE_TYPE, unsigned int N, unsigned int C, bool MATCH, typename CURRENT, typename ... REMAINING>
	struct Setter
	{
		static void Set(value_type<CURRENT, REMAINING...>& item, const SOURCE_TYPE& source)
		{
			Setter<SOURCE_TYPE, N, C + 1, N == C, REMAINING...>::Set(item.item, source);
		}
	};

	// Used for intermediate item in tuple when N==C
	template<typename SOURCE_TYPE, unsigned int N, unsigned int C, typename CURRENT, typename ...REMAINING>
	struct Setter<SOURCE_TYPE, N, C, true, CURRENT, REMAINING...>
	{
		static void Set(value_type<CURRENT, REMAINING...>& item, const SOURCE_TYPE& source)
		{
			item.value = source;
		}
	};

	// Used for last item in tuple when N==C
	template<typename SOURCE_TYPE, unsigned int N, unsigned int C, typename CURRENT>
	struct Setter<SOURCE_TYPE, N, C, true, CURRENT>
	{
		static void Set(value_type<CURRENT>& item, const SOURCE_TYPE& source)
		{
			item.value = source;
		}
	};

	template<unsigned int N, typename SOURCE_TYPE>
	void Set(const SOURCE_TYPE& source)
	{
		Setter<SOURCE_TYPE,N, 1, N == 0, Ts...>::Set(item,source);
	}

	// Used for intermediate items in tuple
	template<typename CURRENT, typename...REMAINING>
	struct Constructor
	{
		static void Construct(value_type<CURRENT,REMAINING...>& item, CURRENT source, REMAINING...args)
		{
			item.value = source;

			Constructor<REMAINING...>::Construct(item.item, args...);
		}

		static void DefaultConstruct(value_type<CURRENT, REMAINING...>& item)
		{
			item.value = CURRENT();

			Constructor<REMAINING...>::DefaultConstruct(item.item);
		}
	};

	// Used for last item in tuple
	template<typename CURRENT>
	struct Constructor<CURRENT>
	{
		static void Construct(value_type<CURRENT>& item, CURRENT source)
		{
			item.value = source;
		}

		static void DefaultConstruct(value_type<CURRENT>& item)
		{
			item.value = CURRENT();
		}
	};

	TestTuple(Ts...args)
	{
		Constructor<Ts...>::Construct(item, args...);
	}

	TestTuple()
	{
		Constructor<Ts...>::DefaultConstruct(item);
	}
	
};


#endif

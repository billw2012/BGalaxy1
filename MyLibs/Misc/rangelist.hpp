#ifndef _MISC_RANGELIST_HPP
#define _MISC_RANGELIST_HPP

#include <set>

namespace misc
{

template < class RangeValueType, class IteratorType >
struct RangeIterator
{
	typedef std::pair< RangeValueType, RangeValueType > value_type;
	typedef IteratorType iterator_type;
	//typedef RangeListType range_list_type;

private:
	typedef RangeIterator< RangeValueType, IteratorType > this_type;
	//RangeListType& ranges;
	IteratorType itr;
	IteratorType nextitr;
	IteratorType endMarker;
	value_type currval;

public:
	RangeIterator(const this_type& other) 
		: endMarker(other.endMarker),
		itr(other.itr),
		nextitr(other.nextitr),
		currval(other.currval) {}
	RangeIterator(IteratorType endMarker_, IteratorType first_, IteratorType last_)
		: endMarker(endMarker_), itr(first_), nextitr(last_), currval() 
	{
		if(itr != endMarker && nextitr != endMarker)
			currval = value_type(itr->index, nextitr->index);
	}

	this_type& operator++() // prefix
	{
		++itr; ++itr;
		if(itr != endMarker) 
		{
			++nextitr; 
			++nextitr;
			currval = value_type(itr->index, nextitr->index);
		}
		else
		{
			// forward the 
			++nextitr;
		}
		return *this;
	}

	this_type& operator--() // prefix
	{
		--itr; --itr;
		--nextitr; --nextitr;
		currval = value_type(itr->index, nextitr->index);
		return *this;
	}

	this_type operator++(int) // postfix
	{
		RangeIterator ret = *this;
		++(*this);
		return ret;
	}

	this_type operator--(int) // postfix
	{
		RangeIterator ret = *this;
		--(*this);
		return ret;
	}

	const value_type* operator->() const
	{
		return &currval;
	}

	value_type* operator->()
	{
		return &currval;
	}

	const value_type& operator*() const
	{
		return currval;
	}

	value_type& operator*()
	{
		return currval;
	}

	bool operator!=(const this_type& other) const
	{
		return endMarker != other.endMarker || itr != other.itr || nextitr != other.nextitr;
	}
};

template < class ValueType >
struct RangeList
{
	typedef ValueType value_type;

	struct RangeRec
	{
		value_type index;
		bool start : 1;

		RangeRec(value_type index_, bool start_) : index(index_), start(start_) {}

		bool operator<(const RangeRec& u) const
		{
			return index < u.index;
		}
	};

	typedef std::set< RangeRec > RangeMap;

private:
	typedef RangeList<value_type> this_type;

public:
	typedef RangeIterator< value_type, typename RangeMap::iterator > iterator;
	typedef RangeIterator< value_type, typename RangeMap::const_iterator > const_iterator;


private:
	RangeMap _ranges;

public:
	void clear() 
	{
		_ranges.clear();
	}

	void add(value_type start, value_type end)
	{
		if(end <= start)
			return;
		// dirty list represents ranges with alternate elements: start,end,start,end 
		// to insert a new range:
		// find insert point of start. find element before, if it is a start element then it is the start of the new range
		// otherwise the new insertion is
		// find insert point of end. find element after, if it is an end element then it is the end of the new range
		// otherwise the new insertion is
		std::pair<RangeMap::iterator, bool> sloc = _ranges.insert(RangeRec(start, true));

		if(sloc.first != _ranges.begin())
		{
			RangeMap::iterator prev = sloc.first; prev--;
			if(prev->start)
			{
				sloc.first--;
			}
		}
		std::pair<RangeMap::iterator, bool> eloc = _ranges.insert(RangeRec(end, false));
		RangeMap::iterator next = eloc.first; next++;

		if(next != _ranges.end() && !next->start)
		{
			eloc.first++;
		}

		// erase the interior of the range
		++sloc.first;
		if(sloc.first != eloc.first && sloc.first != _ranges.end())
		{
			//eloc.first;
			if(sloc.first != eloc.first)
				_ranges.erase(sloc.first, eloc.first);
			else
				_ranges.erase(sloc.first);
		}
		assert((_ranges.size() - 1) % 2 != 0);
	}


	iterator begin()
	{
		iterator::iterator_type start = _ranges.begin(), end = _ranges.begin();
		if(end != _ranges.end())
			++end;
		return iterator(_ranges.end(), start, end);
	}

	const_iterator begin() const
	{
		const_iterator::iterator_type start = _ranges.begin(), end = _ranges.begin();
		if(end != _ranges.end())
			++end;
		return const_iterator(_ranges.end(), start, end);
	}

	bool empty() const
	{
		return _ranges.size() == 0;
	}

	iterator end()
	{
		return iterator(_ranges.end(), _ranges.end(), _ranges.end());
	}

	const_iterator end() const
	{
		return const_iterator(_ranges.end(), _ranges.end(), _ranges.end());
	}
};

}

#endif // _MISC_RANGELIST_HPP
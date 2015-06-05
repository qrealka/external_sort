#ifndef EXTERNAL_SORT_RANGE_H
#define EXTERNAL_SORT_RANGE_H

#include <iterator>
#include <algorithm>
#include <vector>
#include <assert.h>


namespace external_sort
{

template<typename T>
class Range
{
public:
    typedef Range<T> type;
    typedef typename std::iterator_traits<T>::value_type value_type;
    typedef typename std::iterator_traits<T>::difference_type difference_type;

    typedef std::size_t size_type;
    typedef typename std::iterator_traits<T>::reference reference;

    typedef T const_iterator;
    typedef T iterator;

public:
    Range() : m_begin( iterator() ), m_end( iterator() )
    { }

    template<class Iterator>
    Range(Iterator begin, Iterator end) : m_begin(begin), m_end(end)
    {}

    template<class Iterator>
    Range(const Range<Iterator> & r): m_begin(r.begin()), m_end(r.end())
    {
    }

    type& operator = (const type& r)
    {
        m_begin  = r.begin();
        m_end    = r.end();
        return *this;
    }

    template<class Iterator>
    type& operator = (const Range<Iterator>& r)
    {
        m_begin  = r.begin();
        m_end    = r.end();
        return *this;
    }

    iterator begin() const
    {
        return m_begin;
    }

    iterator end() const
    {
        return m_end;
    }

    size_type size() const
    {
        return m_end - m_begin;
    }

    bool empty() const
    {
        return m_begin == m_end;
    }

    bool operator < ( const Range& r ) const
    {
        return std::lexicographical_compare(m_begin, m_end, r.m_begin, r.m_end );
    }

    reference front() const
    {
        assert(!empty());
        return *m_begin;
    }

    reference back() const
    {
        assert(!empty());
        iterator last(m_end);
        return *(--last);
    }

    reference operator[](difference_type at) const
    {
        assert((at >= 0) && (at < (m_end - m_begin)));
        return m_begin[at];
    }

private:
    T m_begin;
    T m_end;
};

template<class Iterator1T, class Iterator2T >
inline bool operator < (const Range<Iterator1T>& l, const Range<Iterator2T>& r)
{
    return std::lexicographical_compare(std::begin(l), std::end(l), std::begin(r), std::end(r));
}

typedef std::vector<char> CharBuffer;
typedef Range<const char*> RangeConstChar;
typedef std::vector<RangeConstChar> RangeLines;
}

#endif //EXTERNAL_SORT_RANGE_H

#pragma once
#ifndef ZGRAPH_DETAIL_ZVALUE_MAP
#define ZGRAPH_DETAIL_ZVALUE_MAP
 #define BOOST_RESULT_OF_USE_DECLTYPE
#include <boost/iterator/transform_iterator.hpp>
#include <queue>
#include <iterator>

namespace zgraph::detail
{




    template <typename Iter>
    class zmap_iterator : public std::iterator<std::bidirectional_iterator_tag, typename Iter::value_type::second_type> {
    public:
        using base_t = std::iterator<std::bidirectional_iterator_tag,typename Iter::value_type::second_type>;
        using reference =  typename base_t::reference;
        using pointer =  typename base_t::pointer;
        using value_type =  typename Iter::value_type::second_type;
        zmap_iterator() {}
        zmap_iterator(Iter j) : i(j) {}
        zmap_iterator& operator++() { ++i; return *this; }
        zmap_iterator operator++(int) { auto tmp = *this; ++(*this); return tmp; }
        zmap_iterator& operator--() { --i; return *this; }
        zmap_iterator operator--(int) { auto tmp = *this; --(*this); return tmp; }
        bool operator==(zmap_iterator j) const { return i == j.i; }
        bool operator!=(zmap_iterator j) const { return !(*this == j); }
        reference operator*() { return i->second; }
        pointer operator->() { return &i->second; }
    protected:
        Iter i;
    };

    template <typename Iter>
    inline zmap_iterator<Iter> make_zmap_iterator(Iter j) { return zmap_iterator<Iter>(j); }


    template<typename T1, typename T2>  const T2 & take_second_c(const std::pair<T1, T2> &a_pair) 
    {
      return a_pair.second;
    }
    template<typename T1, typename T2>  T2 & take_second(const std::pair<T1, T2> &a_pair) 
    {
      return const_cast<T2 &>(a_pair.second);
    }

    /// Used to iterate over the values of a map
    /// https://gist.github.com/eruffaldi/93d09ed6644ae3fa279f
    template <class map>
    struct zvalue_map
    {
        using map_t = map;
        using key_type = typename map_t::key_type;
        using value_type = typename map_t::mapped_type;
   
        zvalue_map() : x_() {}

        auto begin() const{return boost::make_transform_iterator(x_.begin(), take_second_c<key_type, value_type>);}
        auto end()   const{return boost::make_transform_iterator(x_.end(), take_second_c<key_type, value_type>);}
        auto begin()      {return boost::make_transform_iterator(x_.begin(), take_second<key_type, value_type>);}
        auto end()        {return boost::make_transform_iterator(x_.end(), take_second<key_type, value_type>);}

        

        auto && operator[](const key_type key) const{
            return x_[key];
        }
        auto && operator[](const key_type key){
            return x_[key];
        }

        map_t  x_;
    };

} // end namespace zgraph


#endif // ZGRAPH_DETAIL_ZVALUE_MAP
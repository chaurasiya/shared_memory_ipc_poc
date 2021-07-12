#include <functional>
#include <cstdio>
#include <utility>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>

namespace emulation {
    template <typename T1,typename T2,typename Alloc>
        struct mutable_pair
        {
            typedef T1 first_type;
            typedef T2 second_type;

            mutable_pair(Alloc alloc):first(T1(alloc)),second(T2(alloc)){}
            mutable_pair(const T1& f,const T2& s):first(f),second(s){}
            mutable_pair(const std::pair<T1,T2>& p):first(p.first),second(p.second){}

            T1         first;
            mutable T2 second;
        };

    using namespace boost::multi_index;

    template <typename Key, typename T, typename Compare, typename Allocator, typename Element = mutable_pair<Key, T, Allocator> >
        using map = multi_index_container<
            Element,
            indexed_by<
                ordered_unique<member<Element,Key,&Element::first>,Compare>
            >,
            typename Allocator::template rebind<Element>::other
        >;

  template <typename Key, typename T, typename Compare, typename Allocator, typename Element = mutable_pair<Key, T, Allocator> >
    using multimap = multi_index_container<
        Element,
        indexed_by<
            ordered_non_unique<member<Element,Key,&Element::first>,Compare>
        >,
        typename Allocator::template rebind<Element>::other
    >;

  template <typename Key, typename T, typename Compare, typename Allocator> 
      struct wrap_map : map<Key, T, Compare, Allocator> {
          typedef map<Key, T, Compare, Allocator> base_type;
          typedef typename base_type::template nth_index<0>::type index_type;

          wrap_map(Allocator alloc) : base_type({}, alloc)
          {
          }

          wrap_map(Compare cmp, Allocator alloc) : base_type(
                  typename base_type::ctor_args_list{
                    typename index_type::ctor_args { typename index_type::key_from_value {}, cmp }
                  },
                  alloc)
          {
          }
      };
}

// Typedefs of allocators and containers
namespace Shared {
    typedef boost::interprocess::managed_shared_memory Segment;
    typedef boost::interprocess::managed_shared_memory::segment_manager SegmentManager;
    typedef boost::interprocess::allocator<void, SegmentManager> Allocator;
    typedef boost::interprocess::allocator<char, SegmentManager> CharAllocator;
    typedef boost::interprocess::basic_string<char, std::char_traits<char>, CharAllocator> String;

    struct MyLess {
        template <typename T, typename U> bool operator()(const T &t, const U &u) const { return t < u; }
    };
    typedef MyLess StringComparator;


    typedef boost::interprocess::allocator<char, SegmentManager> StringAlloc;
    typedef emulation::mutable_pair<const String, String, StringAlloc> MapItem;
    typedef boost::interprocess::allocator<MapItem, SegmentManager> MapItemAllocator;
    typedef emulation::wrap_map<String, String, StringComparator, MapItemAllocator> Map;
}
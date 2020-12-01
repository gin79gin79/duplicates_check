#ifndef __COMMON_DEFINITIONS_H__
#define __COMMON_DEFINITIONS_H__

#include <experimental/set>
#include <experimental/map>
#include <experimental/vector>
#include <experimental/utility>
#include <experimental/filesystem>
#include <experimental/functional>
#include <locale>

//Storage types
typedef std::experimental::filesystem::path Path;
typedef std::set<Path> PathsSet;

typedef std::map<size_t, PathsSet> PathsGroupedBySize;

typedef std::vector<uint8_t> HashDiggest;

typedef std::map< std::pair<size_t, HashDiggest>, PathsSet> PathsGroupedBySizeAndHash;

//Tools
namespace utils {

template< typename ContainerT, typename PredicateT >
void erase_if( ContainerT& items, const PredicateT& predicate ) {
    for( auto it = std::begin(items); it != std::end(items); ) {
        if( predicate(*it) )
            it = items.erase(it);
        else
            ++it;
    }
}

template <class CharT, typename... Rest>
auto to_upper(const std::basic_string<CharT, Rest...>& from) {
    static auto toupper = std::bind( std::toupper<CharT>, std::placeholders::_1 , std::locale() );
    std::basic_string<CharT, Rest...> to;
    std::transform(std::begin(from), std::end(from), std::back_inserter(to), toupper);
    return to;
}

}//namespace utils

#endif//__COMMON_DEFINITIONS_H__

/*
vi:set tabstop=8 softtabstop=4 shiftwidth=4 noexpandtab nowrap:
*/

#include <experimental/vector>
#include <experimental/iterator>
#include <experimental/filesystem>
#include <iostream>
#include <boost/program_options.hpp>

#include "common_definitions.h"
#include "scan_folders.h"
#include "file_hash.h"

using std::cerr, std::cout, std::endl, std::begin, std::cbegin, std::end, std::cend, std::string;
namespace fs = std::experimental::filesystem;
namespace po = boost::program_options;

//Main
int main(int argc, char** argv) {

    //Declare and parse input parameters
    size_t deep = 0;
    bool follow_symlink = false;
    std::vector<string> input_folders;
    string hash_name;

    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "produce this message")
        ("deep,d", po::value<size_t>(&deep)->default_value(0), "set recursion deep")
        ("symlinks,s", "follow symlinks if set")
        ("hash,H", po::value<string>(&hash_name)->default_value("SHA256"), "set hash algorihtm (CRC32, CRC32C, MD5, SHA1, SHA256, SHA224, SHA512, SHA384)")
        ("input-folders,I", po::value< std::vector<string> >(&input_folders), "list of input folders")
    ;

    po::positional_options_description p;
    p.add("input-folders", -1);

    po::variables_map vm;
    try {
        po::store(po::command_line_parser(argc, argv)
                .options(desc)
                .positional(p)
                .run(), vm);
    }
    catch(po::error& e) {
        cerr << e.what() << endl;
        cerr << desc << endl;
        return 1;
    }
    po::notify(vm);

    //Check input parameters
    if(vm.count("help")) {
        cerr << desc << endl;
        return 1;
    }

    if(vm.count("symlinks"))
        follow_symlink = true;

    if(not input_folders.size())
    {
        cerr << "No input folders" << endl;
        cerr << desc << endl;
        return 1;
    }

    HashFunctionPtr hash_ptr = createHashFunction(hash_name);
    if(not hash_ptr)
    {
        cerr << "Wrong hash name: " << hash_name << endl;
        cerr << desc << endl;
        return 1;
    }

    //Scan folders to collect the files grouped by size
    PathsGroupedBySize same_size_files;
    for(auto& path: PathsSet(begin(input_folders), end(input_folders)))
        scan_folder(path, same_size_files, deep+1, follow_symlink);

    //Erase groups with single file path
    utils::erase_if( same_size_files, [](const auto& item) {return item.second.size() < 2;} );

    //Collect hash for files and do the grouping by size+hash
    PathsGroupedBySizeAndHash same_size_and_hash_files;
    for(auto& [size, paths]: same_size_files)
        for(auto& path: paths) {
            HashDiggest digest = calculateFileHash(path, hash_ptr);
            if( not digest.empty() )
                same_size_and_hash_files[ std::pair( size, digest ) ].insert( path );
        }
    
    //Erase groups with single file path
    utils::erase_if( same_size_and_hash_files, [](const auto& item) {return item.second.size() < 2;} );

    //Print result
    for(auto& [key, value]: same_size_and_hash_files) {
        cout << "Size " << key.first << " " << hash_ptr->AlgorithmName() << " " << hashToString(key.second) << endl;
        for(auto& path: value)
            cout << path.native() << endl;
    }

    return 0;
}

/*
vi:set tabstop=8 softtabstop=4 shiftwidth=4 noexpandtab nowrap:
*/

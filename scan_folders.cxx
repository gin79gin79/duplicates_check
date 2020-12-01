#include <experimental/map>
#include <experimental/iterator>
#include <experimental/algorithm>
#include <experimental/system_error>
#include <iostream>

#include "scan_folders.h"

using std::cerr, std::cout, std::endl, std::begin, std::cbegin, std::end, std::cend, std::string;
namespace fs = std::experimental::filesystem;

void scan_folder(const Path& path, PathsGroupedBySize& result, const size_t deep, const bool follow_symlink) noexcept {
    std::error_code error;
    fs::file_status file_status = fs::symlink_status(path, error);
    if(error) {
        cerr << error.message() << endl;
        return;
    }

    if( follow_symlink and fs::is_symlink(file_status) ) {
        Path spath = fs::read_symlink(path, error);
        if(error)
            cerr << error.message() << endl;
        else
            scan_folder(spath, result, deep, follow_symlink);
    }
    else if( fs::is_regular_file(file_status) ) {
        size_t file_size = fs::file_size(path, error);
        if(error)
            cerr << error.message() << endl;
        else if(file_size)
            result[file_size].insert(path);
    }
    else if( deep and fs::is_directory(file_status) ) {
        fs::directory_iterator dir_it(path, fs::directory_options::skip_permission_denied, error);
        if(error)
            cerr << error.message() << endl;
        else
            for(auto& dir_entry: dir_it)
                scan_folder(dir_entry, result, deep-1, follow_symlink);
    }
}

/*
vi:set tabstop=8 softtabstop=4 shiftwidth=4 noexpandtab nowrap:
*/

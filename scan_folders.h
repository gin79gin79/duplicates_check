#ifndef __SCAN_FOLDERS_H__
#define __SCAN_FOLDERS_H__
#include "common_definitions.h"

void scan_folder(const Path& path, PathsGroupedBySize& result, const size_t deep, const bool follow_symlink) noexcept;

#endif//__SCAN_FOLDERS_H__

/*
vi:set tabstop=8 softtabstop=4 shiftwidth=4 noexpandtab nowrap:
*/

#include <filesystem>
#include "foldertools.hpp"
#include <iostream>

bool moveToFolder(std::string folderName, std::filesystem::path &currentPath)
{
    if (folderName == "..")
    {
        // special case - go up one dir
        currentPath = currentPath.parent_path();
        // this should always work
        return true;
    }

    // test if currentPath is a top-level path
    std::filesystem::path newPath(folderName);
    if (std::filesystem::exists(newPath))
    {
        // it's a top-level path
        currentPath = newPath;
        return true;
    }

    // test if it's a subfolder
    // thanks to the horrible abuse of operator/ going on in <filesystem>,
    // this somehow works
    newPath = currentPath / folderName;
    if (std::filesystem::exists(newPath))
    {
        // success
        currentPath = newPath;
        return true;
    }
    return false;
}
/*
    Initial author: Convery (tcn@ayria.se)
    Started: 08-01-2018
    License: MIT
    Notes:
        Plugins should keep all their files in their archive.
*/

#pragma once
#include "../Stdinclude.hpp"

namespace Package
{
    // Perform file IO.
    void Deletefile(std::string_view Filename);
    std::string Readfile(std::string_view Filename);
    void Writefile(std::string_view Filename, std::string Filebuffer);

    // Get file-information.
    std::vector<std::string> Findfiles(std::string_view Criteria);
    uint64_t Filetimestamp(std::string_view Filename);
    bool Fileexists(std::string_view Filename);
    size_t Filesize(std::string_view Filename);
}

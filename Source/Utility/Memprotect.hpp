/*
    Initial author: Convery (tcn@ayria.se)
    Started: 08-01-2018
    License: MIT
    Notes:
        Changes the page-permissions for a range.
*/

#pragma once
#include "../Stdinclude.hpp"

namespace Memprotect
{
    #if defined(_WIN32)

    // Windows memory protection,
    inline void Protectrange(void *Address, const size_t Length, unsigned long Oldprotection)
    {
        unsigned long Temp;
        VirtualProtect(Address, Length, Oldprotection, &Temp);
    }
    inline unsigned long Unprotectrange(void *Address, const size_t Length)
    {
        unsigned long Oldprotection;
        VirtualProtect(Address, Length, PAGE_EXECUTE_READWRITE, &Oldprotection);
        return Oldprotection;
    }

    #else

    // Nix memory protection.
    inline void Protectrange(void *Address, const size_t Length, unsigned long Oldprotection)
    {
        int Pagesize = getpagesize();
        *(size_t *)&Address -= size_t(Address) % Pagesize;
        mprotect(Address, Length + ((Length % Pagesize) ? (Pagesize - (Length % Pagesize)) : 0), Oldprotection);
    }
    inline unsigned long Unprotectrange(void *Address, const size_t Length)
    {
        unsigned long Oldprotection = (PROT_READ | PROT_WRITE | PROT_EXEC);

        // Get the old protection of the range, we assume it's continuous.
        std::FILE *Filehandle = std::fopen("/proc/self/maps", "r");
        if(Filehandle)
        {
            char Buffer[1024]{}, Permissions[5]{}, Device[6]{}, Mapname[256]{};
            unsigned long Start, End, Node, Foo;

            while(std::fgets(Buffer, 1024, Filehandle))
            {
                std::sscanf(Buffer, "%lx-%lx %4s %lx %5s %lu %s", &Start, &End, Permissions, &Foo, Device, &Node, Mapname);

                if(Start <= (unsigned long)Address || End >= (unsigned long)Address)
                {
                    Oldprotection = 0;

                    if(Permissions[0] == 'r') Oldprotection |= PROT_READ;
                    if(Permissions[1] == 'w') Oldprotection |= PROT_WRITE;
                    if(Permissions[2] == 'x') Oldprotection |= PROT_EXEC;

                    break;
                }
            }

            std::fclose(Filehandle);
        }

        // Write the new protection.
        int Pagesize = getpagesize();
        *(size_t *)&Address -= size_t(Address) % Pagesize;
        mprotect(Address, Length + ((Length % Pagesize) ? (Pagesize - (Length % Pagesize)) : 0), PROT_READ | PROT_WRITE | PROT_EXEC);
        return Oldprotection;
    }

    #endif

    // Helpers to provide more readable code.
    inline void Protectrange(std::uintptr_t Address, const size_t Length, unsigned long Oldprotection)
    {
        return Protectrange(reinterpret_cast<void *>(Address), Length, Oldprotection);
    }
    inline unsigned long Unprotectrange(std::uintptr_t Address, const size_t Length)
    {
        return Unprotectrange(reinterpret_cast<void *>(Address), Length);
    }
}

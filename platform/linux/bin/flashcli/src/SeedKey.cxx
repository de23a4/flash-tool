/*
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 */

#include "SeedKey.hxx"

#include <uds-secacc/uds-secacc.hxx>

bool calculateSecurityPassword(const std::vector<std::uint8_t>& seed, const int securityLevel, std::vector<std::uint8_t>& key)
{
    key.resize(256);
    unsigned int actualKeyArraySize;
    int          rv = GenerateKeyEx(seed.data(), seed.size(), securityLevel, "", key.data(), key.size(), actualKeyArraySize);
    if (rv != KGRE_Ok)
    {
        key.resize(0);
        return false;
    }
    else
    {
        key.resize(actualKeyArraySize);
    }

    return true;
}
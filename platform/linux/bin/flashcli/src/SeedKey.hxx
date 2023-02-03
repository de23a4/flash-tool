/*
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 *
 * Description:
 * TODO: add description
 */

#ifndef FLASH_TOOL_SEEDKEY_HXX
#define FLASH_TOOL_SEEDKEY_HXX

#include <cstdint>
#include <vector>

bool calculateSecurityPassword(const std::vector<std::uint8_t>& seed, const int securityLevel, std::vector<std::uint8_t>& key);

#endif // FLASH_TOOL_SEEDKEY_HXX

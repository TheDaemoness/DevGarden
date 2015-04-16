#ifndef CONFIGLOADER_H
#define CONFIGLOADER_H

#include "envmacros.h"

#if defined(DG_ENV_MACOS)
#define DG_CONFIG_PREFIX_GLOBAL "/Library/Application Support/"
#define DG_CONFIG_PREFIX_LOCAL  "~/Library/Application Support/"
#elif defined(DG_ENV_WINDOWS)
#define DG_CONFIG_PREFIX_GLOBAL "C:/ProgramData/"
#define DG_CONFIG_PREFIX_LOCAL  "~/AppData/Roaming/"
#elif defined(DG_ENV_UNIX)
#define DG_CONFIG_PREFIX_GLOBAL "/usr/share/"
#define DG_CONFIG_PREFIX_LOCAL  "~/."
#endif

#include <fstream>
#include <string>
#include <utility>

std::ifstream* getConfigFile(const std::string& name); //TODO: Qt-Specific file stuff.
std::pair<std::string,std::string> getEntryPair(const std::ifstream& stream);

#endif // CONFIGLOADER_H

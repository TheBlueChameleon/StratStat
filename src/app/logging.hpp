#ifndef LOGGING_HPP
#define LOGGING_HPP

#include "config.hpp"

constexpr auto DEFALUT_LOGGER_NAME = "DefaultLogger";

void configureLogger(const Config& cfg);
void logCfg(const Config& cfg);

#endif // LOGGING_HPP

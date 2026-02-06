#pragma once

#include <memory>

class Config;

/**
 * Entry point for oneshot mode
 */
int oneshot_init(std::shared_ptr<Config> config);

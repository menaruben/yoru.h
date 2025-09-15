#pragma once

#define func(__return_type, __name, ...) __return_type (*__name)(__VA_ARGS__)

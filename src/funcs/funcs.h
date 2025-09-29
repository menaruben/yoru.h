#pragma once

#define YORU_FUNC(__return_type, __name, ...) __return_type (*__name)(__VA_ARGS__)

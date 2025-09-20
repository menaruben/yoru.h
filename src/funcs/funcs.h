#pragma once

#define func(__return_type, __name, ...) __return_type (*__name)(__VA_ARGS__)

#define interface(__name, ...) typedef struct ns(__name) { __VA_ARGS__ } ns(__name)

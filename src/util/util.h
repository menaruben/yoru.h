#pragma once

#include "panic.h"

#define TODO(msg) panicf("%s: not implemented yet", msg)

#define nameof(x) #x
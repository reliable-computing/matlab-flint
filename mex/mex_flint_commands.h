/**
 * Copyright (C) 2026 Kai T. Ohlhus <kai.ohlhus@gmail.com>
 *
 * This file is part of MATLAB-FLINT.
 *
 * MATLAB-FLINT is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License (LGPL) as published
 * by the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.  See <https://www.gnu.org/licenses/>.
 */

#ifndef MEX_FLINT_COMMANDS_H_
#define MEX_FLINT_COMMANDS_H_

#include <stdint.h>

typedef uint64_t CmdCode;

// ============================================================================
// class AP auxillary functions
// ============================================================================

const CmdCode AP_CLEAR = 0;

const CmdCode AP_SET_VERBOSE = 10;
const CmdCode AP_GET_VERBOSE = 20;

const CmdCode AP_GET_GMP_VERSION = 100;
const CmdCode AP_GET_MPFR_VERSION = 110;
const CmdCode AP_GET_FLINT_VERSION = 120;


const char* get_cmd_string(CmdCode cmd_code) {
    switch (cmd_code) {
        case AP_CLEAR: return "AP_CLEAR";

        case AP_SET_VERBOSE: return "AP_SET_VERBOSE";
        case AP_GET_VERBOSE: return "AP_GET_VERBOSE";

        case AP_GET_GMP_VERSION:return "AP_GET_GMP_VERSION";
        case AP_GET_MPFR_VERSION:return "AP_GET_MPFR_VERSION";
        case AP_GET_FLINT_VERSION:return "AP_GET_FLINT_VERSION";

        default: return "UNKNOWN_COMMAND";
    }
}

#endif  // MEX_FLINT_COMMANDS_H_

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

#ifndef MEX_ARB_T_H_
#define MEX_ARB_T_H_

#include "flint/arb.h"

#include "mex_flint_interface.h"

/**
 * MEX interface for arb_t data type.
 *
 * @param nlhs MEX parameter.
 * @param plhs MEX parameter.
 * @param nrhs MEX parameter.
 * @param prhs MEX parameter.
 * @param cmd_code code of command to execute (1000 - 1999).
 */
void
mex_arb_t (int nlhs, mxArray *plhs[],
           int nrhs, const mxArray *prhs[],
           uint64_t cmd_code);


/**
 * Function called at exit of the mex file to tidy up all memory.
 * After calling this function the initial state is restored.
 */
void
arb_tidy_up (void);


/**
 * Check for valid index range.
 *
 * @param[in] idx Pointer to index (1-based, idx_t) of ARB variables.
 *
 * @returns `0` if `idx` is invalid, otherwise `idx` is valid.
 */
int
is_valid (idx_t *idx);


/**
 * Safely read ARB index (idx_t) structure.
 *
 * @param[in] idx MEX input position index (0 is first).
 * @param[in] nrhs Number of right-hand sides.
 * @param[in] mxArray  MEX input array.
 * @param[out] idx_vec If function returns `1`, `idx_vec` contains a valid
 *                     index (idx_t) structure extracted from the MEX input,
 *                     otherwise `idx_vec` remains unchanged.
 *
 * @returns success of extraction.
 */
int
extract_idx (int idx, int nrhs, const mxArray *prhs[], idx_t *idx_vec);



/**
 * Safely read scalar slong (aka "long int") from MEX input.
 *
 * @param[in] idx MEX input position index (0 is first).
 * @param[in] nrhs Number of right-hand sides.
 * @param[in] mxArray  MEX input array.
 * @param[out] slong If function returns `1`, `si` contains the scalar signed
 *                   integer extracted from the MEX input, otherwise `si` remains
 *                   unchanged.
 *
 * @returns success of extraction.
 */
int
extract_slong (int idx, int nrhs, const mxArray *prhs[], int64_t *si);


/**
 * Safely declare and read ARB_T variable(-arrays) from MEX interface.
 *
 * @param mex_rhs Position (0-based) in MEX input.
 * @param name    Desired variable name.
 */
#define MEX_ARB_T(mex_rhs, name)                                     \
  idx_t name;                                                        \
  if (! extract_idx ((mex_rhs), nrhs, prhs, &name))                  \
    MEX_FCN_ERR ("cmd[%d]:"#name " Invalid ARB variable indices.\n", \
                 cmd_code);


/**
 * Safely declare and read SLONG variable from MEX interface.
 *
 * @param mex_rhs Position (0-based) in MEX input.
 * @param name    Desired variable name.
 */
#define MEX_SLONG(mex_rhs, name)                                       \
  slong name = 0;                                                      \
  if (! extract_slong ((mex_rhs), nrhs, prhs, &name))                  \
    MEX_FCN_ERR ("cmd[%d]:"#name " must be a signed integer.\n", cmd_code);


#endif  // MEX_ARB_T_H_

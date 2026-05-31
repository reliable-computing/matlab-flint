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

#ifndef MEX_AXB_MAT_H_
#define MEX_AXB_MAT_H_

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
mex_axb_mat (int nlhs, mxArray *plhs[],
             int nrhs, const mxArray *prhs[],
             uint64_t cmd_code);


/**
 * Function called at exit of the mex file to tidy up all memory.
 * After calling this function the initial state is restored.
 */
void
mex_axb_mat_tidy_up (void);


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
 * Safely declare and read ARB_MAT from MEX interface.
 *
 * @param mex_rhs Position (0-based) in MEX input.
 * @param name    Desired variable name.
 */
#define MEX_AXB_MAT(mex_rhs, name)                                   \
  axb_mat name;                                                      \
  if (! extract_ui ((mex_rhs), nrhs, prhs, &name))                   \
    MEX_FCN_ERR ("cmd[%d]:"#name " Invalid AXB_MAT variable indices.\n", cmd_code);


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


#endif  // MEX_AXB_MAT_H_

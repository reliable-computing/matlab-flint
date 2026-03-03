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

#include "mex_flint_interface.h"
#include "mex_arb_t.h"


// State deciding about the output verbosity level
// - level = 0: no output at all (including no error messages)
// - level = 1: show error messages
// - level = 2: show error messages and precision warnings [default]
// - level = 3: very verbose debug output.

int VERBOSE = 2;


/**
 * MEX interface for MATLAB-FLINT.
 */
void
mexFunction (int nlhs, mxArray *plhs[],
             int nrhs, const mxArray *prhs[])
{
  // Avoid automatic clearing of memory on exit of the MEX file.
  static int locked = 0;
  if (locked == 0) {
    DBG_PRINTF ("%s\n", "Lock MEX file.");
    mexLock();
    locked = 1;
  }

  // Read command code.
  uint64_t cmd_code = 0;

  if (! extract_ui (0, nrhs, prhs, &cmd_code))
    MEX_FCN_ERR ("%s\n", "First argument must be a command code (non-negative integer).");

  DBG_PRINTF ("Command: code = %d, nlhs = %d, nrhs = %d\n", (int) cmd_code, nlhs, nrhs);

  /**
   * Branch to specialized interfaces.
   */
  if ((1000 <= cmd_code) && (cmd_code < 2000))
    mex_arb_t (nlhs, plhs, nrhs, prhs, cmd_code);

  else if (cmd_code == 0)  // void flint.clear (void)
    {
      MEX_NARGINCHK (1);
      mexUnlock();
      locked = 0;
      arb_tidy_up();
    }
  
  else if (cmd_code == 10)  // void flint.set_verbose (int level)
    {
      MEX_NARGINCHK (2);
      int64_t level = 1;
      if (extract_si (1, nrhs, prhs, &level) && (0 <= level) && (level <= 3))
        VERBOSE = (int) level;
      else
        MEX_FCN_ERR ("cmd[%s]: VERBOSE must be 0, 1, 2, or 3.\n", "flint.set_verbose");
    }

  else if (cmd_code == 11)  // int flint.get_verbose (void)
    {
      MEX_NARGINCHK (1);
      plhs[0] = mxCreateDoubleScalar ((double) VERBOSE);
    }

  else if (cmd_code == 20)  // int flint.get_gmp_version (void)
    {
      MEX_NARGINCHK (1);
      char *output_buf = (char *) mxCalloc (strlen (gmp_version), sizeof(char));
      strcpy (output_buf, gmp_version);
      plhs[0] = mxCreateString (output_buf);
      return;
    }
  
  else if (cmd_code == 21)  // int flint.get_mpfr_version (void)
    {
      MEX_NARGINCHK (1);
      char *output_buf = (char *) mxCalloc (strlen (mpfr_get_version ()) + 1, sizeof(char));
      strcpy (output_buf, mpfr_get_version ());
      plhs[0] = mxCreateString (output_buf);
      return;
    }

  else if (cmd_code == 22)  // int flint.get_flint_version (void)
    {
      MEX_NARGINCHK (1);
      char *output_buf = (char *) mxCalloc (strlen (FLINT_VERSION), sizeof(char));
      strcpy (output_buf, FLINT_VERSION);
      plhs[0] = mxCreateString (output_buf);
      return;
    }

  else
    MEX_FCN_ERR ("Unknown command code '%d'\n", cmd_code);
}


/**
 * Safely read numeric double scalar from MEX input.
 *
 * @param[in] idx MEX input position index (0 is first).
 * @param[in] nrhs Number of right-hand sides.
 * @param[in] mxArray  MEX input array.
 * @param[out] d If function returns `1`, `d` contains the double scalar
 *               extracted from the MEX input, otherwise `d` remains
 *               unchanged.
 *
 * @returns success of extraction.
 */
int
extract_d (int idx, int nrhs, const mxArray *prhs[], double *d)
{
  if ((nrhs > idx) && mxIsScalar (prhs[idx]) && mxIsNumeric (prhs[idx]))
    {
      *d = (double) mxGetScalar (prhs[idx]);
      return (1);
    }
  DBG_PRINTF ("%s\n", "Failed.");
  return (0);
}


/**
 * Safely read scalar signed integer (si) from MEX input.
 *
 * @param[in] idx MEX input position index (0 is first).
 * @param[in] nrhs Number of right-hand sides.
 * @param[in] mxArray  MEX input array.
 * @param[out] si If function returns `1`, `si` contains the scalar signed
 *                integer extracted from the MEX input, otherwise `si` remains
 *                unchanged.
 *
 * @returns success of extraction.
 */
int
extract_si (int idx, int nrhs, const mxArray *prhs[], int64_t *si)
{
  double d = 0.0;

  if (extract_d (idx, nrhs, prhs, &d) && mxIsFinite (d) && (floor (d) == d))
    {
      *si = (int64_t) d;
      return (1);
    }
  DBG_PRINTF ("%s\n", "Failed.");
  return (0);
}


/**
 * Safely read scalar unsigned integer (ui) from MEX input.
 *
 * @param[in] idx MEX input position index (0 is first).
 * @param[in] nrhs Number of right-hand sides.
 * @param[in] mxArray  MEX input array.
 * @param[out] ui If function returns `1`, `ui` contains the scalar unsigned
 *                integer extracted from the MEX input, otherwise `ui` remains
 *                unchanged.
 *
 * @returns success of extraction.
 */
int
extract_ui (int idx, int nrhs, const mxArray *prhs[], uint64_t *ui)
{
  int64_t si = 0.0;

  if (extract_si (idx, nrhs, prhs, &si) && (si >= 0))
    {
      *ui = (uint64_t) si;
      return (1);
    }
  DBG_PRINTF ("%s\n", "Failed.");
  return (0);
}


/**
 * Safely read vector of unsigned integer (ui) from MEX input.
 *
 * @param[in] idx MEX input position index (0 is first).
 * @param[in] nrhs Number of right-hand sides.
 * @param[in] mxArray  MEX input array.
 * @param[in] len Expected vector length.
 * @param[out] ui If function returns `1`, `ui` contains a vector of unsigned
 *                integers of length `len` extracted from the MEX input,
 *                otherwise `ui` is NULL.
 *
 * @returns success of extraction.
 */
int
extract_ui_vector (int idx, int nrhs, const mxArray *prhs[], uint64_t **ui,
                   size_t len)
{
  if ((nrhs > idx) && mxIsNumeric (prhs[idx])
      && ((mxGetM (prhs[idx]) * mxGetN (prhs[idx]) >= len)))
    {
      double *vec = mxGetPr (prhs[idx]);
      *ui = (uint64_t *) mxMalloc (len * sizeof(uint64_t));
      int good = 1;
      for (size_t i = 0; i < len; i++)
        {
          if ((vec[i] >= 0.0) && mxIsFinite (vec[i])
              && (floor (vec[i]) == vec[i]))
            (*ui)[i] = (uint64_t) vec[i];
          else
            {
              good = 0;
              break;
            }
        }
      if (good)
        return (1);

      mxFree (*ui);  // In case of an error free space.
      *ui = NULL;
    }
  DBG_PRINTF ("%s\n", "Failed.");
  return (0);
}

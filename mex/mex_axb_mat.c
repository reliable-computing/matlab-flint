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

#include "mex_arb_mat.h"

#define DATA_CHUNK_SIZE 1000  // Allocate variables in chunks.



// ARB_MAT / ACB_MAT memory management
// ===================================

typedef enum
{
    UNUSED = 0,
    REAL,
    COMPLEX,
}
axb_mat_state;

typedef struct
{
    void*         mat;
    axb_mat_state state;
}
axb_mat_struct;

typedef axb_mat_struct axb_mat[1];
typedef axb_mat_struct * axb_ptr;


// Similar to C++ std::vector:
// - xxx_capacity: number of elements that `xxx` has currently allocated space for.
// - xxx_size:     number of elements in `xxx`.

axb_ptr axb_mat_data          = NULL;
size_t  axb_mat_data_capacity = 0;
size_t  axb_mat_data_size     = 0;


/**
* Function called at exit of the mex file to tidy up all memory.
* After calling this function the initial state is restored.
*/
void
arb_tidy_up (void)
{
    DBG_PRINTF ("%s\n", "Call");
    for (size_t i = 0; i < axb_mat_data_size; i++) {
        switch (axb_mat_data[i].state) {
            case REAL:
                arb_mat_clear (axb_mat_data[i].mat);
                break;
            case COMPLEX:
                acb_mat_clear (axb_mat_data[i].mat);
                break;
            default:
                DBG_PRINTF ("%s\n", "Invalid state");
                break;
        }
    }
    mxFree (axb_mat_data);
    axb_mat_data          = NULL;
    axb_mat_data_capacity = 0;
    axb_mat_data_size     = 0;
}


/**
* Constructor for new ARB_MAT / ACB_MAT variables.
*
* @param nlhs MEX parameter.
*
* @returns NULL on failure, pointer to.
*/
axb_mat
axb_mat_allocate ()
{
    // Check if an unused element can be used
    for (size_t i = 0; i < axb_mat_data_capacity; i++)
    {
        if (axb_mat_data[i].state == UNUSED)
        {
            arb_mat_init(arb_mat_t mat, slong r, slong c);
        }
    }

    // Check if there is enough space to create new ARB variables.
    if ((arb_data_size + count) > arb_data_capacity)
    {
        // Determine new capacity.
        size_t new_capacity = arb_data_capacity;
        while ((arb_data_size + count) > new_capacity)
            new_capacity += DATA_CHUNK_SIZE;

        DBG_PRINTF ("Increase capacity to '%d'.\n", new_capacity);
        // Reallocate memory.
        if (axb_mat_data == NULL)
        {
            axb_mat_data = (arb_ptr) mxMalloc (new_capacity * sizeof(arb_t));
        }
        else
            axb_mat_data = (arb_ptr) mxRealloc (arb_data, new_capacity * sizeof(arb_t));
        if (arb_data == NULL)
            return (-1); // Memory allocation failed.

        mexMakeMemoryPersistent (axb_mat_data);

        // Initialize new ARB variables.
        for (size_t i = arb_data_capacity; i < new_capacity; i++)
            arb_init (arb_data + i);

        arb_data_capacity = new_capacity;
    }

    idx->start     = arb_data_size + 1;
    arb_data_size += count;
    idx->end       = arb_data_size;
    DBG_PRINTF ("New ARB variable [%d:%d] allocated.\n", idx->start, idx->end);
    return (0);
}



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
             uint64_t cmd_code)
{

    DBG_PRINTF ("Command: code = %d, nlhs = %d, nrhs = %d\n", (int) cmd_code, nlhs, nrhs);

    switch (cmd_code)
    {

        case 1000: // idx = arb_t.scalar_contructor(string inp, slong prec)
            {
                MEX_NARGINCHK (3);
                MEX_SLONG (2, prec);
                if (! mxIsChar (prhs[1])) {
                    MEX_FCN_ERR ("cmd[%d]: First argument must be a string.\n", cmd_code);
                }
                char *inp = mxArrayToString (prhs[1]);
                if (inp == NULL) {
                    MEX_FCN_ERR ("cmd[%d]: Failed to convert input to string.\n", cmd_code);
                }
                idx_t idx;
                if (mex_arb_allocate (1, &idx)) {
                    MEX_FCN_ERR ("cmd[%d]: Failed to allocate memory ARB variable.\n", cmd_code);
                }
                if (arb_set_str (arb_data + idx.start - 1, inp, prec)) {
                    MEX_FCN_ERR ("cmd[%d]: Failed to set ARB variable from string.\n", cmd_code);
                }
                mxFree (inp);

                plhs[0] = mxCreateDoubleMatrix (1, 2, mxREAL);
                double *vec = mxGetPr (plhs[0]);
                vec[0] = (double) idx.start;
                vec[1] = (double) idx.end;
                return;
            }


        case 1001: // char *arb_get_str(const arb_t x, slong n[, ulong flags])
            {
                MEX_NARGINCHK (3);
                MEX_ARB_T(1, idx);
                MEX_SLONG (2, prec);

                if (idx.start != idx.end) {
                    MEX_FCN_ERR ("cmd[%d]: idx must be a scalar index.\n", cmd_code);
                }
                char *output_buf = arb_get_str (arb_data + idx.start - 1, prec, 0);
                if (output_buf == NULL) {
                    MEX_FCN_ERR ("cmd[%d]: Failed to get string representation of ARB variable.\n", cmd_code);
                }
                plhs[0] = mxCreateString (output_buf);
                free (output_buf);
                return;
            }


        case 1002: // arb_t.clear_and_init(arb_t x)
            {
                MEX_NARGINCHK (2);
                MEX_ARB_T(1, idx);

                if (idx.start != idx.end) {
                    MEX_FCN_ERR ("cmd[%d]: idx must be a scalar index.\n", cmd_code);
                }
                DBG_PRINTF ("Release memory for arb_t '%d'.\n", idx.start);
                arb_clear (arb_data + idx.start - 1);
                arb_init (arb_data + idx.start - 1);
                return;
            }


        case 1100: // void arb_add(arb_t z, const arb_t x, const arb_t y, slong prec)
            {
                MEX_NARGINCHK (4);
                MEX_ARB_T(1, x);
                MEX_ARB_T(2, y);
                MEX_SLONG (3, prec);

                if (x.start != x.end || y.start != y.end) {
                    MEX_FCN_ERR ("cmd[%d]: x and y must be scalar indices.\n", cmd_code);
                }
                idx_t z;
                if (mex_arb_allocate (1, &z)) {
                    MEX_FCN_ERR ("cmd[%d]: Failed to allocate memory ARB variable.\n", cmd_code);
                }
                arb_add (arb_data + z.start - 1, arb_data + x.start - 1, arb_data + y.start - 1, prec);
                plhs[0] = mxCreateDoubleMatrix (1, 2, mxREAL);
                double *vec = mxGetPr (plhs[0]);
                vec[0] = (double) z.start;
                vec[1] = (double) z.end;
                return;
            }

        case 1101: // void arb_sub(arb_t z, const arb_t x, const arb_t y, slong prec)
            {
                MEX_NARGINCHK (4);
                MEX_ARB_T(1, x);
                MEX_ARB_T(2, y);
                MEX_SLONG (3, prec);

                if (x.start != x.end || y.start != y.end) {
                    MEX_FCN_ERR ("cmd[%d]: x and y must be scalar indices.\n", cmd_code);
                }
                idx_t z;
                if (mex_arb_allocate (1, &z)) {
                    MEX_FCN_ERR ("cmd[%d]: Failed to allocate memory ARB variable.\n", cmd_code);
                }
                arb_sub (arb_data + z.start - 1, arb_data + x.start - 1, arb_data + y.start - 1, prec);
                plhs[0] = mxCreateDoubleMatrix (1, 2, mxREAL);
                double *vec = mxGetPr (plhs[0]);
                vec[0] = (double) z.start;
                vec[1] = (double) z.end;
                return;
            }

        case 1102: // void arb_mul(arb_t z, const arb_t x, const arb_t y, slong prec)
            {
                MEX_NARGINCHK (4);
                MEX_ARB_T(1, x);
                MEX_ARB_T(2, y);
                MEX_SLONG (3, prec);

                if (x.start != x.end || y.start != y.end) {
                    MEX_FCN_ERR ("cmd[%d]: x and y must be scalar indices.\n", cmd_code);
                }
                idx_t z;
                if (mex_arb_allocate (1, &z)) {
                    MEX_FCN_ERR ("cmd[%d]: Failed to allocate memory ARB variable.\n", cmd_code);
                }
                arb_mul (arb_data + z.start - 1, arb_data + x.start - 1, arb_data + y.start - 1, prec);
                plhs[0] = mxCreateDoubleMatrix (1, 2, mxREAL);
                double *vec = mxGetPr (plhs[0]);
                vec[0] = (double) z.start;
                vec[1] = (double) z.end;
                return;
            }

        case 1103: // void arb_div(arb_t z, const arb_t x, const arb_t y, slong prec)
            {
                MEX_NARGINCHK (4);
                MEX_ARB_T(1, x);
                MEX_ARB_T(2, y);
                MEX_SLONG (3, prec);

                if (x.start != x.end || y.start != y.end) {
                    MEX_FCN_ERR ("cmd[%d]: x and y must be scalar indices.\n", cmd_code);
                }
                idx_t z;
                if (mex_arb_allocate (1, &z)) {
                    MEX_FCN_ERR ("cmd[%d]: Failed to allocate memory ARB variable.\n", cmd_code);
                }
                arb_div (arb_data + z.start - 1, arb_data + x.start - 1, arb_data + y.start - 1, prec);
                plhs[0] = mxCreateDoubleMatrix (1, 2, mxREAL);
                double *vec = mxGetPr (plhs[0]);
                vec[0] = (double) z.start;
                vec[1] = (double) z.end;
                return;
            }


        default:
            MEX_FCN_ERR ("Unknown command code '%d'\n", cmd_code);
    }

}


/**
* Safely read flint specific scalar slong (aka "long int") from MEX input.
*
* @param[in] idx MEX input position index (0 is first).
* @param[in] nrhs Number of right-hand sides.
* @param[in] mxArray  MEX input array.
* @param[out] slong If function returns `1`, `si` contains the scalar signed
*                   integer extracted from the MEX input, otherwise `si` remains
*                   unchanged.
*
* @returns 0 on failure, other value means success of extraction.
*/
int
extract_slong (int idx, int nrhs, const mxArray *prhs[], int64_t *si)
{
    double d = 0.0;

    if (extract_d (idx, nrhs, prhs, &d) && mxIsFinite (d) && (floor (d) == d))
    {
        *si = (slong) d;
        return (1);
    }
    DBG_PRINTF ("%s\n", "Failed.");
    return (0);
}

/*
 * C++ implementation of lapack routine dlartg
 *
 * $Id: dlartg.cpp,v 1.6 1993/04/06 20:41:24 alv Exp $
 *
 *************************************************************
 *
 * Rogue Wave Software, Inc.
 * P.O. Box 2328
 * Corvallis, OR 97339
 *
 * Copyright (C) 1993.
 * This software is subject to copyright protection under the
 * laws of the United States and other countries.
 *
 *************************************************************
 *
 * Translated from the Fortran using Cobalt Blue's FOR_C++,
 * and then massaged slightly to Rogue Wave format.
 *
 * Translated by FOR_C++, v1.1 (P), on 02/18/93 at 07:36:03
 * FOR_C++ Options SET: alloc do=rt no=p pf=dlapack,xlapack,dbla s=dv str=l - prototypes
 *
 * $Log: dlartg.cpp,v $
 * Revision 1.6  1993/04/06 20:41:24  alv
 * added const to parameters; added include lapkdefs
 *
 * Revision 1.5  1993/03/19  17:18:24  alv
 * added RWLAPKDECL linkage specifier
 *
 * Revision 1.4  1993/03/19  16:57:25  alv
 * sprinkled in some const
 *
 * Revision 1.3  1993/03/09  16:14:40  alv
 * made parms const
 *
 * Revision 1.2  1993/03/05  23:15:47  alv
 * changed ref parms to const ref
 *
 * Revision 1.1  1993/03/03  16:07:40  alv
 * Initial revision
 *
 */

#if 0
#include "rw/lapkdefs.h"
#include "rw/bla.h"
#include "rw/lapack.h"
#include "rw/fortran.h" /* Fortran run time library */
#else
#include "level/lapack.h"
#endif

RWLAPKDECL void /*FUNCTION*/ dlartg(const double &f, const double &g, double &cs, double &sn, 
                                    double &r)
{
// PARAMETER translations
    const double ZERO = 0.0e0;
    const double ONE = 1.0e0;
// end of PARAMETER translations

    double t, tt;

  
    //  -- LAPACK auxiliary routine (version 1.0) --
    //     Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,
    //     Courant Institute, Argonne National Lab, and Rice University
    //     February 29, 1992
  
    //     .. Scalar Arguments ..
    //     ..
  
    //  Purpose
    //  =======
  
    //  DLARTG generate a plane rotation so that
  
    //     [  CS  SN  ]  .  [ F ]  =  [ R ]   where CS**2 + SN**2 = 1.
    //     [ -SN  CS  ]     [ G ]     [ 0 ]
  
    //  This is a faster version of the BLAS1 routine DROTG, except for
    //  the following differences:
    //     F and G are unchanged on return.
    //     If G=0, then CS=1 and SN=0.
    //     If F=0 and (G .ne. 0), then CS=0 and SN=1 without doing any
    //        floating point operations (saves work in DBDSQR when
    //        there are zeros on the diagonal).
  
    //  Arguments
    //  =========
  
    //  F       (input) DOUBLE PRECISION
    //          The first component of vector to be rotated.
  
    //  G       (input) DOUBLE PRECISION
    //          The second component of vector to be rotated.
  
    //  CS      (output) DOUBLE PRECISION
    //          The cosine of the rotation.
  
    //  SN      (output) DOUBLE PRECISION
    //          The sine of the rotation.
  
    //  R       (output) DOUBLE PRECISION
    //          The nonzero component of the rotated vector.
  
    //     .. Parameters ..
    //     ..
    //     .. Local Scalars ..
    //     ..
    //     .. Intrinsic Functions ..
    //     ..
    //     .. Executable Statements ..
  
    if( f == ZERO ) { 
        if( g == ZERO ) { 
            cs = ONE;
            sn = ZERO;
            r = ZERO;
        }
        else { 
            cs = ZERO;
            sn = ONE;
            r = g;
        }
    }
    else { 
        if( fabs( f ) > fabs( g ) ) { 
            t = g/f;
            tt = sqrt( ONE + t*t );
            cs = ONE/tt;
            sn = t*cs;
            r = f*tt;
        }
        else { 
            t = f/g;
            tt = sqrt( ONE + t*t );
            sn = ONE/tt;
            cs = t*sn;
            r = g*tt;
        }
    }
    return;
  
    //     End of DLARTG
  
} // end of function 


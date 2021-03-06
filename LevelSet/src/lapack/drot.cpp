/*
 * Default C++ implementation of drot
 * For optimum performance, use a machine specific bla library
 *
 * $Id: drot.cpp,v 1.3 1993/03/19 16:26:58 alv Exp $
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
 * Translated from the default Fortran bla using Cobalt
 * Blue's FOR_C++, and then massaged slightly to Rogue
 * Wave format.
 *
 * Translated by FOR_C++, v1.1 (P), on 02/17/93 at 14:40:06
 * FOR_C++ Options SET: alloc do=rt no=p pf=dbla,xbla s=dv str=l - prototypes
 *
 * $Log: drot.cpp,v $
 * Revision 1.3  1993/03/19 16:26:58  alv
 * added RWBLADECL linkage specification
 *
 * Revision 1.2  1993/03/05  23:07:16  alv
 * changed ref parms to const ref
 *
 * Revision 1.1  1993/03/03  16:04:38  alv
 * Initial revision
 *
 */

#define RW_CPPBLAS 1
#if 0
#include "rw/bla.h"
#include "rw/bla.h"
#include "rw/fortran.h" /* Fortran run time library */
#else
#include "level/lapack.h"
#endif

RWLAPKDECL void drot(const long &n, double dx[], const long &incx, double dy[], 
                     const long &incy, const double &c, const double &s)
{
    long _do0, _do1, i, i_, ix, iy;
    double dtemp;

  
    //     applies a plane rotation.
    //     jack dongarra, linpack, 3/11/78.
  
  
    if( n <= 0 ) 
        return;
    if( incx == 1 && incy == 1 ) 
        goto L_20;
  
    //       code for unequal increments or equal increments not equal
    //         to 1
  
    ix = 1;
    iy = 1;
    if( incx < 0 ) 
        ix = (-n + 1)*incx + 1;
    if( incy < 0 ) 
        iy = (-n + 1)*incy + 1;
    for( i = 1, i_ = i - 1, _do0 = n; i <= _do0; i++, i_++ ) { 
        dtemp = c*dx[ix - 1] + s*dy[iy - 1];
        dy[iy - 1] = c*dy[iy - 1] - s*dx[ix - 1];
        dx[ix - 1] = dtemp;
        ix = ix + incx;
        iy = iy + incy;
    }
    return;
  
    //       code for both increments equal to 1
  
L_20:
    for( i = 1, i_ = i - 1, _do1 = n; i <= _do1; i++, i_++ ) { 
        dtemp = c*dx[i_] + s*dy[i_];
        dy[i_] = c*dy[i_] - s*dx[i_];
        dx[i_] = dtemp;
    }
    return;
} // end of function 


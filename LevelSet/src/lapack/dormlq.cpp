/*
 * C++ implementation of lapack routine dormlq
 *
 * $Id: dormlq.cpp,v 1.7 1993/07/07 23:00:32 alv Exp $
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
 * Translated by FOR_C++, v1.1 (P), on 02/18/93 at 07:36:58
 * FOR_C++ Options SET: alloc do=rt no=p pf=dlapack,xlapack,dbla s=dv str=l - prototypes
 *
 * $Log: dormlq.cpp,v $
 * Revision 1.7  1993/07/07 23:00:32  alv
 * NBMAX reduced for DOS to avoid running out of memory
 *
 * Revision 1.6  1993/04/06  20:41:46  alv
 * added const to parameters; added include lapkdefs
 *
 * Revision 1.5  1993/03/19  18:41:23  alv
 * now passes chars explicitly, rather than indirection of a string, to shut up SUN warnings
 *
 * Revision 1.4  1993/03/19  17:18:24  alv
 * added RWLAPKDECL linkage specifier
 *
 * Revision 1.3  1993/03/09  16:14:40  alv
 * made parms const
 *
 * Revision 1.2  1993/03/05  23:16:22  alv
 * changed ref parms to const ref
 *
 * Revision 1.1  1993/03/03  16:08:13  alv
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

RWLAPKDECL void /*FUNCTION*/ dormlq(const char &side, const char &trans, const long &m, const long &n, 
                                    const long &k, double *a, const long &lda, double tau[], double *c, const long &ldc, 
                                    double work[], const long &lwork, long &info)
{
#define A(I_,J_)  (*(a+(I_)*(lda)+(J_)))
#define C(I_,J_)  (*(c+(I_)*(ldc)+(J_)))
// PARAMETER translations
#if defined(__MSDOS__)
    const long NBMAX = 8;
#else
    const long NBMAX = 64;
#endif
// end of PARAMETER translations

    // PARAMETER translations
    const long LDT = NBMAX + 1;
    // end of PARAMETER translations

    char _c0[2], _c1[2];
    int left, notran;
    char transt;
    long _do0, _do1, i, i1, i2, i3, i_, ib, ic, iinfo, iws, jc, 
        ldwork, mi, nb, nbmin, ni, nq, nw;
    double (*const t)[LDT]=(double(*)[LDT])new double[NBMAX*LDT];
#define NCHRTMPS 1
    CHRTMP _c[NCHRTMPS];
    ini_chrtmp(_c,NCHRTMPS);
    if( t == NULL )
        memerr( "unable to allocate object in dormlq()" );

  
    //  -- LAPACK routine (version 1.0) --
    //     Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,
    //     Courant Institute, Argonne National Lab, and Rice University
    //     February 29, 1992
  
    //     .. Scalar Arguments ..
    //     ..
    //     .. Array Arguments ..
    //     ..
  
    //  Purpose
    //  =======
  
    //  DORMLQ overwrites the general real m by n matrix C with
  
    //        Q * C  if SIDE = 'L' and TRANS = 'N', or
  
    //        Q'* C  if SIDE = 'L' and TRANS = 'T', or
  
    //        C * Q  if SIDE = 'R' and TRANS = 'N', or
  
    //        C * Q' if SIDE = 'R' and TRANS = 'T',
  
    //  where Q is a real orthogonal matrix defined as the product of k
    //  elementary reflectors
  
    //        Q = H(k) . . . H(2) H(1)
  
    //  as returned by DGELQF. Q is of order m if SIDE = 'L' and of order n
    //  if SIDE = 'R'.
  
    //  Arguments
    //  =========
  
    //  SIDE    (input) CHARACTER*1
    //          = 'L': apply Q or Q' from the Left
    //          = 'R': apply Q or Q' from the Right
  
    //  TRANS   (input) CHARACTER*1
    //          = 'N': apply Q  (No transpose)
    //          = 'T': apply Q' (Transpose)
  
    //  M       (input) INTEGER
    //          The number of rows of the matrix C. M >= 0.
  
    //  N       (input) INTEGER
    //          The number of columns of the matrix C. N >= 0.
  
    //  K       (input) INTEGER
    //          The number of elementary reflectors whose product defines
    //          the matrix Q.
    //          If SIDE = 'L', M >= K >= 0;
    //          if SIDE = 'R', N >= K >= 0.
  
    //  A       (input) DOUBLE PRECISION array, dimension
    //                               (LDA,M) if SIDE = 'L',
    //                               (LDA,N) if SIDE = 'R'
    //          The i-th row must contain the vector which defines the
    //          elementary reflector H(i), for i = 1,2,...,k, as returned by
    //          DGELQF in the first k rows of its array argument A.
    //          A is modified by the routine but restored on exit.
  
    //  LDA     (input) INTEGER
    //          The leading dimension of the array A. LDA >= max(1,K).
  
    //  TAU     (input) DOUBLE PRECISION array, dimension (K)
    //          TAU(i) must contain the scalar factor of the elementary
    //          reflector H(i), as returned by DGELQF.
  
    //  C       (input/output) DOUBLE PRECISION array, dimension (LDC,N)
    //          On entry, the m-by-n matrix C.
    //          On exit, C is overwritten by Q*C or Q'*C or C*Q' or C*Q.
  
    //  LDC     (input) INTEGER
    //          The leading dimension of the array C. LDC >= max(1,M).
  
    //  WORK    (workspace) DOUBLE PRECISION array, dimension (LWORK)
    //          On exit, if INFO = 0, WORK(1) returns the minimum value of
    //          LWORK required to use the optimal blocksize.
  
    //  LWORK   (input) INTEGER
    //          The dimension of the array WORK.
    //          If SIDE = 'L', LWORK >= max(1,N);
    //          if SIDE = 'R', LWORK >= max(1,M).
    //          For optimum performance LWORK should be at least N*NB
    //          if SIDE = 'L' and at least M*NB if SIDE = 'R', where NB is
    //          the optimal blocksize.
  
    //  INFO    (output) INTEGER
    //          = 0: successful exit
    //          < 0: if INFO = -i, the i-th argument had an illegal value
  
    //  =====================================================================
  
    //     .. Parameters ..
    //     ..
    //     .. Local Scalars ..
    //     ..
    //     .. Local Arrays ..
    //     ..
    //     .. External Functions ..
    //     ..
    //     .. External Subroutines ..
    //     ..
    //     .. Intrinsic Functions ..
    //     ..
    //     .. Executable Statements ..
  
    //     Test the input arguments
  
    info = 0;
    left = lsame( side, 'L' );
    notran = lsame( trans, 'N' );
  
    //     NQ is the order of Q and NW is the minimum dimension of WORK
  
    if( left ) { 
        nq = m;
        nw = n;
    }
    else { 
        nq = n;
        nw = m;
    }
    if( !left && !lsame( side, 'R' ) ) { 
        info = -1;
    }
    else if( !notran && !lsame( trans, 'T' ) ) { 
        info = -2;
    }
    else if( m < 0 ) { 
        info = -3;
    }
    else if( n < 0 ) { 
        info = -4;
    }
    else if( k < 0 || k > nq ) { 
        info = -5;
    }
    else if( lda < max( 1, k ) ) { 
        info = -7;
    }
    else if( ldc < max( 1, m ) ) { 
        info = -10;
    }
    else if( lwork < max( 1, nw ) ) { 
        info = -12;
    }
    if( info != 0 ) { 
        xerbla( "DORMLQ", -info );
        delete [] t;
        return;
    }
  
    //     Quick return if possible
  
    if( (m == 0 || n == 0) || k == 0 ) { 
        work[0] = 1;
        delete [] t;
        return;
    }
  
    //     Determine the block size.  NB may be at most NBMAX, where NBMAX
    //     is used to define the local array T.
  
    nb = min( NBMAX, ilaenv( 1, "DORMLQ", f_concat(&_c[0],STR1(_c0,side),
                                                   STR1(_c1,trans),NULL), m, n, k, -1 ) );
    nbmin = 2;
    ldwork = nw;
    if( nb > 1 && nb < k ) { 
        iws = nw*nb;
        if( lwork < iws ) { 
            nb = lwork/ldwork;
            nbmin = max( 2, ilaenv( 2, "DORMLQ", f_concat(&_c[0],STR1(_c0,side),
                                                          STR1(_c1,trans),NULL), m, n, k, -1 ) );
        }
    }
    else { 
        iws = nw;
    }
  
    if( nb < nbmin || nb >= k ) { 
    
        //        Use unblocked code
    
        dorml2( side, trans, m, n, k, a, lda, tau, c, ldc, work, iinfo );
    }
    else { 
    
        //        Use blocked code
    
        if( (left && notran) || (!left && !notran) ) { 
            i1 = 1;
            i2 = k;
            i3 = nb;
        }
        else { 
            i1 = ((k - 1)/nb)*nb + 1;
            i2 = 1;
            i3 = -nb;
        }
    
        if( left ) { 
            ni = n;
            jc = 1;
        }
        else { 
            mi = m;
            ic = 1;
        }
    
        if( notran ) { 
            transt = 'T';
        }
        else { 
            transt = 'N';
        }
    
        for( i = i1, i_ = i - 1, _do0=docnt(i,i2,_do1 = i3); _do0 > 0; i += _do1, i_ += _do1, _do0-- ) { 
            ib = min( nb, k - i + 1 );
      
            //           Form the triangular factor of the block reflector
            //           H = H(i) H(i+1) . . . H(i+ib-1)
      
            dlarft( 'F'/* Forward */, 'R'/* Rowwise */, nq - i + 1, 
                    ib, &A(i_,i_), lda, &tau[i_], (double*)t, LDT );
            if( left ) { 
        
                //              H or H' is applied to C(i:m,1:n)
        
                mi = m - i + 1;
                ic = i;
            }
            else { 
        
                //              H or H' is applied to C(1:m,i:n)
        
                ni = n - i + 1;
                jc = i;
            }
      
            //           Apply H or H'
      
            dlarfb( side, transt, 'F'/* Forward */, 'R'/* Rowwise */
                    , mi, ni, ib, &A(i_,i_), lda, (double*)t, LDT, &C(jc - 1,ic - 1), 
                    ldc, work, ldwork );
        }
    }
    work[0] = iws;
    rel_chrtmp(_c,NCHRTMPS);
    delete [] t;
    return;
  
    //     End of DORMLQ
  
#undef  NCHRTMPS
#undef  C
#undef  A
} // end of function 


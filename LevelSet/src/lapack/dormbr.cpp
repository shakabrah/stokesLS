/*
 * C++ implementation of lapack routine dormbr
 *
 * $Id: dormbr.cpp,v 1.5 1993/04/06 20:41:45 alv Exp $
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
 * Translated by FOR_C++, v1.1 (P), on 02/18/93 at 07:36:54
 * FOR_C++ Options SET: alloc do=rt no=p pf=dlapack,xlapack,dbla s=dv str=l - prototypes
 *
 * $Log: dormbr.cpp,v $
 * Revision 1.5  1993/04/06 20:41:45  alv
 * added const to parameters; added include lapkdefs
 *
 * Revision 1.4  1993/03/19  17:18:24  alv
 * added RWLAPKDECL linkage specifier
 *
 * Revision 1.3  1993/03/09  16:14:40  alv
 * made parms const
 *
 * Revision 1.2  1993/03/05  23:16:20  alv
 * changed ref parms to const ref
 *
 * Revision 1.1  1993/03/03  16:08:08  alv
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

RWLAPKDECL void /*FUNCTION*/ dormbr(const char &vect, const char &side, const char &trans, const long &m, 
                                    const long &n, const long &k, double *a, const long &lda, double tau[], 
                                    double *c, const long &ldc, double work[], const long &lwork, long &info)
{
#define A(I_,J_)  (*(a+(I_)*(lda)+(J_)))
#define C(I_,J_)  (*(c+(I_)*(ldc)+(J_)))
    int applyq, left, notran;
    char transt;
    long i1, i2, iinfo, mi, ni, nq, nw;

  
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
  
    //  If VECT = 'Q', DORMBR overwrites the general real m by n matrix C
    //  with
  
    //        Q * C  if SIDE = 'L' and TRANS = 'N', or
  
    //        Q'* C  if SIDE = 'L' and TRANS = 'T', or
  
    //        C * Q  if SIDE = 'R' and TRANS = 'N', or
  
    //        C * Q' if SIDE = 'R' and TRANS = 'T'.
  
    //  If VECT = 'P', DORMBR overwrites the general real m by n matrix C
    //  with
  
    //        P * C  if SIDE = 'L' and TRANS = 'N', or
  
    //        P'* C  if SIDE = 'L' and TRANS = 'T', or
  
    //        C * P  if SIDE = 'R' and TRANS = 'N', or
  
    //        C * P' if SIDE = 'R' and TRANS = 'T'.
  
    //  Here Q and P' are the orthogonal matrices determined by DGEBRD when
    //  reducing a real matrix A to bidiagonal form: A = Q * B * P'. Q and P'
    //  are defined as products of elementary reflectors H(i) and G(i)
    //  respectively.
  
    //  Let nq = m if SIDE = 'L' and nq = n if SIDE = 'R'. Thus nq is the
    //  order of the orthogonal matrix Q or P' that is applied.
  
    //  If VECT = 'Q', A is assumed to have been an nq-by-k matrix:
    //  if nq >= k, Q = H(1) H(2) . . . H(k);
    //  if nq < k, Q = H(1) H(2) . . . H(nq-1).
  
    //  If VECT = 'P', A is assumed to have been a k-by-nq matrix:
    //  if k < nq, P = G(1) G(2) . . . G(k);
    //  if k >= nq, P = G(1) G(2) . . . G(nq-1).
  
    //  Arguments
    //  =========
  
    //  VECT    (input) CHARACTER*1
    //          = 'Q': apply Q or Q'
    //          = 'P': apply P or P'
  
    //  SIDE    (input) CHARACTER*1
    //          = 'L': apply Q, Q', P or P' from the Left
    //          = 'R': apply Q, Q', P or P' from the Right
  
    //  TRANS   (input) CHARACTER*1
    //          = 'N': apply Q  or P  (No transpose)
    //          = 'T': apply Q' or P' (Transpose)
  
    //  M       (input) INTEGER
    //          The number of rows of the matrix C. M >= 0.
  
    //  N       (input) INTEGER
    //          The number of columns of the matrix C. N >= 0.
  
    //  K       (input) INTEGER
    //          If VECT = 'Q', the number of columns in the original
    //          matrix reduced by DGEBRD.
    //          If VECT = 'P', the number of rows in the original
    //          matrix reduced by DGEBRD.
    //          K >= 0.
  
    //  A       (input) DOUBLE PRECISION array, dimension
    //                                (LDA,min(nq,K)) if VECT = 'Q'
    //                                (LDA,nq)        if VECT = 'P'
    //          The vectors which define the elementary reflectors H(i) and
    //          G(i), whose products determine the matrices Q and P, as
    //          returned by DGEBRD.
  
    //  LDA     (input) INTEGER
    //          The leading dimension of the array A.
    //          If VECT = 'Q', LDA >= max(1,nq);
    //          if VECT = 'P', LDA >= max(1,min(nq,K)).
  
    //  TAU     (input) DOUBLE PRECISION array, dimension (min(nq,K))
    //          TAU(i) must contain the scalar factor of the elementary
    //          reflector H(i) or G(i) which determines Q or P, as returned
    //          by DGEBRD in the array argument TAUQ or TAUP.
  
    //  C       (input/output) DOUBLE PRECISION array, dimension (LDC,N)
    //          On entry, the m by n matrix C.
    //          On exit, C is overwritten by Q*C or Q'*C or C*Q' or C*Q
    //          or P*C or P'*C or C*P or C*P'.
  
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
  
    //     .. Local Scalars ..
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
    applyq = lsame( vect, 'Q' );
    left = lsame( side, 'L' );
    notran = lsame( trans, 'N' );
  
    //     NQ is the order of Q or P and NW is the minimum dimension of WORK
  
    if( left ) { 
        nq = m;
        nw = n;
    }
    else { 
        nq = n;
        nw = m;
    }
    if( !applyq && !lsame( vect, 'P' ) ) { 
        info = -1;
    }
    else if( !left && !lsame( side, 'R' ) ) { 
        info = -2;
    }
    else if( !notran && !lsame( trans, 'T' ) ) { 
        info = -3;
    }
    else if( m < 0 ) { 
        info = -4;
    }
    else if( n < 0 ) { 
        info = -5;
    }
    else if( k < 0 ) { 
        info = -6;
    }
    else if( (applyq && lda < max( 1, nq )) || (!applyq && lda < max( 1, 
                                                                      min( nq, k ) )) ) { 
        info = -8;
    }
    else if( ldc < max( 1, m ) ) { 
        info = -11;
    }
    else if( lwork < max( 1, nw ) ) { 
        info = -13;
    }
    if( info != 0 ) { 
        xerbla( "DORMBR", -info );
        return;
    }
  
    //     Quick return if possible
  
    if( m == 0 || n == 0 ) { 
        work[0] = 1;
        return;
    }
  
    if( applyq ) { 
    
        //        Apply Q
    
        if( nq >= k ) { 
      
            //           Q was determined by a call to DGEBRD with nq >= k
      
            dormqr( side, trans, m, n, k, a, lda, tau, c, ldc, work, 
                    lwork, iinfo );
        }
        else { 
      
            //           Q was determined by a call to DGEBRD with nq < k
      
            if( left ) { 
                mi = m - 1;
                ni = n;
                i1 = 2;
                i2 = 1;
            }
            else { 
                mi = m;
                ni = n - 1;
                i1 = 1;
                i2 = 2;
            }
            dormqr( side, trans, mi, ni, nq - 1, &A(0,1), lda, tau, 
                    &C(i2 - 1,i1 - 1), ldc, work, lwork, iinfo );
        }
    }
    else { 
    
        //        Apply P
    
        if( notran ) { 
            transt = 'T';
        }
        else { 
            transt = 'N';
        }
        if( nq > k ) { 
      
            //           P was determined by a call to DGEBRD with nq > k
      
            dormlq( side, transt, m, n, k, a, lda, tau, c, ldc, work, 
                    lwork, iinfo );
        }
        else { 
      
            //           P was determined by a call to DGEBRD with nq <= k
      
            if( left ) { 
                mi = m - 1;
                ni = n;
                i1 = 2;
                i2 = 1;
            }
            else { 
                mi = m;
                ni = n - 1;
                i1 = 1;
                i2 = 2;
            }
            dormlq( side, transt, mi, ni, nq - 1, &A(1,0), lda, tau, 
                    &C(i2 - 1,i1 - 1), ldc, work, lwork, iinfo );
        }
    }
    return;
  
    //     End of DORMBR
  
#undef  C
#undef  A
} // end of function 


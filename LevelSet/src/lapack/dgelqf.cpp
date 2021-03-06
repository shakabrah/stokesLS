/*
 * C++ implementation of lapack routine dgelqf
 *
 * $Id: dgelqf.cpp,v 1.6 1993/04/06 20:40:30 alv Exp $
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
 * Translated by FOR_C++, v1.1 (P), on 02/18/93 at 07:34:08
 * FOR_C++ Options SET: alloc do=rt no=p pf=dlapack,xlapack,dbla s=dv str=l - prototypes
 *
 * $Log: dgelqf.cpp,v $
 * Revision 1.6  1993/04/06 20:40:30  alv
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
 * Revision 1.2  1993/03/05  23:14:39  alv
 * changed ref parms to const ref
 *
 * Revision 1.1  1993/03/03  16:06:37  alv
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

RWLAPKDECL void /*FUNCTION*/ dgelqf(const long &m, const long &n, double *a, const long &lda, 
                                    double tau[], double work[], const long &lwork, long &info)
{
#define A(I_,J_)  (*(a+(I_)*(lda)+(J_)))
    long _do0, _do1, i, i_, ib, iinfo, iws, k, ldwork, nb, nbmin, 
        nx;

  
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
  
    //  DGELQF computes an LQ factorization of a real m by n matrix A:
    //  A = L * Q.
  
    //  Arguments
    //  =========
  
    //  M       (input) INTEGER
    //          The number of rows of the matrix A.  M >= 0.
  
    //  N       (input) INTEGER
    //          The number of columns of the matrix A.  N >= 0.
  
    //  A       (input/output) DOUBLE PRECISION array, dimension (LDA,N)
    //          On entry, the m by n matrix A.
    //          On exit, the elements on and below the diagonal of the array
    //          contain the m by min(m,n) lower trapezoidal matrix L (L is
    //          lower triangular if m <= n); the elements above the diagonal,
    //          with the array TAU, represent the orthogonal matrix Q as a
    //          product of elementary reflectors (see Further Details).
  
    //  LDA     (input) INTEGER
    //          The leading dimension of the array A.  LDA >= max(1,M).
  
    //  TAU     (output) DOUBLE PRECISION array, dimension (min(M,N))
    //          The scalar factors of the elementary reflectors (see Further
    //          Details).
  
    //  WORK    (workspace) DOUBLE PRECISION array, dimension (LWORK)
    //          On exit, if INFO = 0, WORK(1) returns the minimum value of
    //          LWORK required to use the optimal blocksize.
  
    //  LWORK   (input) INTEGER
    //          The dimension of the array WORK.  LWORK >= max(1,M).
    //          For optimum performance LWORK should be at least M*NB,
    //          where NB is the optimal blocksize.
  
    //  INFO    (output) INTEGER
    //          = 0: successful exit
    //          < 0: if INFO = -i, the i-th argument had an illegal value
  
    //  Further Details
    //  ===============
  
    //  The matrix Q is represented as a product of elementary reflectors
  
    //     Q = H(k) . . . H(2) H(1), where k = min(m,n).
  
    //  Each H(i) has the form
  
    //     H(i) = I - tau * v * v'
  
    //  where tau is a real scalar, and v is a real vector with
    //  v(1:i-1) = 0 and v(i) = 1; v(i+1:n) is stored on exit in A(i,i+1:n),
    //  and tau in TAU(i).
  
    //  =====================================================================
  
    //     .. Local Scalars ..
    //     ..
    //     .. External Subroutines ..
    //     ..
    //     .. Intrinsic Functions ..
    //     ..
    //     .. External Functions ..
    //     ..
    //     .. Executable Statements ..
  
    //     Test the input arguments
  
    info = 0;
    if( m < 0 ) { 
        info = -1;
    }
    else if( n < 0 ) { 
        info = -2;
    }
    else if( lda < max( 1, m ) ) { 
        info = -4;
    }
    else if( lwork < max( 1, m ) ) { 
        info = -7;
    }
    if( info != 0 ) { 
        xerbla( "DGELQF", -info );
        return;
    }
  
    //     Quick return if possible
  
    k = min( m, n );
    if( k == 0 ) { 
        work[0] = 1;
        return;
    }
  
    //     Determine the block size.
  
    nb = ilaenv( 1, "DGELQF", " ", m, n, -1, -1 );
    nbmin = 2;
    nx = 0;
    iws = m;
    if( nb > 1 && nb < k ) { 
    
        //        Determine when to cross over from blocked to unblocked code.
    
        nx = max( 0, ilaenv( 3, "DGELQF", " ", m, n, -1, -1 ) );
        if( nx < k ) { 
      
            //           Determine if workspace is large enough for blocked code.
      
            ldwork = m;
            iws = ldwork*nb;
            if( lwork < iws ) { 
        
                //              Not enough workspace to use optimal NB:  reduce NB and
                //              determine the minimum value of NB.
        
                nb = lwork/ldwork;
                nbmin = max( 2, ilaenv( 2, "DGELQF", " ", m, n, -1, 
                                        -1 ) );
            }
        }
    }
  
    if( (nb >= nbmin && nb < k) && nx < k ) { 
    
        //        Use blocked code initially
    
        for( i = 1, i_ = i - 1, _do0=docnt(i,k - nx,_do1 = nb); _do0 > 0; i += _do1, i_ += _do1, _do0-- ) { 
            ib = min( k - i + 1, nb );
      
            //           Compute the LQ factorization of the current block
            //           A(i:i+ib-1,i:n)
      
            dgelq2( ib, n - i + 1, &A(i_,i_), lda, &tau[i_], work, 
                    iinfo );
            if( i + ib <= m ) { 
        
                //              Form the triangular factor of the block reflector
                //              H = H(i) H(i+1) . . . H(i+ib-1)
        
                dlarft( 'F'/* Forward */, 'R'/* Rowwise */, n - i + 
                        1, ib, &A(i_,i_), lda, &tau[i_], work, ldwork );
        
                //              Apply H to A(i+ib:m,i:n) from the right
        
                dlarfb( 'R'/* Right */, 'N'/* No transpose */, 'F'/* Forward */
                        , 'R'/* Rowwise */, m - i - ib + 1, n - i + 1, ib, 
                        &A(i_,i_), lda, work, ldwork, &A(i_,i_ + ib), lda, 
                        &work[ib], ldwork );
            }
        }
    }
    else { 
        i = 1;
    }
  
    //     Use unblocked code to factor the last or only block.
  
    if( i <= k ) 
        dgelq2( m - i + 1, n - i + 1, &A(i - 1,i - 1), lda, &tau[i - 1], 
                work, iinfo );
  
    work[0] = iws;
    return;
  
    //     End of DGELQF
  
#undef  A
} // end of function 


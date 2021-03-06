/*
 * C++ implementation of lapack routine dlarft
 *
 * $Id: dlarft.cpp,v 1.5 1993/04/06 20:41:18 alv Exp $
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
 * Translated by FOR_C++, v1.1 (P), on 02/18/93 at 07:35:57
 * FOR_C++ Options SET: alloc do=rt no=p pf=dlapack,xlapack,dbla s=dv str=l - prototypes
 *
 * $Log: dlarft.cpp,v $
 * Revision 1.5  1993/04/06 20:41:18  alv
 * added const to parameters; added include lapkdefs
 *
 * Revision 1.4  1993/03/19  18:41:23  alv
 * now passes chars explicitly, rather than indirection of a string, to shut up SUN warnings
 *
 * Revision 1.3  1993/03/19  17:18:24  alv
 * added RWLAPKDECL linkage specifier
 *
 * Revision 1.2  1993/03/05  23:15:43  alv
 * changed ref parms to const ref
 *
 * Revision 1.1  1993/03/03  16:07:37  alv
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

RWLAPKDECL void /*FUNCTION*/ dlarft(const char &direct, const char &storev, const long &n, 
                                    const long &k, double *v, const long &ldv, double tau[], double *t, const long &ldt)
{
#define V(I_,J_)  (*(v+(I_)*(ldv)+(J_)))
#define T(I_,J_)  (*(t+(I_)*(ldt)+(J_)))
// PARAMETER translations
    const double ONE = 1.0e0;
    const double ZERO = 0.0e0;
// end of PARAMETER translations

    long _do0, _do1, _do2, i, i_, j, j_;
    double vii;

  
    //  -- LAPACK auxiliary routine (version 1.0) --
    //     Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,
    //     Courant Institute, Argonne National Lab, and Rice University
    //     February 29, 1992
  
    //     .. Scalar Arguments ..
    //     ..
    //     .. Array Arguments ..
    //     ..
  
    //  Purpose
    //  =======
  
    //  DLARFT forms the triangular factor T of a real block reflector H
    //  of order n, which is defined as a product of k elementary reflectors.
  
    //  If DIRECT = 'F', H = H(1) H(2) . . . H(k) and T is upper triangular;
  
    //  If DIRECT = 'B', H = H(k) . . . H(2) H(1) and T is lower triangular.
  
    //  If STOREV = 'C', the vector which defines the elementary reflector
    //  H(i) is stored in the i-th column of the array V, and
  
    //     H  =  I - V * T * V'
  
    //  If STOREV = 'R', the vector which defines the elementary reflector
    //  H(i) is stored in the i-th row of the array V, and
  
    //     H  =  I - V' * T * V
  
    //  Arguments
    //  =========
  
    //  DIRECT  (input) CHARACTER*1
    //          Specifies the order in which the elementary reflectors are
    //          multiplied to form the block reflector:
    //          = 'F': H = H(1) H(2) . . . H(k) (Forward)
    //          = 'B': H = H(k) . . . H(2) H(1) (Backward)
  
    //  STOREV  (input) CHARACTER*1
    //          Specifies how the vectors which define the elementary
    //          reflectors are stored (see also Further Details):
    //          = 'C': columnwise
    //          = 'R': rowwise
  
    //  N       (input) INTEGER
    //          The order of the block reflector H. N >= 0.
  
    //  K       (input) INTEGER
    //          The order of the triangular factor T (= the number of
    //          elementary reflectors). K >= 1.
  
    //  V       (input/output) DOUBLE PRECISION array, dimension
    //                               (LDV,K) if STOREV = 'C'
    //                               (LDV,N) if STOREV = 'R'
    //          The matrix V. See further details.
  
    //  LDV     (input) INTEGER
    //          The leading dimension of the array V.
    //          If STOREV = 'C', LDV >= max(1,N); if STOREV = 'R', LDV >= K.
  
    //  TAU     (input) DOUBLE PRECISION array, dimension (K)
    //          TAU(i) must contain the scalar factor of the elementary
    //          reflector H(i).
  
    //  T       (output) DOUBLE PRECISION array, dimension (LDT,K)
    //          The k by k triangular factor T of the block reflector.
    //          If DIRECT = 'F', T is upper triangular; if DIRECT = 'B', T is
    //          lower triangular. The rest of the array is not used.
  
    //  LDT     (input) INTEGER
    //          The leading dimension of the array T. LDT >= K.
  
    //  Further Details
    //  ===============
  
    //  The shape of the matrix V and the storage of the vectors which define
    //  the H(i) is best illustrated by the following example with n = 5 and
    //  k = 3. The elements equal to 1 are not stored; the corresponding
    //  array elements are modified but restored on exit. The rest of the
    //  array is not used.
  
    //  DIRECT = 'F' and STOREV = 'C':         DIRECT = 'F' and STOREV = 'R':
  
    //               V = (  1       )                 V = (  1 v1 v1 v1 v1 )
    //                   ( v1  1    )                     (     1 v2 v2 v2 )
    //                   ( v1 v2  1 )                     (        1 v3 v3 )
    //                   ( v1 v2 v3 )
    //                   ( v1 v2 v3 )
  
    //  DIRECT = 'B' and STOREV = 'C':         DIRECT = 'B' and STOREV = 'R':
  
    //               V = ( v1 v2 v3 )                 V = ( v1 v1  1       )
    //                   ( v1 v2 v3 )                     ( v2 v2 v2  1    )
    //                   (  1 v2 v3 )                     ( v3 v3 v3 v3  1 )
    //                   (     1 v3 )
    //                   (        1 )
  
    //  =====================================================================
  
    //     .. Parameters ..
    //     ..
    //     .. Local Scalars ..
    //     ..
    //     .. External Subroutines ..
    //     ..
    //     .. External Functions ..
    //     ..
    //     .. Executable Statements ..
  
    //     Quick return if possible
  
    if( n == 0 ) 
        return;
  
    if( lsame( direct, 'F' ) ) { 
        for( i = 1, i_ = i - 1, _do0 = k; i <= _do0; i++, i_++ ) { 
            if( tau[i_] == ZERO ) { 
        
                //              H(i)  =  I
        
                for( j = 1, j_ = j - 1, _do1 = i; j <= _do1; j++, j_++ ) { 
                    T(i_,j_) = ZERO;
                }
            }
            else { 
        
                //              general case
        
                vii = V(i_,i_);
                V(i_,i_) = ONE;
                if( lsame( storev, 'C' ) ) { 
          
                    //                 T(1:i-1,i) := - tau(i) * V(i:n,1:i-1)' * V(i:n,i)
          
                    dgemv( 'T'/* Transpose */, n - i + 1, i - 1, -tau[i_], 
                           &V(0,i_), ldv, &V(i_,i_), 1, ZERO, &T(i_,0), 
                           1 );
                }
                else { 
          
                    //                 T(1:i-1,i) := - tau(i) * V(1:i-1,i:n) * V(i,i:n)'
          
                    dgemv( 'N'/* No transpose */, i - 1, n - i + 1, 
                           -tau[i_], &V(i_,0), ldv, &V(i_,i_), ldv, ZERO, 
                           &T(i_,0), 1 );
                }
                V(i_,i_) = vii;
        
                //              T(1:i-1,i) := T(1:i-1,1:i-1) * T(1:i-1,i)
        
                dtrmv( 'U'/* Upper */, 'N'/* No transpose */, 'N'/* Non-unit */
                       , i - 1, t, ldt, &T(i_,0), 1 );
                T(i_,i_) = tau[i_];
            }
        }
    }
    else { 
        for( i = k, i_ = i - 1; i >= 1; i--, i_-- ) { 
            if( tau[i_] == ZERO ) { 
        
                //              H(i)  =  I
        
                for( j = i, j_ = j - 1, _do2 = k; j <= _do2; j++, j_++ ) { 
                    T(i_,j_) = ZERO;
                }
            }
            else { 
        
                //              general case
        
                if( i < k ) { 
                    if( lsame( storev, 'C' ) ) { 
                        vii = V(i_,n - k + i_);
                        V(i_,n - k + i_) = ONE;
            
                        //                    T(i+1:k,i) :=
                        //                            - tau(i) * V(1:n-k+i,i+1:k)' * V(1:n-k+i,i)
            
                        dgemv( 'T'/* Transpose */, n - k + i, k - i, 
                               -tau[i_], &V(i_ + 1,0), ldv, &V(i_,0), 1, 
                               ZERO, &T(i_,i_ + 1), 1 );
                        V(i_,n - k + i_) = vii;
                    }
                    else { 
                        vii = V(n - k + i_,i_);
                        V(n - k + i_,i_) = ONE;
            
                        //                    T(i+1:k,i) :=
                        //                            - tau(i) * V(i+1:k,1:n-k+i) * V(i,1:n-k+i)'
            
                        dgemv( 'N'/* No transpose */, k - i, n - k + 
                               i, -tau[i_], &V(0,i_ + 1), ldv, &V(0,i_), 
                               ldv, ZERO, &T(i_,i_ + 1), 1 );
                        V(n - k + i_,i_) = vii;
                    }
          
                    //                 T(i+1:k,i) := T(i+1:k,i+1:k) * T(i+1:k,i)
          
                    dtrmv( 'L'/* Lower */, 'N'/* No transpose */, 'N'/* Non-unit */
                           , k - i, &T(i_ + 1,i_ + 1), ldt, &T(i_,i_ + 1), 
                           1 );
                }
                T(i_,i_) = tau[i_];
            }
        }
    }
    return;
  
    //     End of DLARFT
  
#undef  V
#undef  T
} // end of function 


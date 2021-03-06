/*
 * C++ implementation of lapack routine dbdsqr
 *
 * $Id: dbdsqr.cpp,v 1.6 1993/04/06 20:40:17 alv Exp $
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
 * Translated by FOR_C++, v1.1 (P), on 02/18/93 at 07:33:32
 * FOR_C++ Options SET: alloc do=rt no=p pf=dlapack,xlapack,dbla s=dv str=l - prototypes
 *
 * $Log: dbdsqr.cpp,v $
 * Revision 1.6  1993/04/06 20:40:17  alv
 * added const to parameters; added include lapkdefs
 *
 * Revision 1.5  1993/03/19  18:41:23  alv
 * now passes chars explicitly, rather than indirection of a string, to shut up SUN warnings
 *
 * Revision 1.4  1993/03/19  17:18:24  alv
 * added RWLAPKDECL linkage specifier
 *
 * Revision 1.3  1993/03/09  16:13:59  alv
 * added const here and there
 *
 * Revision 1.2  1993/03/05  23:14:20  alv
 * changed ref parms to const ref
 *
 * Revision 1.1  1993/03/03  16:06:22  alv
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

RWLAPKDECL void dbdsqr(const char &uplo, const long &n, const long &ncvt, 
                       const long &nru, const long &ncc, double d[], double e[], double *vt, 
                       const long &ldvt, double *u, const long &ldu, double *c, const long &ldc, 
                       double work[], long &info)
{
#define VT(I_,J_) (*(vt+(I_)*(ldvt)+(J_)))
#define U(I_,J_)  (*(u+(I_)*(ldu)+(J_)))
#define C(I_,J_)  (*(c+(I_)*(ldc)+(J_)))

// PARAMETER translations
    const double ZERO = 0.0e0;
    const double ONE = 1.0e0;
    const double NEGONE = -1.0e0;
    const double HNDRTH = 0.01e0;
    const double TEN = 10.0e0;
    const double HNDRD = 100.0e0;
    const double MEIGTH = -0.125e0;
    const long MAXITR = 6;
// end of PARAMETER translations

    int rotate;
    long _do0, _do1, _do10, _do11, _do12, _do13, _do14, _do15, 
        _do16, _do17, _do2, _do3, _do4, _do5, _do6, _do7, _do8, _do9, 
        i, i_, idir, irot, isub, iter, iuplo, j, j_, job, ll, lll, lll_, 
        m, maxit, nm1, nm12, nm13, oldll, oldm;
    double abse, abss, cosl, cosr, cs, eps, f, g, gap, gmax, h, mu, 
        oldcs, oldsn, r, shift, sigmn, sigmx, sinl, sinr, sll, smax, 
        smin, sminl, sminlo, sminoa, sn, thresh, tol, tolmul, unfl;

  
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
  
    //  DBDSQR computes the singular value decomposition (SVD) of a real
    //  N-by-N (upper or lower) bidiagonal matrix with diagonal D and
    //  offdiagonal E, accumulating the transformations if desired. Letting
    //  B denote the input bidiagonal matrix, the algorithm computes
    //  orthogonal matrices Q and P such that B = Q * S * P' (P' denotes the
    //  transpose of P). The singular values S are overwritten on D.
  
    //  The input matrix U  is changed to U  * Q  if desired.
    //  The input matrix VT is changed to P' * VT if desired.
    //  The input matrix C  is changed to Q' * C  if desired.
  
    //  See "Computing  Small Singular Values of Bidiagonal Matrices With
    //  Guaranteed High Relative Accuracy," by J. Demmel and W. Kahan,
    //  LAPACK Working Note #3, for a detailed description of the algorithm.
  
    //  Arguments
    //  =========
  
    //  UPLO    (input) CHARACTER*1
    //          On entry, UPLO specifies whether the input bidiagonal matrix
    //          is upper or lower bidiagonal.
    //             UPLO = 'U' or 'u'   B is upper bidiagonal.
    //             UPLO = 'L' or 'l'   B is lower bidiagonal.
  
    //  N       (input) INTEGER
    //          On entry, N specifies the number of rows and columns
    //          in the matrix. N must be at least 0.
  
    //  NCVT    (input) INTEGER
    //          On entry, NCVT specifies the number of columns of
    //          the matrix VT. NCVT must be at least 0.
  
    //  NRU     (input) INTEGER
    //          On entry, NRU specifies the number of rows of
    //          the matrix U. NRU must be at least 0.
  
    //  NCC     (input) INTEGER
    //          On entry, NCC specifies the number of columns of
    //          the matrix C. NCC must be at least 0.
  
    //  D       (input/output) DOUBLE PRECISION array, dimension (N)
    //          On entry, D contains the diagonal entries of the
    //          bidiagonal matrix whose SVD is desired. On normal exit,
    //          D contains the singular values in decreasing order.
  
    //  E       (input/output) DOUBLE PRECISION array, dimension (N-1)
    //          On entry, the entries of E contain the
    //          offdiagonal entries of of the bidiagonal matrix
    //          whose SVD is desired. On normal exit, E will contain 0.
    //          If the algorithm does not converge, D and E will contain
    //          the diagonal and superdiagonal entries of a bidiagonal
    //          matrix orthogonally equivalent to the one given as input.
  
    //  VT      (input/output) DOUBLE PRECISION array, dimension (LDVT, NCVT)
    //          On entry, contains an N-by-NCVT matrix which on exit
    //          has been premultiplied by P' (not referenced if NCVT=0).
  
    //  LDVT    (input) INTEGER
    //          On entry, LDVT specifies the leading dimension of VT as
    //          declared in the calling (sub) program. LDVT must be at
    //          least 1. If NCVT is nonzero LDVT must also be at least N.
  
    //  U       (input/output) DOUBLE PRECISION array, dimension (LDU, N)
    //          On entry, contains an NRU-by-N matrix which on exit
    //          has been postmultiplied by Q (not referenced if NRU=0).
  
    //  LDU     (input) INTEGER
    //          On entry, LDU  specifies the leading dimension of U as
    //          declared in the calling (sub) program. LDU must be at
    //          least max( 1, NRU ) .
  
    //  C       (input/output) DOUBLE PRECISION array, dimension (LDC, NCC)
    //          On entry, contains an N-by-NCC matrix which on exit
    //          has been premultiplied by Q' (not referenced if NCC=0).
  
    //  LDC     (input) INTEGER
    //          On entry, LDC  specifies the leading dimension of C as
    //          declared in the calling (sub) program. LDC must be at
    //          least 1. If NCC is nonzero, LDC must also be at least N.
  
    //  WORK    (workspace) DOUBLE PRECISION array, dimension
    //                      (MAX( 1, 4*N-4 ))
    //          Workspace. Only referenced if one of NCVT, NRU, or NCC is
    //          nonzero, and if N is at least 2.
  
    //  INFO    (output) INTEGER
    //          On exit, a value of 0 indicates a successful exit.
    //          If INFO < 0, argument number -INFO is illegal.
    //          If INFO > 0, the algorithm did not converge, and INFO
    //          specifies how many superdiagonals did not converge.
  
    //  Internal Parameters
    //  ===================
  
    //  TOLMUL  DOUBLE PRECISION, default = max(10,min(100,EPS**(-1/8)))
    //          TOLMUL controls the convergence criterion of the QR loop.
    //          If it is positive, TOLMUL*EPS is the desired relative
    //             precision in the computed singular values.
    //          If it is negative, abs(TOLMUL*EPS*sigma_max) is the
    //             desired absolute accuracy in the computed singular
    //             values (corresponds to relative accuracy
    //             abs(TOLMUL*EPS) in the largest singular value.
    //          abs(TOLMUL) should be between 1 and 1/EPS, and preferably
    //             between 10 (for fast convergence) and .1/EPS
    //             (for there to be some accuracy in the results).
    //          Default is to lose at either one eighth or 2 of the
    //             available decimal digits in each computed singular value
    //             (whichever is smaller).
  
    //  MAXITR  INTEGER, default = 6
    //          MAXITR controls the maximum number of passes of the
    //          algorithm through its inner loop. The algorithms stops
    //          (and so fails to converge) if the number of passes
    //          through the inner loop exceeds MAXITR*N**2.
  
    //     .. Parameters ..
    //     ..
    //     .. Local Scalars ..
    //     ..
    //     .. External Functions ..
    //     ..
    //     .. External Subroutines ..
    //     ..
    //     .. Intrinsic Functions ..
    //     ..
    //     .. Executable Statements ..
  
    //     Test the input parameters.
  
    info = 0;
    iuplo = 0;
    if( lsame( uplo, 'U' ) ) 
        iuplo = 1;
    if( lsame( uplo, 'L' ) ) 
        iuplo = 2;
    if( iuplo == 0 ) { 
        info = -1;
    }
    else if( n < 0 ) { 
        info = -2;
    }
    else if( ncvt < 0 ) { 
        info = -3;
    }
    else if( nru < 0 ) { 
        info = -4;
    }
    else if( ncc < 0 ) { 
        info = -5;
    }
    else if( (ncvt == 0 && ldvt < 1) || (ncvt > 0 && ldvt < max( 1, 
                                                                 n )) ) { 
        info = -9;
    }
    else if( ldu < max( 1, nru ) ) { 
        info = -11;
    }
    else if( (ncc == 0 && ldc < 1) || (ncc > 0 && ldc < max( 1, n )) ) { 
        info = -13;
    }
    if( info != 0 ) { 
        xerbla( "DBDSQR", -info );
        return;
    }
    if( n == 0 ) 
        return;
    if( n == 1 ) 
        goto L_190;
  
    //     ROTATE is true if any singular vectors desired, false otherwise
  
    rotate = ((ncvt > 0) || (nru > 0)) || (ncc > 0);
    nm1 = n - 1;
    nm12 = nm1 + nm1;
    nm13 = nm12 + nm1;
  
    //     Get machine constants
  
    eps = dlamch( 'E'/* Epsilon */ );
    unfl = dlamch( 'S'/* Safe minimum */ );
    tolmul = max( TEN, min( HNDRD, pow(eps, MEIGTH) ) );
    tol = tolmul*eps;
  
    //     If matrix lower bidiagonal, rotate to be upper bidiagonal
    //     by applying Givens rotations on the left
  
    if( iuplo == 2 ) { 
        for( i = 1, i_ = i - 1, _do0 = n - 1; i <= _do0; i++, i_++ ) { 
            dlartg( d[i_], e[i_], cs, sn, r );
            d[i_] = r;
            e[i_] = sn*d[i_ + 1];
            d[i_ + 1] = cs*d[i_ + 1];
            if( rotate ) { 
                work[i_] = cs;
                work[nm1 + i_] = sn;
            }
        }
    
        //        Update singular vectors if desired
    
        if( nru > 0 ) 
            dlasr( 'R', 'V', 'F', nru, n, &work[0], &work[n - 1], 
                   u, ldu );
        if( ncc > 0 ) 
            dlasr( 'L', 'V', 'F', n, ncc, &work[0], &work[n - 1], 
                   c, ldc );
    }
  
    //     Compute approximate maximum, minimum singular values
  
    smax = fabs( d[n - 1] );
    for( i = 1, i_ = i - 1, _do1 = n - 1; i <= _do1; i++, i_++ ) { 
        smax = vmax( smax, fabs( d[i_] ), fabs( e[i_] ), FEND );
    }
    sminl = ZERO;
    if( tol >= ZERO ) { 
        sminoa = fabs( d[0] );
        if( sminoa == ZERO ) 
            goto L_40;
        mu = sminoa;
        for( i = 2, i_ = i - 1, _do2 = n; i <= _do2; i++, i_++ ) { 
            mu = fabs( d[i_] )*(mu/(mu + fabs( e[i_ - 1] )));
            sminoa = min( sminoa, mu );
            if( sminoa == ZERO ) 
                goto L_40;
        }
    L_40:
        ;
        sminoa = sminoa/sqrt( (double)( n ) );
    }
  
    //     Prepare for main iteration loop for the singular values
  
    maxit = MAXITR*n*n;
    iter = 0;
    oldll = -1;
    oldm = -1;
    if( (ncc == 0 && nru == 0) && ncvt == 0 ) { 
    
        //        No singular vectors desired
    
        job = 0;
    }
    else { 
    
        //        Singular vectors desired
    
        job = 1;
    }
    if( tol >= ZERO ) { 
    
        //        Relative accuracy desired
    
        thresh = max( tol*sminoa, maxit*unfl );
    }
    else { 
    
        //        Absolute accuracy desired
    
        thresh = max( fabs( tol )*smax, maxit*unfl );
    }
  
    //     M points to last entry of unconverged part of matrix
  
    m = n;
  
    //     Begin main iteration loop
  
L_50:
    ;
  
    //     Check for convergence or exceeding iteration count
  
    if( m <= 1 ) 
        goto L_190;
    if( iter > maxit ) 
        goto L_230;
  
    //     Find diagonal block of matrix to work on
  
    if( tol < ZERO && fabs( d[m - 1] ) <= thresh ) 
        d[m - 1] = ZERO;
    smax = fabs( d[m - 1] );
    smin = smax;
    for( lll = 1, lll_ = lll - 1, _do3 = m; lll <= _do3; lll++, lll_++ ) { 
        ll = m - lll;
        if( ll == 0 ) 
            goto L_80;
        abss = abs( d[ll - 1] );
        abse = abs( e[ll - 1] );
        if( tol < ZERO && abss <= thresh ) 
            d[ll - 1] = ZERO;
        if( abse <= thresh ) 
            goto L_70;
        smin = min( smin, abss );
        smax = vmax( smax, abss, abse, FEND );
    }
L_70:
    ;
    e[ll - 1] = ZERO;
  
    //     Matrix splits since E(LL) = 0
  
    if( ll == m - 1 ) { 
    
        //        Convergence of bottom singular value, return to top of loop
    
        m = m - 1;
        goto L_50;
    }
L_80:
    ;
    ll = ll + 1;
  
    //     E(LL) through E(M-1) are nonzero, E(LL-1) is zero
  
    if( ll == m - 1 ) { 
    
        //        2 by 2 block, handle separately
    
        dlasv2( d[m - 2], e[m - 2], d[m - 1], sigmn, sigmx, sinr, 
                cosr, sinl, cosl );
        d[m - 2] = sigmx;
        e[m - 2] = ZERO;
        d[m - 1] = sigmn;
    
        //        Compute singular vectors, if desired
    
        if( ncvt > 0 ) 
            drot( ncvt, &VT(0,m - 2), ldvt, &VT(0,m - 1), ldvt, cosr, 
                  sinr );
        if( nru > 0 ) 
            drot( nru, &U(m - 2,0), 1, &U(m - 1,0), 1, cosl, sinl );
        if( ncc > 0 ) 
            drot( ncc, &C(0,m - 2), ldc, &C(0,m - 1), ldc, cosl, sinl );
        m = m - 2;
        goto L_50;
    }
  
    //     If working on new submatrix, choose shift direction
    //     (from larger end diagonal entry towards smaller)
  
    if( ll > oldm || m < oldll ) { 
        if( fabs( d[ll - 1] ) >= fabs( d[m - 1] ) ) { 
      
            //           Chase bulge from top (big end) to bottom (small end)
      
            idir = 1;
        }
        else { 
      
            //           Chase bulge from bottom (big end) to top (small end)
      
            idir = 2;
        }
    }
  
    //     Apply convergence tests
  
    if( idir == 1 ) { 
    
        //        Run convergence test in forward direction
        //        First apply standard test to bottom of matrix
    
        if( fabs( e[m - 2] ) <= fabs( tol )*fabs( d[m - 1] ) || (tol < 
                                                                 ZERO && fabs( e[m - 2] ) <= thresh) ) { 
            e[m - 2] = ZERO;
            goto L_50;
        }
    
        if( tol >= ZERO ) { 
      
            //           If relative accuracy desired,
            //           apply convergence criterion forward
      
            mu = fabs( d[ll - 1] );
            sminl = mu;
            for( lll = ll, lll_ = lll - 1, _do4 = m - 1; lll <= _do4; lll++, lll_++ ) { 
                if( fabs( e[lll_] ) <= tol*mu ) { 
                    e[lll_] = ZERO;
                    goto L_50;
                }
                sminlo = sminl;
                mu = fabs( d[lll_ + 1] )*(mu/(mu + fabs( e[lll_] )));
                sminl = min( sminl, mu );
            }
      
            //           If singular values only wanted, apply gap test to bottom
            //           end of matrix
      
            if( job == 0 ) { 
                gap = sminlo/sqrt( (double)( m - ll ) ) - fabs( d[m - 1] );
                if( gap > ZERO ) { 
                    abss = fabs( d[m - 1] );
                    abse = fabs( e[m - 2] );
                    gmax = vmax( gap, abss, abse, FEND );
                    if( pow(abse/gmax, 2) <= tol*(gap/gmax)*(abss/
                                                             gmax) ) { 
                        e[m - 2] = ZERO;
                        goto L_50;
                    }
                }
            }
        }
    }
    else { 
    
        //        Run convergence test in backward direction
        //        First apply standard test to top of matrix
    
        if( fabs( e[ll - 1] ) <= fabs( tol )*fabs( d[ll - 1] ) || (tol < 
                                                                   ZERO && fabs( e[ll - 1] ) <= thresh) ) { 
            e[ll - 1] = ZERO;
            goto L_50;
        }
    
        if( tol >= ZERO ) { 
      
            //           If relative accuracy desired,
            //           apply convergence criterion backward
      
            mu = fabs( d[m - 1] );
            sminl = mu;
            for( lll = m - 1, lll_ = lll - 1, _do5 = ll; lll >= _do5; lll--, lll_-- ) { 
                if( fabs( e[lll_] ) <= tol*mu ) { 
                    e[lll_] = ZERO;
                    goto L_50;
                }
                sminlo = sminl;
                mu = fabs( d[lll_] )*(mu/(mu + fabs( e[lll_] )));
                sminl = min( sminl, mu );
            }
      
            //           If singular values only wanted, apply gap test to top
            //           end of matrix
      
            if( job == 0 ) { 
                gap = sminlo/sqrt( (double)( m - ll ) ) - fabs( d[ll - 1] );
                if( gap > ZERO ) { 
                    abss = fabs( d[ll - 1] );
                    abse = fabs( e[ll - 1] );
                    gmax = vmax( gap, abss, abse, FEND );
                    if( pow(abse/gmax, 2) <= tol*(gap/gmax)*(abss/
                                                             gmax) ) { 
                        e[ll - 1] = ZERO;
                        goto L_50;
                    }
                }
            }
        }
    }
    oldll = ll;
    oldm = m;
  
    //     Compute shift.  First, test if shifting would ruin relative
    //     accuracy, and if so set the shift to zero.
  
    if( tol >= ZERO && n*tol*(sminl/smax) <= max( eps, HNDRTH*tol ) ) { 
    
        //        Use a zero shift to avoid loss of relative accuracy
    
        shift = ZERO;
    }
    else { 
    
        //        Compute the shift from 2-by-2 block at end of matrix
    
        if( idir == 1 ) { 
            sll = fabs( d[ll - 1] );
            dlas2( d[m - 2], e[m - 2], d[m - 1], shift, r );
        }
        else { 
            sll = fabs( d[m - 1] );
            dlas2( d[ll - 1], e[ll - 1], d[ll], shift, r );
        }
    
        //        Test if shift negligible, and if so set to zero
    
        if( sll > ZERO ) { 
            if( pow(shift/sll, 2) < eps ) 
                shift = ZERO;
        }
    }
  
    //     Increment iteration count
  
    iter = iter + m - ll;
  
    //     If SHIFT = 0, do simplified QR iteration
  
    if( shift == ZERO ) { 
        if( idir == 1 ) { 
      
            //           Chase bulge from top to bottom
      
            cs = ONE;
            oldcs = ONE;
      
            //           Save cosines and sines if singular vectors desired
      
            if( rotate ) { 
        
                dlartg( d[ll - 1]*cs, e[ll - 1], cs, sn, r );
                dlartg( oldcs*r, d[ll]*sn, oldcs, oldsn, d[ll - 1] );
                work[0] = cs;
                work[nm1] = sn;
                work[nm12] = oldcs;
                work[nm13] = oldsn;
                irot = 1;
                for( i = ll + 1, i_ = i - 1, _do6 = m - 1; i <= _do6; i++, i_++ ) { 
                    dlartg( d[i_]*cs, e[i_], cs, sn, r );
                    e[i_ - 1] = oldsn*r;
                    dlartg( oldcs*r, d[i_ + 1]*sn, oldcs, oldsn, d[i_] );
                    irot = irot + 1;
                    work[irot - 1] = cs;
                    work[irot + nm1 - 1] = sn;
                    work[irot + nm12 - 1] = oldcs;
                    work[irot + nm13 - 1] = oldsn;
                }
                h = d[m - 1]*cs;
                d[m - 1] = h*oldcs;
                e[m - 2] = h*oldsn;
        
                //              Update singular vectors
        
                if( ncvt > 0 ) 
                    dlasr( 'L', 'V', 'F', m - ll + 1, ncvt, &work[0], 
                           &work[n - 1], &VT(0,ll - 1), ldvt );
                if( nru > 0 ) 
                    dlasr( 'R', 'V', 'F', nru, m - ll + 1, &work[nm12], 
                           &work[nm13], &U(ll - 1,0), ldu );
                if( ncc > 0 ) 
                    dlasr( 'L', 'V', 'F', m - ll + 1, ncc, &work[nm12], 
                           &work[nm13], &C(0,ll - 1), ldc );
        
            }
            else { 
        
                dlartg( d[ll - 1]*cs, e[ll - 1], cs, sn, r );
                dlartg( oldcs*r, d[ll]*sn, oldcs, oldsn, d[ll - 1] );
                for( i = ll + 1, i_ = i - 1, _do7 = m - 1; i <= _do7; i++, i_++ ) { 
                    dlartg( d[i_]*cs, e[i_], cs, sn, r );
                    e[i_ - 1] = oldsn*r;
                    dlartg( oldcs*r, d[i_ + 1]*sn, oldcs, oldsn, d[i_] );
                }
                h = d[m - 1]*cs;
                d[m - 1] = h*oldcs;
                e[m - 2] = h*oldsn;
        
            }
      
            //           Test convergence
      
            if( fabs( e[m - 2] ) <= thresh ) 
                e[m - 2] = ZERO;
      
        }
        else { 
      
            //           Chase bulge from bottom to top
      
            cs = ONE;
            oldcs = ONE;
      
            //           Save cosines and sines if singular vectors desired
      
            if( rotate ) { 
        
                dlartg( d[m - 1]*cs, e[m - 2], cs, sn, r );
                dlartg( oldcs*r, d[m - 2]*sn, oldcs, oldsn, d[m - 1] );
                work[m - ll - 1] = cs;
                work[m - ll + nm1 - 1] = -sn;
                work[m - ll + nm12 - 1] = oldcs;
                work[m - ll + nm13 - 1] = -oldsn;
                irot = m - ll;
                for( i = m - 1, i_ = i - 1, _do8 = ll + 1; i >= _do8; i--, i_-- ) { 
                    dlartg( d[i_]*cs, e[i_ - 1], cs, sn, r );
                    e[i_] = oldsn*r;
                    dlartg( oldcs*r, d[i_ - 1]*sn, oldcs, oldsn, d[i_] );
                    irot = irot - 1;
                    work[irot - 1] = cs;
                    work[irot + nm1 - 1] = -sn;
                    work[irot + nm12 - 1] = oldcs;
                    work[irot + nm13 - 1] = -oldsn;
                }
                h = d[ll - 1]*cs;
                d[ll - 1] = h*oldcs;
                e[ll - 1] = h*oldsn;
        
                //              Update singular vectors
        
                if( ncvt > 0 ) 
                    dlasr( 'L', 'V', 'B', m - ll + 1, ncvt, &work[nm12], 
                           &work[nm13], &VT(0,ll - 1), ldvt );
                if( nru > 0 ) 
                    dlasr( 'R', 'V', 'B', nru, m - ll + 1, &work[0], 
                           &work[n - 1], &U(ll - 1,0), ldu );
                if( ncc > 0 ) 
                    dlasr( 'L', 'V', 'B', m - ll + 1, ncc, &work[0], 
                           &work[n - 1], &C(0,ll - 1), ldc );
        
            }
            else { 
        
                dlartg( d[m - 1]*cs, e[m - 2], cs, sn, r );
                dlartg( oldcs*r, d[m - 2]*sn, oldcs, oldsn, d[m - 1] );
                for( i = m - 1, i_ = i - 1, _do9 = ll + 1; i >= _do9; i--, i_-- ) { 
                    dlartg( d[i_]*cs, e[i_ - 1], cs, sn, r );
                    e[i_] = oldsn*r;
                    dlartg( oldcs*r, d[i_ - 1]*sn, oldcs, oldsn, d[i_] );
                }
                h = d[ll - 1]*cs;
                d[ll - 1] = h*oldcs;
                e[ll - 1] = h*oldsn;
        
            }
      
            //           Test convergence
      
            if( fabs( e[ll - 1] ) <= thresh ) 
                e[ll - 1] = ZERO;
        }
    }
    else { 
    
        //        Use nonzero shift
    
        if( idir == 1 ) { 
      
            //           Chase bulge from top to bottom
      
            f = (fabs( d[ll - 1] ) - shift)*(sign( ONE, d[ll - 1] ) + 
                                             shift/d[ll - 1]);
            g = e[ll - 1];
      
            //           Save cosines and sines if singular vectors desired
      
            if( rotate ) { 
        
                dlartg( f, g, cosr, sinr, r );
                f = cosr*d[ll - 1] + sinr*e[ll - 1];
                e[ll - 1] = cosr*e[ll - 1] - sinr*d[ll - 1];
                g = sinr*d[ll];
                d[ll] = cosr*d[ll];
                dlartg( f, g, cosl, sinl, r );
                d[ll - 1] = r;
                f = cosl*e[ll - 1] + sinl*d[ll];
                d[ll] = cosl*d[ll] - sinl*e[ll - 1];
                g = sinl*e[ll];
                e[ll] = cosl*e[ll];
                work[0] = cosr;
                work[nm1] = sinr;
                work[nm12] = cosl;
                work[nm13] = sinl;
                irot = 1;
                for( i = ll + 1, i_ = i - 1, _do10 = m - 2; i <= _do10; i++, i_++ ) { 
                    dlartg( f, g, cosr, sinr, r );
                    e[i_ - 1] = r;
                    f = cosr*d[i_] + sinr*e[i_];
                    e[i_] = cosr*e[i_] - sinr*d[i_];
                    g = sinr*d[i_ + 1];
                    d[i_ + 1] = cosr*d[i_ + 1];
                    dlartg( f, g, cosl, sinl, r );
                    d[i_] = r;
                    f = cosl*e[i_] + sinl*d[i_ + 1];
                    d[i_ + 1] = cosl*d[i_ + 1] - sinl*e[i_];
                    g = sinl*e[i_ + 1];
                    e[i_ + 1] = cosl*e[i_ + 1];
                    irot = irot + 1;
                    work[irot - 1] = cosr;
                    work[irot + nm1 - 1] = sinr;
                    work[irot + nm12 - 1] = cosl;
                    work[irot + nm13 - 1] = sinl;
                }
                dlartg( f, g, cosr, sinr, r );
                e[m - 3] = r;
                f = cosr*d[m - 2] + sinr*e[m - 2];
                e[m - 2] = cosr*e[m - 2] - sinr*d[m - 2];
                g = sinr*d[m - 1];
                d[m - 1] = cosr*d[m - 1];
                dlartg( f, g, cosl, sinl, r );
                d[m - 2] = r;
                f = cosl*e[m - 2] + sinl*d[m - 1];
                d[m - 1] = cosl*d[m - 1] - sinl*e[m - 2];
                irot = irot + 1;
                work[irot - 1] = cosr;
                work[irot + nm1 - 1] = sinr;
                work[irot + nm12 - 1] = cosl;
                work[irot + nm13 - 1] = sinl;
                e[m - 2] = f;
        
                //              Update singular vectors
        
                if( ncvt > 0 ) 
                    dlasr( 'L', 'V', 'F', m - ll + 1, ncvt, &work[0], 
                           &work[n - 1], &VT(0,ll - 1), ldvt );
                if( nru > 0 ) 
                    dlasr( 'R', 'V', 'F', nru, m - ll + 1, &work[nm12], 
                           &work[nm13], &U(ll - 1,0), ldu );
                if( ncc > 0 ) 
                    dlasr( 'L', 'V', 'F', m - ll + 1, ncc, &work[nm12], 
                           &work[nm13], &C(0,ll - 1), ldc );
        
            }
            else { 
        
                dlartg( f, g, cosr, sinr, r );
                f = cosr*d[ll - 1] + sinr*e[ll - 1];
                e[ll - 1] = cosr*e[ll - 1] - sinr*d[ll - 1];
                g = sinr*d[ll];
                d[ll] = cosr*d[ll];
                dlartg( f, g, cosl, sinl, r );
                d[ll - 1] = r;
                f = cosl*e[ll - 1] + sinl*d[ll];
                d[ll] = cosl*d[ll] - sinl*e[ll - 1];
                g = sinl*e[ll];
                e[ll] = cosl*e[ll];
                for( i = ll + 1, i_ = i - 1, _do11 = m - 2; i <= _do11; i++, i_++ ) { 
                    dlartg( f, g, cosr, sinr, r );
                    e[i_ - 1] = r;
                    f = cosr*d[i_] + sinr*e[i_];
                    e[i_] = cosr*e[i_] - sinr*d[i_];
                    g = sinr*d[i_ + 1];
                    d[i_ + 1] = cosr*d[i_ + 1];
                    dlartg( f, g, cosl, sinl, r );
                    d[i_] = r;
                    f = cosl*e[i_] + sinl*d[i_ + 1];
                    d[i_ + 1] = cosl*d[i_ + 1] - sinl*e[i_];
                    g = sinl*e[i_ + 1];
                    e[i_ + 1] = cosl*e[i_ + 1];
                }
                dlartg( f, g, cosr, sinr, r );
                e[m - 3] = r;
                f = cosr*d[m - 2] + sinr*e[m - 2];
                e[m - 2] = cosr*e[m - 2] - sinr*d[m - 2];
                g = sinr*d[m - 1];
                d[m - 1] = cosr*d[m - 1];
                dlartg( f, g, cosl, sinl, r );
                d[m - 2] = r;
                f = cosl*e[m - 2] + sinl*d[m - 1];
                d[m - 1] = cosl*d[m - 1] - sinl*e[m - 2];
                e[m - 2] = f;
        
            }
      
            //           Test convergence
      
            if( fabs( e[m - 2] ) <= thresh ) 
                e[m - 2] = ZERO;
      
        }
        else { 
      
            //           Chase bulge from bottom to top
      
            f = (fabs( d[m - 1] ) - shift)*(sign( ONE, d[m - 1] ) + 
                                            shift/d[m - 1]);
            g = e[m - 2];
      
            //           Save cosines and sines if singular vectors desired
      
            if( rotate ) { 
        
                dlartg( f, g, cosr, sinr, r );
                f = cosr*d[m - 1] + sinr*e[m - 2];
                e[m - 2] = cosr*e[m - 2] - sinr*d[m - 1];
                g = sinr*d[m - 2];
                d[m - 2] = cosr*d[m - 2];
                dlartg( f, g, cosl, sinl, r );
                d[m - 1] = r;
                f = cosl*e[m - 2] + sinl*d[m - 2];
                d[m - 2] = cosl*d[m - 2] - sinl*e[m - 2];
                g = sinl*e[m - 3];
                e[m - 3] = cosl*e[m - 3];
                work[m - ll - 1] = cosr;
                work[m - ll + nm1 - 1] = -sinr;
                work[m - ll + nm12 - 1] = cosl;
                work[m - ll + nm13 - 1] = -sinl;
                irot = m - ll;
                for( i = m - 1, i_ = i - 1, _do12 = ll + 2; i >= _do12; i--, i_-- ) { 
                    dlartg( f, g, cosr, sinr, r );
                    e[i_] = r;
                    f = cosr*d[i_] + sinr*e[i_ - 1];
                    e[i_ - 1] = cosr*e[i_ - 1] - sinr*d[i_];
                    g = sinr*d[i_ - 1];
                    d[i_ - 1] = cosr*d[i_ - 1];
                    dlartg( f, g, cosl, sinl, r );
                    d[i_] = r;
                    f = cosl*e[i_ - 1] + sinl*d[i_ - 1];
                    d[i_ - 1] = cosl*d[i_ - 1] - sinl*e[i_ - 1];
                    g = sinl*e[i_ - 2];
                    e[i_ - 2] = cosl*e[i_ - 2];
                    irot = irot - 1;
                    work[irot - 1] = cosr;
                    work[irot + nm1 - 1] = -sinr;
                    work[irot + nm12 - 1] = cosl;
                    work[irot + nm13 - 1] = -sinl;
                }
                dlartg( f, g, cosr, sinr, r );
                e[ll] = r;
                f = cosr*d[ll] + sinr*e[ll - 1];
                e[ll - 1] = cosr*e[ll - 1] - sinr*d[ll];
                g = sinr*d[ll - 1];
                d[ll - 1] = cosr*d[ll - 1];
                dlartg( f, g, cosl, sinl, r );
                d[ll] = r;
                f = cosl*e[ll - 1] + sinl*d[ll - 1];
                d[ll - 1] = cosl*d[ll - 1] - sinl*e[ll - 1];
                irot = irot - 1;
                work[irot - 1] = cosr;
                work[irot + nm1 - 1] = -sinr;
                work[irot + nm12 - 1] = cosl;
                work[irot + nm13 - 1] = -sinl;
                e[ll - 1] = f;
        
            }
            else { 
        
                dlartg( f, g, cosr, sinr, r );
                f = cosr*d[m - 1] + sinr*e[m - 2];
                e[m - 2] = cosr*e[m - 2] - sinr*d[m - 1];
                g = sinr*d[m - 2];
                d[m - 2] = cosr*d[m - 2];
                dlartg( f, g, cosl, sinl, r );
                d[m - 1] = r;
                f = cosl*e[m - 2] + sinl*d[m - 2];
                d[m - 2] = cosl*d[m - 2] - sinl*e[m - 2];
                g = sinl*e[m - 3];
                e[m - 3] = cosl*e[m - 3];
                for( i = m - 1, i_ = i - 1, _do13 = ll + 2; i >= _do13; i--, i_-- ) { 
                    dlartg( f, g, cosr, sinr, r );
                    e[i_] = r;
                    f = cosr*d[i_] + sinr*e[i_ - 1];
                    e[i_ - 1] = cosr*e[i_ - 1] - sinr*d[i_];
                    g = sinr*d[i_ - 1];
                    d[i_ - 1] = cosr*d[i_ - 1];
                    dlartg( f, g, cosl, sinl, r );
                    d[i_] = r;
                    f = cosl*e[i_ - 1] + sinl*d[i_ - 1];
                    d[i_ - 1] = cosl*d[i_ - 1] - sinl*e[i_ - 1];
                    g = sinl*e[i_ - 2];
                    e[i_ - 2] = cosl*e[i_ - 2];
                }
                dlartg( f, g, cosr, sinr, r );
                e[ll] = r;
                f = cosr*d[ll] + sinr*e[ll - 1];
                e[ll - 1] = cosr*e[ll - 1] - sinr*d[ll];
                g = sinr*d[ll - 1];
                d[ll - 1] = cosr*d[ll - 1];
                dlartg( f, g, cosl, sinl, r );
                d[ll] = r;
                f = cosl*e[ll - 1] + sinl*d[ll - 1];
                d[ll - 1] = cosl*d[ll - 1] - sinl*e[ll - 1];
                e[ll - 1] = f;
        
            }
      
            //           Test convergence
      
            if( fabs( e[ll - 1] ) <= thresh ) 
                e[ll - 1] = ZERO;
      
            //           Update singular vectors if desired
      
            if( ncvt > 0 ) 
                dlasr( 'L', 'V', 'B', m - ll + 1, ncvt, &work[nm12], 
                       &work[nm13], &VT(0,ll - 1), ldvt );
            if( nru > 0 ) 
                dlasr( 'R', 'V', 'B', nru, m - ll + 1, &work[0], &work[n - 1], 
                       &U(ll - 1,0), ldu );
            if( ncc > 0 ) 
                dlasr( 'L', 'V', 'B', m - ll + 1, ncc, &work[0], &work[n - 1], 
                       &C(0,ll - 1), ldc );
        }
    }
  
    //     QR iteration finished, go back and check convergence
  
    goto L_50;
  
    //     All singular values converged, so make them positive
  
L_190:
    ;
    for( i = 1, i_ = i - 1, _do14 = n; i <= _do14; i++, i_++ ) { 
        if( d[i_] < ZERO ) { 
            d[i_] = -d[i_];
      
            //           Change sign of singular vectors, if desired
      
            if( ncvt > 0 ) 
                dscal( ncvt, NEGONE, &VT(0,i_), ldvt );
        }
    }
  
    //     Sort the singular values into decreasing order (insertion sort on
    //     singular values, but only one transposition per singular vector)
  
    for( i = 1, i_ = i - 1, _do15 = n - 1; i <= _do15; i++, i_++ ) { 
    
        //        Scan for smallest D(I)
    
        isub = 1;
        smin = d[0];
        for( j = 2, j_ = j - 1, _do16 = n + 1 - i; j <= _do16; j++, j_++ ) { 
            if( d[j_] <= smin ) { 
                isub = j;
                smin = d[j_];
            }
        }
        if( isub != n + 1 - i ) { 
      
            //           Swap singular values and vectors
      
            d[isub - 1] = d[n - i];
            d[n - i] = smin;
            if( ncvt > 0 ) 
                dswap( ncvt, &VT(0,isub - 1), ldvt, &VT(0,n - i), 
                       ldvt );
            if( nru > 0 ) 
                dswap( nru, &U(isub - 1,0), 1, &U(n - i,0), 1 );
            if( ncc > 0 ) 
                dswap( ncc, &C(0,isub - 1), ldc, &C(0,n - i), ldc );
        }
    }
    goto L_250;
  
    //     Maximum number of iterations exceeded, failure to converge
  
L_230:
    ;
    info = 0;
    for( i = 1, i_ = i - 1, _do17 = n - 1; i <= _do17; i++, i_++ ) { 
        if( e[i_] != ZERO ) 
            info = info + 1;
    }
L_250:
    ;
    return;
  
    //     End of DBDSQR
  
#undef  VT
#undef  U
#undef  C
} // end of function 


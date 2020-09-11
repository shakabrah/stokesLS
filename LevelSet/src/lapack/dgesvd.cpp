/*
 * C++ implementation of lapack routine dgesvd
 *
 * $Id: dgesvd.cpp,v 1.5 1993/04/06 20:40:38 alv Exp $
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
 * Translated by FOR_C++, v1.1 (P), on 02/18/93 at 07:34:27
 * FOR_C++ Options SET: alloc do=rt no=p pf=dlapack,xlapack,dbla s=dv str=l - prototypes
 *
 * $Log: dgesvd.cpp,v $
 * Revision 1.5  1993/04/06 20:40:38  alv
 * added const to parameters; added include lapkdefs
 *
 * Revision 1.4  1993/03/19  17:18:24  alv
 * added RWLAPKDECL linkage specifier
 *
 * Revision 1.3  1993/03/09  16:14:40  alv
 * made parms const
 *
 * Revision 1.2  1993/03/05  23:14:52  alv
 * changed ref parms to const ref
 *
 * Revision 1.1  1993/03/03  16:06:46  alv
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

RWLAPKDECL void /*FUNCTION*/ dgesvd(const char &jobu, const char &jobvt, const long &m, const long &n, 
                                    double *a, const long &lda, double s[], double *u, const long &ldu, double *vt, 
                                    const long &ldvt, double work[], const long &lwork, long &info)
{
#define A(I_,J_)  (*(a+(I_)*(lda)+(J_)))
#define U(I_,J_)  (*(u+(I_)*(ldu)+(J_)))
#define VT(I_,J_) (*(vt+(I_)*(ldvt)+(J_)))
// PARAMETER translations
    const double ZERO = 0.0e0;
    const double ONE = 1.0e0;
// end of PARAMETER translations

    char _c0[2], _c1[2];
    int wntua, wntuas, wntun, wntuo, wntus, wntva, wntvas, wntvn, 
        wntvo, wntvs;
    long _do0, _do1, _do2, _do3, _do4, _do5, _do6, _do7, blk, 
        chunk, i, i_, ie, ir, iscl, itau, itaup, itauq, iu, iwork, ldwrkr, 
        ldwrku, maxwrk, minmn, minwrk, mnthr, ncu, ncvt, nru, nrvt, wrkbl;
    double anrm, bignum, dum[1], eps, smlnum;
#define NCHRTMPS 1
    CHRTMP _c[NCHRTMPS];
    ini_chrtmp(_c,NCHRTMPS);

  
    //  -- LAPACK driver routine (version 1.0) --
    //     Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,
    //     Courant Institute, Argonne National Lab, and Rice University
    //     February 29, 1992
  
    //     .. Scalar Arguments ..
    //     ..
    //     .. Array Arguments ..
    //     ..
  
    //  Purpose
    //  =======
  
    //  DGESVD computes the singular value decomposition (SVD) of a real
    //  M-by-N matrix A, optionally computing the left and/or right singular
    //  vectors. The SVD is written
  
    //       A = U * SIGMA * transpose(V)
  
    //  where SIGMA is a diagonal matrix with the singular values on the
    //  diagonal and U and V are orthogonal. The columns of U are the left
    //  singular vectors and the columns of V are the right singular vectors.
    //  If the right singular vectors are requested, VT = transpose(V) is
    //  returned by the routine.
  
    //  The diagonal entries of SIGMA are returned in S:
    //  S(1) >= ... >= S(MINMN) >= 0, where MINMN=MIN(M,N).
  
    //  Depending on JOBU all M left singular vectors may be computed
    //  and stored in U, min(M,N) left singular vectors may be computed and
    //  stored in U or overwritten on A, or no vectors may be computed.
  
    //  Depending on JOBVT all N right singular vectors may be computed
    //  and stored in VT, min(M,N) right singular vectors may be computed and
    //  stored in VT or overwritten on A, or no vectors may be computed.
  
    //  Arguments
    //  =========
  
    //  JOBU    (input) CHARACTER*1
    //          JOBU specifies which left singular vectors to compute. If
    //             JOBU = 'A'  all M M-dimensional left singular
    //                         vectors are returned in U.
    //             JOBU = 'S'  only MIN(M,N) M-dimensional left
    //                         singular vectors are returned in U.
    //             JOBU = 'O'  only MIN(M,N) M-dimensional left
    //                         singular vectors are overwritten on A.
    //             JOBU = 'N'  no left singular vectors are computed.
  
    //  JOBVT   (input) CHARACTER*1
    //          JOBVT specifies which right singular vectors to compute. If
    //             JOBVT = 'A' all N N-dimensional right singular
    //                         vectors are returned in VT.
    //             JOBVT = 'S' only MIN(M,N) N-dimensional right
    //                         singular vectors are returned in VT.
    //             JOBVT = 'O' only MIN(M,N) N-dimensional right
    //                         singular vectors are overwritten on A.
    //             JOBVT = 'N' no right singular vectors are computed.
  
    //          JOBVT and JOBU cannot simultaneously be 'O'.
  
    //  M       (input) INTEGER
    //          The number of rows of the input matrix A.  M >= 0.
  
    //  N       (input) INTEGER
    //          The number of columns of the input matrix A.  N >= 0.
  
    //  A       (input/output) DOUBLE PRECISION array, dimension (LDA,N)
    //          On input, A is the matrix whose SVD is desired.
    //          On output,
    //          if JOBU = 'O',  A has been overwritten with
    //                          min(M,N) left singular vectors;
    //          if JOBVT = 'O', A has been overwritten with
    //                          min(M,N) right singular vectors.
  
    //  LDA     (input) INTEGER
    //          The leading dimension of the array A.  LDA >= max(1,M).
  
    //  S       (output) DOUBLE PRECISION array, dimension (min(M,N))
    //          The singular values of A, sorted so that S(i) >= S(i+1).
  
    //  U       (output) DOUBLE PRECISION array, dimension (LDU,UCOL)
    //          where UCOL is defined as follows.
    //          U contains the computed left singular vectors.
    //          U has M rows and UCOL=M columns if JOBU = 'A'.
    //          U has M rows and UCOL=min(M,N) columns if JOBU = 'S'.
    //          U is not referenced if JOBU = 'N' or 'O'.
  
    //  LDU     (input) INTEGER
    //          The leading dimension of the array U.  LDU >= 1, and
    //          LDU >= M if JOBU = 'S' or 'A'.
  
    //  VT      (output) DOUBLE PRECISION array, dimension (LDVT,N)
    //          VT contains the transpose of the computed left singular
    //             vectors.
    //          VT has N rows and N columns if JOBVT = 'A'.
    //          VT has min(M,N) rows and N columns if JOBVT = 'S'.
    //          VT is not referenced if JOBVT = 'N' or 'O'.
  
    //  LDVT    (input) INTEGER
    //          The leading dimension of the array VT.  LDVT >= 1, and
    //          LDVT >= min(M,N) if JOBVT = 'S', or LDVT >= N if JOBVT = 'A'.
  
    //  WORK    (workspace/output) DOUBLE PRECISION array, dimension (LWORK)
    //          On exit, WORK(1) contains the optimal workspace size LWORK
    //          for high performance.
  
    //  LWORK   (input) INTEGER
    //          The dimension of the array WORK. LWORK must be at least 1.
    //          The exact minimum amount of workspace needed depends
    //          on JOBU, JOBVT, M and N. As long as LWORK is at least
    //          MAX(3*MIN(M,N)+MAX(M,N),5*MIN(M,N)-4), then the code
    //          will execute correctly.
    //          For good performance, LWORK should generally be larger.
    //          The optimum value of LWORK for high performance is
    //          returned in WORK(1).
  
    //  INFO    (output) INTEGER
    //          0  - successful exit
    //          <0 - if INFO = -i, the i-th argument had an illegal value.
    //          >0 - if DBDSQR did not converge, INFO specifies how many
    //               superdiagonals of an intermediate bidiagonal form
    //               did not converge to zero.
  
    //  =====================================================================
  
    //     .. Parameters ..
    //     ..
    //     .. Local Scalars ..
    //     ..
    //     .. Local Arrays ..
    //     ..
    //     .. External Subroutines ..
    //     ..
    //     .. External Functions ..
    //     ..
    //     .. Intrinsic Functions ..
    //     ..
    //     .. Executable Statements ..
  
    //     Test the input arguments
  
    info = 0;
    minmn = min( m, n );
    mnthr = ilaenv( 6, "DGESVD", f_concat(&_c[0],STR1(_c0,jobu),STR1(_c1,jobvt),
                                          (long int)(NULL)), m, n, 0, 0 );
    wntua = lsame( jobu, 'A' );
    wntus = lsame( jobu, 'S' );
    wntuas = wntua || wntus;
    wntuo = lsame( jobu, 'O' );
    wntun = lsame( jobu, 'N' );
    wntva = lsame( jobvt, 'A' );
    wntvs = lsame( jobvt, 'S' );
    wntvas = wntva || wntvs;
    wntvo = lsame( jobvt, 'O' );
    wntvn = lsame( jobvt, 'N' );
    minwrk = 1;
  
    if( !(((wntua || wntus) || wntuo) || wntun) ) { 
        info = -1;
    }
    else if( !(((wntva || wntvs) || wntvo) || wntvn) || (wntvo && 
                                                         wntuo) ) { 
        info = -2;
    }
    else if( m < 0 ) { 
        info = -3;
    }
    else if( n < 0 ) { 
        info = -4;
    }
    else if( lda < max( 1, m ) ) { 
        info = -6;
    }
    else if( ldu < 1 || (wntuas && ldu < m) ) { 
        info = -9;
    }
    else if( (ldvt < 1 || (wntva && ldvt < n)) || (wntvs && ldvt < 
                                                   minmn) ) { 
        info = -11;
    }
  
    //     Compute workspace
    //      (Note: Comments in the code beginning "Workspace:" describe the
    //       minimal amount of workspace needed at that point in the code,
    //       as well as the preferred amount for good performance.
    //       NB refers to the optimal block size for the immediately
    //       following subroutine, as returned by ILAENV.)
  
    if( ((info == 0 && lwork >= 1) && m > 0) && n > 0 ) { 
        if( m >= n ) { 
            if( m >= mnthr ) { 
                if( wntun ) { 
          
                    //                 Path 1 (M much larger than N, JOBU='N')
          
                    maxwrk = n + n*ilaenv( 1, "DGEQRF", " ", m, n, 
                                           -1, -1 );
                    maxwrk = max( maxwrk, 3*n + 2*n*ilaenv( 1, "DGEBRD"
                                                            , " ", n, n, -1, -1 ) );
                    if( wntvo || wntvas ) 
                        maxwrk = max( maxwrk, 3*n + (n - 1)*ilaenv( 1, 
                                                                    "DORGBR", "P", n, n, n, -1 ) );
                    maxwrk = max( maxwrk, 5*n - 4 );
                    minwrk = max( 4*n, 5*n - 4 );
                }
                else if( wntuo && wntvn ) { 
          
                    //                 Path 2 (M much larger than N, JOBU='O', JOBVT='N')
          
                    wrkbl = n + n*ilaenv( 1, "DGEQRF", " ", m, n, 
                                          -1, -1 );
                    wrkbl = max( wrkbl, n + n*ilaenv( 1, "DORGQR", 
                                                      " ", m, n, n, -1 ) );
                    wrkbl = max( wrkbl, 3*n + 2*n*ilaenv( 1, "DGEBRD"
                                                          , " ", n, n, -1, -1 ) );
                    wrkbl = max( wrkbl, 3*n + n*ilaenv( 1, "DORGBR"
                                                        , "Q", n, n, n, -1 ) );
                    wrkbl = max( wrkbl, 5*n - 4 );
                    maxwrk = max( n*n + wrkbl, n*n + m*n );
                    minwrk = max( 3*n + m, 5*n - 4 );
                    minwrk = min( minwrk, maxwrk );
                }
                else if( wntuo && wntvas ) { 
          
                    //                 Path 3 (M much larger than N, JOBU='O', JOBVT='S' or
                    //                 'A')
          
                    wrkbl = n + n*ilaenv( 1, "DGEQRF", " ", m, n, 
                                          -1, -1 );
                    wrkbl = max( wrkbl, n + n*ilaenv( 1, "DORGQR", 
                                                      " ", m, n, n, -1 ) );
                    wrkbl = max( wrkbl, 3*n + 2*n*ilaenv( 1, "DGEBRD"
                                                          , " ", n, n, -1, -1 ) );
                    wrkbl = max( wrkbl, 3*n + n*ilaenv( 1, "DORGBR"
                                                        , "Q", n, n, n, -1 ) );
                    wrkbl = max( wrkbl, 3*n + (n - 1)*ilaenv( 1, "DORGBR"
                                                              , "P", n, n, n, -1 ) );
                    wrkbl = max( wrkbl, 5*n - 4 );
                    maxwrk = max( n*n + wrkbl, n*n + m*n );
                    minwrk = max( 3*n + m, 5*n - 4 );
                    minwrk = min( minwrk, maxwrk );
                }
                else if( wntus && wntvn ) { 
          
                    //                 Path 4 (M much larger than N, JOBU='S', JOBVT='N')
          
                    wrkbl = n + n*ilaenv( 1, "DGEQRF", " ", m, n, 
                                          -1, -1 );
                    wrkbl = max( wrkbl, n + n*ilaenv( 1, "DORGQR", 
                                                      " ", m, n, n, -1 ) );
                    wrkbl = max( wrkbl, 3*n + 2*n*ilaenv( 1, "DGEBRD"
                                                          , " ", n, n, -1, -1 ) );
                    wrkbl = max( wrkbl, 3*n + n*ilaenv( 1, "DORGBR"
                                                        , "Q", n, n, n, -1 ) );
                    wrkbl = max( wrkbl, 5*n - 4 );
                    maxwrk = n*n + wrkbl;
                    minwrk = max( 3*n + m, 5*n - 4 );
                    minwrk = min( minwrk, maxwrk );
                }
                else if( wntus && wntvo ) { 
          
                    //                 Path 5 (M much larger than N, JOBU='S', JOBVT='O')
          
                    wrkbl = n + n*ilaenv( 1, "DGEQRF", " ", m, n, 
                                          -1, -1 );
                    wrkbl = max( wrkbl, n + n*ilaenv( 1, "DORGQR", 
                                                      " ", m, n, n, -1 ) );
                    wrkbl = max( wrkbl, 3*n + 2*n*ilaenv( 1, "DGEBRD"
                                                          , " ", n, n, -1, -1 ) );
                    wrkbl = max( wrkbl, 3*n + n*ilaenv( 1, "DORGBR"
                                                        , "Q", n, n, n, -1 ) );
                    wrkbl = max( wrkbl, 3*n + (n - 1)*ilaenv( 1, "DORGBR"
                                                              , "P", n, n, n, -1 ) );
                    wrkbl = max( wrkbl, 5*n - 4 );
                    maxwrk = 2*n*n + wrkbl;
                    minwrk = max( 3*n + m, 5*n - 4 );
                    minwrk = min( minwrk, maxwrk );
                }
                else if( wntus && wntvas ) { 
          
                    //                 Path 6 (M much larger than N, JOBU='S', JOBVT='S' or
                    //                 'A')
          
                    wrkbl = n + n*ilaenv( 1, "DGEQRF", " ", m, n, 
                                          -1, -1 );
                    wrkbl = max( wrkbl, n + n*ilaenv( 1, "DORGQR", 
                                                      " ", m, n, n, -1 ) );
                    wrkbl = max( wrkbl, 3*n + 2*n*ilaenv( 1, "DGEBRD"
                                                          , " ", n, n, -1, -1 ) );
                    wrkbl = max( wrkbl, 3*n + n*ilaenv( 1, "DORGBR"
                                                        , "Q", n, n, n, -1 ) );
                    wrkbl = max( wrkbl, 3*n + (n - 1)*ilaenv( 1, "DORGBR"
                                                              , "P", n, n, n, -1 ) );
                    wrkbl = max( wrkbl, 5*n - 4 );
                    maxwrk = n*n + wrkbl;
                    minwrk = max( 3*n + m, 5*n - 4 );
                    minwrk = min( minwrk, maxwrk );
                }
                else if( wntua && wntvn ) { 
          
                    //                 Path 7 (M much larger than N, JOBU='A', JOBVT='N')
          
                    wrkbl = n + n*ilaenv( 1, "DGEQRF", " ", m, n, 
                                          -1, -1 );
                    wrkbl = max( wrkbl, n + m*ilaenv( 1, "DORGQR", 
                                                      " ", m, m, n, -1 ) );
                    wrkbl = max( wrkbl, 3*n + 2*n*ilaenv( 1, "DGEBRD"
                                                          , " ", n, n, -1, -1 ) );
                    wrkbl = max( wrkbl, 3*n + n*ilaenv( 1, "DORGBR"
                                                        , "Q", n, n, n, -1 ) );
                    wrkbl = max( wrkbl, 5*n - 4 );
                    maxwrk = n*n + wrkbl;
                    minwrk = max( 3*n + m, 5*n - 4 );
                    minwrk = min( minwrk, maxwrk );
                }
                else if( wntua && wntvo ) { 
          
                    //                 Path 8 (M much larger than N, JOBU='A', JOBVT='O')
          
                    wrkbl = n + n*ilaenv( 1, "DGEQRF", " ", m, n, 
                                          -1, -1 );
                    wrkbl = max( wrkbl, n + m*ilaenv( 1, "DORGQR", 
                                                      " ", m, m, n, -1 ) );
                    wrkbl = max( wrkbl, 3*n + 2*n*ilaenv( 1, "DGEBRD"
                                                          , " ", n, n, -1, -1 ) );
                    wrkbl = max( wrkbl, 3*n + n*ilaenv( 1, "DORGBR"
                                                        , "Q", n, n, n, -1 ) );
                    wrkbl = max( wrkbl, 3*n + (n - 1)*ilaenv( 1, "DORGBR"
                                                              , "P", n, n, n, -1 ) );
                    wrkbl = max( wrkbl, 5*n - 4 );
                    maxwrk = 2*n*n + wrkbl;
                    minwrk = max( 3*n + m, 5*n - 4 );
                    minwrk = min( minwrk, maxwrk );
                }
                else if( wntua && wntvas ) { 
          
                    //                 Path 9 (M much larger than N, JOBU='A', JOBVT='S' or
                    //                 'A')
          
                    wrkbl = n + n*ilaenv( 1, "DGEQRF", " ", m, n, 
                                          -1, -1 );
                    wrkbl = max( wrkbl, n + m*ilaenv( 1, "DORGQR", 
                                                      " ", m, m, n, -1 ) );
                    wrkbl = max( wrkbl, 3*n + 2*n*ilaenv( 1, "DGEBRD"
                                                          , " ", n, n, -1, -1 ) );
                    wrkbl = max( wrkbl, 3*n + n*ilaenv( 1, "DORGBR"
                                                        , "Q", n, n, n, -1 ) );
                    wrkbl = max( wrkbl, 3*n + (n - 1)*ilaenv( 1, "DORGBR"
                                                              , "P", n, n, n, -1 ) );
                    wrkbl = max( wrkbl, 5*n - 4 );
                    maxwrk = n*n + wrkbl;
                    minwrk = max( 3*n + m, 5*n - 4 );
                    minwrk = min( minwrk, maxwrk );
                }
            }
            else { 
        
                //              Path 10 (M at least N, but not much larger)
        
                maxwrk = 3*n + (m + n)*ilaenv( 1, "DGEBRD", " ", m, 
                                               n, -1, -1 );
                if( wntus || wntuo ) 
                    maxwrk = max( maxwrk, 3*n + n*ilaenv( 1, "DORGBR"
                                                          , "Q", m, n, n, -1 ) );
                if( wntua ) 
                    maxwrk = max( maxwrk, 3*n + m*ilaenv( 1, "DORGBR"
                                                          , "Q", m, m, n, -1 ) );
                if( !wntvn ) 
                    maxwrk = max( maxwrk, 3*n + (n - 1)*ilaenv( 1, 
                                                                "DORGBR", "P", n, n, n, -1 ) );
                maxwrk = max( maxwrk, 5*n - 4 );
                minwrk = max( 3*n + m, 5*n - 4 );
            }
        }
        else { 
            if( n >= mnthr ) { 
                if( wntvn ) { 
          
                    //                 Path 1t(N much larger than M, JOBVT='N')
          
                    maxwrk = m + m*ilaenv( 1, "DGELQF", " ", m, n, 
                                           -1, -1 );
                    maxwrk = max( maxwrk, 3*m + 2*m*ilaenv( 1, "DGEBRD"
                                                            , " ", m, m, -1, -1 ) );
                    if( wntuo || wntuas ) 
                        maxwrk = max( maxwrk, 3*m + m*ilaenv( 1, "DORGBR"
                                                              , "Q", m, m, m, -1 ) );
                    maxwrk = max( maxwrk, 5*m - 4 );
                    minwrk = max( 4*m, 5*m - 4 );
                }
                else if( wntvo && wntun ) { 
          
                    //                 Path 2t(N much larger than M, JOBU='N', JOBVT='O')
          
                    wrkbl = m + m*ilaenv( 1, "DGELQF", " ", m, n, 
                                          -1, -1 );
                    wrkbl = max( wrkbl, m + m*ilaenv( 1, "DORGLQ", 
                                                      " ", m, n, m, -1 ) );
                    wrkbl = max( wrkbl, 3*m + 2*m*ilaenv( 1, "DGEBRD"
                                                          , " ", m, m, -1, -1 ) );
                    wrkbl = max( wrkbl, 3*m + (m - 1)*ilaenv( 1, "DORGBR"
                                                              , "P", m, m, m, -1 ) );
                    wrkbl = max( wrkbl, 5*m - 4 );
                    maxwrk = max( m*m + wrkbl, m*m + m*n );
                    minwrk = max( 3*m + n, 5*m - 4 );
                    minwrk = min( minwrk, maxwrk );
                }
                else if( wntvo && wntuas ) { 
          
                    //                 Path 3t(N much larger than M, JOBU='S' or 'A',
                    //                 JOBVT='O')
          
                    wrkbl = m + m*ilaenv( 1, "DGELQF", " ", m, n, 
                                          -1, -1 );
                    wrkbl = max( wrkbl, m + m*ilaenv( 1, "DORGLQ", 
                                                      " ", m, n, m, -1 ) );
                    wrkbl = max( wrkbl, 3*m + 2*m*ilaenv( 1, "DGEBRD"
                                                          , " ", m, m, -1, -1 ) );
                    wrkbl = max( wrkbl, 3*m + (m - 1)*ilaenv( 1, "DORGBR"
                                                              , "P", m, m, m, -1 ) );
                    wrkbl = max( wrkbl, 3*m + m*ilaenv( 1, "DORGBR"
                                                        , "Q", m, m, m, -1 ) );
                    wrkbl = max( wrkbl, 5*m - 4 );
                    maxwrk = max( m*m + wrkbl, m*m + m*n );
                    minwrk = max( 3*m + n, 5*m - 4 );
                    minwrk = min( minwrk, maxwrk );
                }
                else if( wntvs && wntun ) { 
          
                    //                 Path 4t(N much larger than M, JOBU='N', JOBVT='S')
          
                    wrkbl = m + m*ilaenv( 1, "DGELQF", " ", m, n, 
                                          -1, -1 );
                    wrkbl = max( wrkbl, m + m*ilaenv( 1, "DORGLQ", 
                                                      " ", m, n, m, -1 ) );
                    wrkbl = max( wrkbl, 3*m + 2*m*ilaenv( 1, "DGEBRD"
                                                          , " ", m, m, -1, -1 ) );
                    wrkbl = max( wrkbl, 3*m + (m - 1)*ilaenv( 1, "DORGBR"
                                                              , "P", m, m, m, -1 ) );
                    wrkbl = max( wrkbl, 5*m - 4 );
                    maxwrk = m*m + wrkbl;
                    minwrk = max( 3*m + n, 5*m - 4 );
                    minwrk = min( minwrk, maxwrk );
                }
                else if( wntvs && wntuo ) { 
          
                    //                 Path 5t(N much larger than M, JOBU='O', JOBVT='S')
          
                    wrkbl = m + m*ilaenv( 1, "DGELQF", " ", m, n, 
                                          -1, -1 );
                    wrkbl = max( wrkbl, m + m*ilaenv( 1, "DORGLQ", 
                                                      " ", m, n, m, -1 ) );
                    wrkbl = max( wrkbl, 3*m + 2*m*ilaenv( 1, "DGEBRD"
                                                          , " ", m, m, -1, -1 ) );
                    wrkbl = max( wrkbl, 3*m + (m - 1)*ilaenv( 1, "DORGBR"
                                                              , "P", m, m, m, -1 ) );
                    wrkbl = max( wrkbl, 3*m + m*ilaenv( 1, "DORGBR"
                                                        , "Q", m, m, m, -1 ) );
                    wrkbl = max( wrkbl, 5*m - 4 );
                    maxwrk = 2*m*m + wrkbl;
                    minwrk = max( 3*m + n, 5*m - 4 );
                    minwrk = min( minwrk, maxwrk );
                }
                else if( wntvs && wntuas ) { 
          
                    //                 Path 6t(N much larger than M, JOBU='S' or 'A',
                    //                 JOBVT='S')
          
                    wrkbl = m + m*ilaenv( 1, "DGELQF", " ", m, n, 
                                          -1, -1 );
                    wrkbl = max( wrkbl, m + m*ilaenv( 1, "DORGLQ", 
                                                      " ", m, n, m, -1 ) );
                    wrkbl = max( wrkbl, 3*m + 2*m*ilaenv( 1, "DGEBRD"
                                                          , " ", m, m, -1, -1 ) );
                    wrkbl = max( wrkbl, 3*m + (m - 1)*ilaenv( 1, "DORGBR"
                                                              , "P", m, m, m, -1 ) );
                    wrkbl = max( wrkbl, 3*m + m*ilaenv( 1, "DORGBR"
                                                        , "Q", m, m, m, -1 ) );
                    wrkbl = max( wrkbl, 5*m - 4 );
                    maxwrk = m*m + wrkbl;
                    minwrk = max( 3*m + n, 5*m - 4 );
                    minwrk = min( minwrk, maxwrk );
                }
                else if( wntva && wntun ) { 
          
                    //                 Path 7t(N much larger than M, JOBU='N', JOBVT='A')
          
                    wrkbl = m + m*ilaenv( 1, "DGELQF", " ", m, n, 
                                          -1, -1 );
                    wrkbl = max( wrkbl, m + n*ilaenv( 1, "DORGLQ", 
                                                      " ", n, n, m, -1 ) );
                    wrkbl = max( wrkbl, 3*m + 2*m*ilaenv( 1, "DGEBRD"
                                                          , " ", m, m, -1, -1 ) );
                    wrkbl = max( wrkbl, 3*m + (m - 1)*ilaenv( 1, "DORGBR"
                                                              , "P", m, m, m, -1 ) );
                    wrkbl = max( wrkbl, 5*m - 4 );
                    maxwrk = m*m + wrkbl;
                    minwrk = max( 3*m + n, 5*m - 4 );
                    minwrk = min( minwrk, maxwrk );
                }
                else if( wntva && wntuo ) { 
          
                    //                 Path 8t(N much larger than M, JOBU='O', JOBVT='A')
          
                    wrkbl = m + m*ilaenv( 1, "DGELQF", " ", m, n, 
                                          -1, -1 );
                    wrkbl = max( wrkbl, m + n*ilaenv( 1, "DORGLQ", 
                                                      " ", n, n, m, -1 ) );
                    wrkbl = max( wrkbl, 3*m + 2*m*ilaenv( 1, "DGEBRD"
                                                          , " ", m, m, -1, -1 ) );
                    wrkbl = max( wrkbl, 3*m + (m - 1)*ilaenv( 1, "DORGBR"
                                                              , "P", m, m, m, -1 ) );
                    wrkbl = max( wrkbl, 3*m + m*ilaenv( 1, "DORGBR"
                                                        , "Q", m, m, m, -1 ) );
                    wrkbl = max( wrkbl, 5*m - 4 );
                    maxwrk = 2*m*m + wrkbl;
                    minwrk = max( 3*m + n, 5*m - 4 );
                    minwrk = min( minwrk, maxwrk );
                }
                else if( wntva && wntuas ) { 
          
                    //                 Path 9t(N much larger than M, JOBU='S' or 'A',
                    //                 JOBVT='A')
          
                    wrkbl = m + m*ilaenv( 1, "DGELQF", " ", m, n, 
                                          -1, -1 );
                    wrkbl = max( wrkbl, m + n*ilaenv( 1, "DORGLQ", 
                                                      " ", n, n, m, -1 ) );
                    wrkbl = max( wrkbl, 3*m + 2*m*ilaenv( 1, "DGEBRD"
                                                          , " ", m, m, -1, -1 ) );
                    wrkbl = max( wrkbl, 3*m + (m - 1)*ilaenv( 1, "DORGBR"
                                                              , "P", m, m, m, -1 ) );
                    wrkbl = max( wrkbl, 3*m + m*ilaenv( 1, "DORGBR"
                                                        , "Q", m, m, m, -1 ) );
                    wrkbl = max( wrkbl, 5*m - 4 );
                    maxwrk = m*m + wrkbl;
                    minwrk = max( 3*m + n, 5*m - 4 );
                    minwrk = min( minwrk, maxwrk );
                }
            }
            else { 
        
                //              Path 10t(N greater than M, but not much larger)
        
                maxwrk = 3*m + (m + n)*ilaenv( 1, "DGEBRD", " ", m, 
                                               n, -1, -1 );
                if( wntvs || wntvo ) 
                    maxwrk = max( maxwrk, 3*m + m*ilaenv( 1, "DORGBR"
                                                          , "P", m, n, m, -1 ) );
                if( wntva ) 
                    maxwrk = max( maxwrk, 3*m + n*ilaenv( 1, "DORGBR"
                                                          , "P", n, n, m, -1 ) );
                if( !wntun ) 
                    maxwrk = max( maxwrk, 3*m + (m - 1)*ilaenv( 1, 
                                                                "DORGBR", "Q", m, m, m, -1 ) );
                maxwrk = max( maxwrk, 5*m - 4 );
                minwrk = max( 3*m + n, 5*m - 4 );
            }
        }
        work[0] = maxwrk;
    }
  
    if( lwork < minwrk ) { 
        info = -13;
    }
    if( info != 0 ) { 
        xerbla( "DGESVD", -info );
        rel_chrtmp(_c,NCHRTMPS);
        return;
    }
  
    //     Quick return if possible
  
    if( m == 0 || n == 0 ) { 
        if( lwork >= 1 ) 
            work[0] = ONE;
        rel_chrtmp(_c,NCHRTMPS);
        return;
    }
  
    //     Get machine constants
  
    eps = dlamch( 'P' );
    smlnum = sqrt( dlamch( 'S' ) )/eps;
    bignum = ONE/smlnum;
  
    //     Scale A if max entry outside range [SMLNUM,BIGNUM]
  
    anrm = dlange( 'M', m, n, a, lda, dum );
    iscl = 0;
    if( anrm > ZERO && anrm < smlnum ) { 
        iscl = 1;
        dlascl( 'G', 0, 0, anrm, smlnum, m, n, a, lda, info );
    }
    else if( anrm > bignum ) { 
        iscl = 1;
        dlascl( 'G', 0, 0, anrm, bignum, m, n, a, lda, info );
    }
  
    if( m >= n ) { 
    
        //        A has at least as many rows as columns. If A has sufficiently
        //        more rows than columns, first reduce using the QR
        //        decomposition (if sufficient workspace available)
    
        if( m >= mnthr ) { 
      
            if( wntun ) { 
        
                //              Path 1 (M much larger than N, JOBU='N')
                //              No left singular vectors to be computed
        
                itau = 1;
                iwork = itau + n;
        
                //              Compute A=Q*R
                //              (Workspace: need 2*N, prefer N+N*NB)
        
                dgeqrf( m, n, a, lda, &work[itau - 1], &work[iwork - 1], 
                        lwork - iwork + 1, info );
        
                //              Zero out below R
        
                dlaset( 'L', n - 1, n - 1, ZERO, ZERO, &A(0,1), lda );
                ie = 1;
                itauq = ie + n;
                itaup = itauq + n;
                iwork = itaup + n;
        
                //              Bidiagonalize R in A
                //              (Workspace: need 4*N, prefer 3*N+2*N*NB)
        
                dgebrd( n, n, a, lda, s, &work[ie - 1], &work[itauq - 1], 
                        &work[itaup - 1], &work[iwork - 1], lwork - iwork + 
                        1, info );
                ncvt = 0;
                if( wntvo || wntvas ) { 
          
                    //                 If right singular vectors desired, generate P'.
                    //                 (Workspace: need 4*N-1, prefer 3*N+(N-1)*NB)
          
                    dorgbr( 'P', n, n, n, a, lda, &work[itaup - 1], 
                            &work[iwork - 1], lwork - iwork + 1, info );
                    ncvt = n;
                }
                iwork = ie + n;
        
                //              Perform bidiagonal QR iteration, computing right
                //              singular vectors of A in A if desired
                //              (Workspace: need 5*N-4)
        
                dbdsqr( 'U', n, ncvt, 0, 0, s, &work[ie - 1], a, lda, 
                        dum, 1, dum, 1, &work[iwork - 1], info );
        
                //              If right singular vectors desired in VT, copy them there
        
                if( wntvas ) 
                    dlacpy( 'F', n, n, a, lda, vt, ldvt );
        
            }
            else if( wntuo && wntvn ) { 
        
                //              Path 2 (M much larger than N, JOBU='O', JOBVT='N')
                //              N left singular vectors to be overwritten on A and
                //              no right singular vectors to be computed
        
                if( lwork >= n*n + max( 4*n, 5*n - 4 ) ) { 
          
                    //                 Sufficient workspace for a fast algorithm
          
                    ir = 1;
                    if( lwork >= max( wrkbl, lda*n ) + lda*n ) { 
            
                        //                    WORK(IU) is LDA by N, WORK(IR) is LDA by N
            
                        ldwrku = lda;
                        ldwrkr = lda;
                    }
                    else if( lwork >= max( wrkbl, lda*n ) + n*n ) { 
            
                        //                    WORK(IU) is LDA by N, WORK(IR) is N by N
            
                        ldwrku = lda;
                        ldwrkr = n;
                    }
                    else { 
            
                        //                    WORK(IU) is LDWRKU by N, WORK(IR) is N by N
            
                        ldwrku = (lwork - n*n)/n;
                        ldwrkr = n;
                    }
                    itau = ir + ldwrkr*n;
                    iwork = itau + n;
          
                    //                 Compute A=Q*R
                    //                 (Workspace: need N*N+2*N, prefer N*N+N+N*NB)
          
                    dgeqrf( m, n, a, lda, &work[itau - 1], &work[iwork - 1], 
                            lwork - iwork + 1, info );
          
                    //                 Copy R to WORK(IR) and zero out below it
          
                    dlacpy( 'U', n, n, a, lda, &work[ir - 1], ldwrkr );
                    dlaset( 'L', n - 1, n - 1, ZERO, ZERO, &work[ir], 
                            ldwrkr );
          
                    //                 Generate Q in A
                    //                 (Workspace: need N*N+2*N, prefer N*N+N+N*NB)
          
                    dorgqr( m, n, n, a, lda, &work[itau - 1], &work[iwork - 1], 
                            lwork - iwork + 1, info );
                    ie = itau;
                    itauq = ie + n;
                    itaup = itauq + n;
                    iwork = itaup + n;
          
                    //                 Bidiagonalize R in WORK(IR)
                    //                 (Workspace: need N*N+4*N, prefer N*N+3*N+2*N*NB)
          
                    dgebrd( n, n, &work[ir - 1], ldwrkr, s, &work[ie - 1], 
                            &work[itauq - 1], &work[itaup - 1], &work[iwork - 1], 
                            lwork - iwork + 1, info );
          
                    //                 Generate left vectors bidiagonalizing R
                    //                 (Workspace: need N*N+4*N, prefer N*N+3*N+N*NB)
          
                    dorgbr( 'Q', n, n, n, &work[ir - 1], ldwrkr, &work[itauq - 1], 
                            &work[iwork - 1], lwork - iwork + 1, info );
                    iwork = ie + n;
          
                    //                 Perform bidiagonal QR iteration, computing left
                    //                 singular vectors of R in WORK(IR)
                    //                 (Workspace: need N*N+5*N-4)
          
                    dbdsqr( 'U', n, 0, n, 0, s, &work[ie - 1], dum, 
                            1, &work[ir - 1], ldwrkr, dum, 1, &work[iwork - 1], 
                            info );
                    iu = ie;
          
                    //                 Multiply Q in A by left singular vectors of R in
                    //                 WORK(IR), storing result in WORK(IU) and copying to A
                    //                 (Workspace: need N*N+N, prefer N*N+M*N)
          
                    for( i = 1, i_ = i - 1, _do0=docnt(i,m,_do1 = ldwrku); _do0 > 0; i += _do1, i_ += _do1, _do0-- ) { 
                        chunk = min( m - i + 1, ldwrku );
                        dgemm( 'N', 'N', chunk, n, n, ONE, &A(0,i_), 
                               lda, &work[ir - 1], ldwrkr, ZERO, &work[iu - 1], 
                               ldwrku );
                        dlacpy( 'F', chunk, n, &work[iu - 1], ldwrku, 
                                &A(0,i_), lda );
                    }
          
                }
                else { 
          
                    //                 Insufficient workspace for a fast algorithm
          
                    ie = 1;
                    itauq = ie + n;
                    itaup = itauq + n;
                    iwork = itaup + n;
          
                    //                 Bidiagonalize A
                    //                 (Workspace: need 3*N+M, prefer 3*N+(M+N)*NB)
          
                    dgebrd( m, n, a, lda, s, &work[ie - 1], &work[itauq - 1], 
                            &work[itaup - 1], &work[iwork - 1], lwork - iwork + 
                            1, info );
          
                    //                 Generate left vectors bidiagonalizing A
                    //                 (Workspace: need 4*N, prefer 3*N+N*NB)
          
                    dorgbr( 'Q', m, n, n, a, lda, &work[itauq - 1], 
                            &work[iwork - 1], lwork - iwork + 1, info );
                    iwork = ie + n;
          
                    //                 Perform bidiagonal QR iteration, computing left
                    //                 singular vectors of A in A
                    //                 (Workspace: need 5*N-4)
          
                    dbdsqr( 'U', n, 0, m, 0, s, &work[ie - 1], dum, 
                            1, a, lda, dum, 1, &work[iwork - 1], info );
          
                }
        
            }
            else if( wntuo && wntvas ) { 
        
                //              Path 3 (M much larger than N, JOBU='O', JOBVT='S' or 'A')
                //              N left singular vectors to be overwritten on A and
                //              N right singular vectors to be computed in VT
        
                if( lwork >= n*n + max( 4*n, 5*n - 4 ) ) { 
          
                    //                 Sufficient workspace for a fast algorithm
          
                    ir = 1;
                    if( lwork >= max( wrkbl, lda*n ) + lda*n ) { 
            
                        //                    WORK(IU) is LDA by N and WORK(IR) is LDA by N
            
                        ldwrku = lda;
                        ldwrkr = lda;
                    }
                    else if( lwork >= max( wrkbl, lda*n ) + n*n ) { 
            
                        //                    WORK(IU) is LDA by N and WORK(IR) is N by N
            
                        ldwrku = lda;
                        ldwrkr = n;
                    }
                    else { 
            
                        //                    WORK(IU) is LDWRKU by N and WORK(IR) is N by N
            
                        ldwrku = (lwork - n*n)/n;
                        ldwrkr = n;
                    }
                    itau = ir + ldwrkr*n;
                    iwork = itau + n;
          
                    //                 Compute A=Q*R
                    //                 (Workspace: need N*N+2*N, prefer N*N+N+N*NB)
          
                    dgeqrf( m, n, a, lda, &work[itau - 1], &work[iwork - 1], 
                            lwork - iwork + 1, info );
          
                    //                 Copy R to VT, zeroing out below it
          
                    dlacpy( 'U', n, n, a, lda, vt, ldvt );
                    dlaset( 'L', n - 1, n - 1, ZERO, ZERO, &VT(0,1), 
                            ldvt );
          
                    //                 Generate Q in A
                    //                 (Workspace: need N*N+2*N, prefer N*N+N+N*NB)
          
                    dorgqr( m, n, n, a, lda, &work[itau - 1], &work[iwork - 1], 
                            lwork - iwork + 1, info );
                    ie = itau;
                    itauq = ie + n;
                    itaup = itauq + n;
                    iwork = itaup + n;
          
                    //                 Bidiagonalize R in VT, copying result to WORK(IR)
                    //                 (Workspace: need N*N+4*N, prefer N*N+3*N+2*N*NB)
          
                    dgebrd( n, n, vt, ldvt, s, &work[ie - 1], &work[itauq - 1], 
                            &work[itaup - 1], &work[iwork - 1], lwork - iwork + 
                            1, info );
                    dlacpy( 'L', n, n, vt, ldvt, &work[ir - 1], ldwrkr );
          
                    //                 Generate left vectors bidiagonalizing R in WORK(IR)
                    //                 (Workspace: need N*N+4*N, prefer N*N+3*N+N*NB)
          
                    dorgbr( 'Q', n, n, n, &work[ir - 1], ldwrkr, &work[itauq - 1], 
                            &work[iwork - 1], lwork - iwork + 1, info );
          
                    //                 Generate right vectors bidiagonalizing R in VT
                    //                 (Workspace: need N*N+4*N-1, prefer N*N+3*N+(N-1)*NB)
          
                    dorgbr( 'P', n, n, n, vt, ldvt, &work[itaup - 1], 
                            &work[iwork - 1], lwork - iwork + 1, info );
                    iwork = ie + n;
          
                    //                 Perform bidiagonal QR iteration, computing left
                    //                 singular vectors of R in WORK(IR) and computing right
                    //                 singular vectors of R in VT
                    //                 (Workspace: need N*N+5*N-4)
          
                    dbdsqr( 'U', n, n, n, 0, s, &work[ie - 1], vt, 
                            ldvt, &work[ir - 1], ldwrkr, dum, 1, &work[iwork - 1], 
                            info );
                    iu = ie;
          
                    //                 Multiply Q in A by left singular vectors of R in
                    //                 WORK(IR), storing result in WORK(IU) and copying to A
                    //                 (Workspace: need N*N+N, prefer N*N+M*N)
          
                    for( i = 1, i_ = i - 1, _do2=docnt(i,m,_do3 = ldwrku); _do2 > 0; i += _do3, i_ += _do3, _do2-- ) { 
                        chunk = min( m - i + 1, ldwrku );
                        dgemm( 'N', 'N', chunk, n, n, ONE, &A(0,i_), 
                               lda, &work[ir - 1], ldwrkr, ZERO, &work[iu - 1], 
                               ldwrku );
                        dlacpy( 'F', chunk, n, &work[iu - 1], ldwrku, 
                                &A(0,i_), lda );
                    }
          
                }
                else { 
          
                    //                 Insufficient workspace for a fast algorithm
          
                    itau = 1;
                    iwork = itau + n;
          
                    //                 Compute A=Q*R
                    //                 (Workspace: need 2*N, prefer N+N*NB)
          
                    dgeqrf( m, n, a, lda, &work[itau - 1], &work[iwork - 1], 
                            lwork - iwork + 1, info );
          
                    //                 Copy R to VT, zeroing out below it
          
                    dlacpy( 'U', n, n, a, lda, vt, ldvt );
                    dlaset( 'L', n - 1, n - 1, ZERO, ZERO, &VT(0,1), 
                            ldvt );
          
                    //                 Generate Q in A
                    //                 (Workspace: need 2*N, prefer N+N*NB)
          
                    dorgqr( m, n, n, a, lda, &work[itau - 1], &work[iwork - 1], 
                            lwork - iwork + 1, info );
                    ie = itau;
                    itauq = ie + n;
                    itaup = itauq + n;
                    iwork = itaup + n;
          
                    //                 Bidiagonalize R in VT
                    //                 (Workspace: need 4*N, prefer 3*N+2*N*NB)
          
                    dgebrd( n, n, vt, ldvt, s, &work[ie - 1], &work[itauq - 1], 
                            &work[itaup - 1], &work[iwork - 1], lwork - iwork + 
                            1, info );
          
                    //                 Multiply Q in A by left vectors bidiagonalizing R
                    //                 (Workspace: need 3*N+M, prefer 3*N+M*NB)
          
                    dormbr( 'Q', 'R', 'N', m, n, n, vt, ldvt, &work[itauq - 1], 
                            a, lda, &work[iwork - 1], lwork - iwork + 1, 
                            info );
          
                    //                 Generate right vectors bidiagonalizing R in VT
                    //                 (Workspace: need 4*N-1, prefer 3*N+(N-1)*NB)
          
                    dorgbr( 'P', n, n, n, vt, ldvt, &work[itaup - 1], 
                            &work[iwork - 1], lwork - iwork + 1, info );
                    iwork = ie + n;
          
                    //                 Perform bidiagonal QR iteration, computing left
                    //                 singular vectors of A in A and computing right
                    //                 singular vectors of A in VT
                    //                 (Workspace: need 5*N-4)
          
                    dbdsqr( 'U', n, n, m, 0, s, &work[ie - 1], vt, 
                            ldvt, a, lda, dum, 1, &work[iwork - 1], info );
          
                }
        
            }
            else if( wntus ) { 
        
                if( wntvn ) { 
          
                    //                 Path 4 (M much larger than N, JOBU='S', JOBVT='N')
                    //                 N left singular vectors to be computed in U and
                    //                 no right singular vectors to be computed
          
                    if( lwork >= n*n + max( 4*n, 5*n - 4 ) ) { 
            
                        //                    Sufficient workspace for a fast algorithm
            
                        ir = 1;
                        if( lwork >= wrkbl + lda*n ) { 
              
                            //                       WORK(IR) is LDA by N
              
                            ldwrkr = lda;
                        }
                        else { 
              
                            //                       WORK(IR) is N by N
              
                            ldwrkr = n;
                        }
                        itau = ir + ldwrkr*n;
                        iwork = itau + n;
            
                        //                    Compute A=Q*R
                        //                    (Workspace: need N*N+2*N, prefer N*N+N+N*NB)
            
                        dgeqrf( m, n, a, lda, &work[itau - 1], &work[iwork - 1], 
                                lwork - iwork + 1, info );
            
                        //                    Copy R to WORK(IR), zeroing out below it
            
                        dlacpy( 'U', n, n, a, lda, &work[ir - 1], 
                                ldwrkr );
                        dlaset( 'L', n - 1, n - 1, ZERO, ZERO, &work[ir], 
                                ldwrkr );
            
                        //                    Generate Q in A
                        //                    (Workspace: need N*N+2*N, prefer N*N+N+N*NB)
            
                        dorgqr( m, n, n, a, lda, &work[itau - 1], 
                                &work[iwork - 1], lwork - iwork + 1, info );
                        ie = itau;
                        itauq = ie + n;
                        itaup = itauq + n;
                        iwork = itaup + n;
            
                        //                    Bidiagonalize R in WORK(IR)
                        //                    (Workspace: need N*N+4*N, prefer N*N+3*N+2*N*NB)
            
                        dgebrd( n, n, &work[ir - 1], ldwrkr, s, &work[ie - 1], 
                                &work[itauq - 1], &work[itaup - 1], &work[iwork - 1], 
                                lwork - iwork + 1, info );
            
                        //                    Generate left vectors bidiagonalizing R in WORK(IR)
                        //                    (Workspace: need N*N+4*N, prefer N*N+3*N+N*NB)
            
                        dorgbr( 'Q', n, n, n, &work[ir - 1], ldwrkr, 
                                &work[itauq - 1], &work[iwork - 1], lwork - 
                                iwork + 1, info );
                        iwork = ie + n;
            
                        //                    Perform bidiagonal QR iteration, computing left
                        //                    singular vectors of R in WORK(IR)
                        //                    (Workspace: need N*N+5*N-4)
            
                        dbdsqr( 'U', n, 0, n, 0, s, &work[ie - 1], 
                                dum, 1, &work[ir - 1], ldwrkr, dum, 1, &work[iwork - 1], 
                                info );
            
                        //                    Multiply Q in A by left singular vectors of R in
                        //                    WORK(IR), storing result in U
                        //                    (Workspace: need N*N)
            
                        dgemm( 'N', 'N', m, n, n, ONE, a, lda, &work[ir - 1], 
                               ldwrkr, ZERO, u, ldu );
            
                    }
                    else { 
            
                        //                    Insufficient workspace for a fast algorithm
            
                        itau = 1;
                        iwork = itau + n;
            
                        //                    Compute A=Q*R, copying result to U
                        //                    (Workspace: need 2*N, prefer N+N*NB)
            
                        dgeqrf( m, n, a, lda, &work[itau - 1], &work[iwork - 1], 
                                lwork - iwork + 1, info );
                        dlacpy( 'L', m, n, a, lda, u, ldu );
            
                        //                    Generate Q in U
                        //                    (Workspace: need 2*N, prefer N+N*NB)
            
                        dorgqr( m, n, n, u, ldu, &work[itau - 1], 
                                &work[iwork - 1], lwork - iwork + 1, info );
                        ie = itau;
                        itauq = ie + n;
                        itaup = itauq + n;
                        iwork = itaup + n;
            
                        //                    Zero out below R in A
            
                        dlaset( 'L', n - 1, n - 1, ZERO, ZERO, &A(0,1), 
                                lda );
            
                        //                    Bidiagonalize R in A
                        //                    (Workspace: need 4*N, prefer 3*N+2*N*NB)
            
                        dgebrd( n, n, a, lda, s, &work[ie - 1], &work[itauq - 1], 
                                &work[itaup - 1], &work[iwork - 1], lwork - 
                                iwork + 1, info );
            
                        //                    Multiply Q in U by left vectors bidiagonalizing R
                        //                    (Workspace: need 3*N+M, prefer 3*N+M*NB)
            
                        dormbr( 'Q', 'R', 'N', m, n, n, a, lda, &work[itauq - 1], 
                                u, ldu, &work[iwork - 1], lwork - iwork + 
                                1, info );
                        iwork = ie + n;
            
                        //                    Perform bidiagonal QR iteration, computing left
                        //                    singular vectors of A in U
                        //                    (Workspace: need 5*N-4)
            
                        dbdsqr( 'U', n, 0, m, 0, s, &work[ie - 1], 
                                dum, 1, u, ldu, dum, 1, &work[iwork - 1], 
                                info );
            
                    }
          
                }
                else if( wntvo ) { 
          
                    //                 Path 5 (M much larger than N, JOBU='S', JOBVT='O')
                    //                 N left singular vectors to be computed in U and
                    //                 N right singular vectors to be overwritten on A
          
                    if( lwork >= 2*n*n + max( 4*n, 5*n - 4 ) ) { 
            
                        //                    Sufficient workspace for a fast algorithm
            
                        iu = 1;
                        if( lwork >= wrkbl + 2*lda*n ) { 
              
                            //                       WORK(IU) is LDA by N and WORK(IR) is LDA by N
              
                            ldwrku = lda;
                            ir = iu + ldwrku*n;
                            ldwrkr = lda;
                        }
                        else if( lwork >= wrkbl + (lda + n)*n ) { 
              
                            //                       WORK(IU) is LDA by N and WORK(IR) is N by N
              
                            ldwrku = lda;
                            ir = iu + ldwrku*n;
                            ldwrkr = n;
                        }
                        else { 
              
                            //                       WORK(IU) is N by N and WORK(IR) is N by N
              
                            ldwrku = n;
                            ir = iu + ldwrku*n;
                            ldwrkr = n;
                        }
                        itau = ir + ldwrkr*n;
                        iwork = itau + n;
            
                        //                    Compute A=Q*R
                        //                    (Workspace: need 2*N*N+2*N, prefer 2*N*N+N+N*NB)
            
                        dgeqrf( m, n, a, lda, &work[itau - 1], &work[iwork - 1], 
                                lwork - iwork + 1, info );
            
                        //                    Copy R to WORK(IU), zeroing out below it
            
                        dlacpy( 'U', n, n, a, lda, &work[iu - 1], 
                                ldwrku );
                        dlaset( 'L', n - 1, n - 1, ZERO, ZERO, &work[iu], 
                                ldwrku );
            
                        //                    Generate Q in A
                        //                    (Workspace: need 2*N*N+2*N, prefer 2*N*N+N+N*NB)
            
                        dorgqr( m, n, n, a, lda, &work[itau - 1], 
                                &work[iwork - 1], lwork - iwork + 1, info );
                        ie = itau;
                        itauq = ie + n;
                        itaup = itauq + n;
                        iwork = itaup + n;
            
                        //                    Bidiagonalize R in WORK(IU), copying result to
                        //                    WORK(IR)
                        //                    (Workspace: need 2*N*N+4*N,
                        //                                prefer 2*N*N+3*N+2*N*NB)
            
                        dgebrd( n, n, &work[iu - 1], ldwrku, s, &work[ie - 1], 
                                &work[itauq - 1], &work[itaup - 1], &work[iwork - 1], 
                                lwork - iwork + 1, info );
                        dlacpy( 'U', n, n, &work[iu - 1], ldwrku, 
                                &work[ir - 1], ldwrkr );
            
                        //                    Generate left bidiagonalizing vectors in WORK(IU)
                        //                    (Workspace: need 2*N*N+4*N, prefer 2*N*N+3*N+N*NB)
            
                        dorgbr( 'Q', n, n, n, &work[iu - 1], ldwrku, 
                                &work[itauq - 1], &work[iwork - 1], lwork - 
                                iwork + 1, info );
            
                        //                    Generate right bidiagonalizing vectors in WORK(IR)
                        //                    (Workspace: need 2*N*N+4*N-1,
                        //                                prefer 2*N*N+3*N+(N-1)*NB)
            
                        dorgbr( 'P', n, n, n, &work[ir - 1], ldwrkr, 
                                &work[itaup - 1], &work[iwork - 1], lwork - 
                                iwork + 1, info );
                        iwork = ie + n;
            
                        //                    Perform bidiagonal QR iteration, computing left
                        //                    singular vectors of R in WORK(IU) and computing
                        //                    right singular vectors of R in WORK(IR)
                        //                    (Workspace: need 2*N*N+5*N-4)
            
                        dbdsqr( 'U', n, n, n, 0, s, &work[ie - 1], 
                                &work[ir - 1], ldwrkr, &work[iu - 1], ldwrku, 
                                dum, 1, &work[iwork - 1], info );
            
                        //                    Multiply Q in A by left singular vectors of R in
                        //                    WORK(IU), storing result in U
                        //                    (Workspace: need N*N)
            
                        dgemm( 'N', 'N', m, n, n, ONE, a, lda, &work[iu - 1], 
                               ldwrku, ZERO, u, ldu );
            
                        //                    Copy right singular vectors of R to A
                        //                    (Workspace: need N*N)
            
                        dlacpy( 'F', n, n, &work[ir - 1], ldwrkr, 
                                a, lda );
            
                    }
                    else { 
            
                        //                    Insufficient workspace for a fast algorithm
            
                        itau = 1;
                        iwork = itau + n;
            
                        //                    Compute A=Q*R, copying result to U
                        //                    (Workspace: need 2*N, prefer N+N*NB)
            
                        dgeqrf( m, n, a, lda, &work[itau - 1], &work[iwork - 1], 
                                lwork - iwork + 1, info );
                        dlacpy( 'L', m, n, a, lda, u, ldu );
            
                        //                    Generate Q in U
                        //                    (Workspace: need 2*N, prefer N+N*NB)
            
                        dorgqr( m, n, n, u, ldu, &work[itau - 1], 
                                &work[iwork - 1], lwork - iwork + 1, info );
                        ie = itau;
                        itauq = ie + n;
                        itaup = itauq + n;
                        iwork = itaup + n;
            
                        //                    Zero out below R in A
            
                        dlaset( 'L', n - 1, n - 1, ZERO, ZERO, &A(0,1), 
                                lda );
            
                        //                    Bidiagonalize R in A
                        //                    (Workspace: need 4*N, prefer 3*N+2*N*NB)
            
                        dgebrd( n, n, a, lda, s, &work[ie - 1], &work[itauq - 1], 
                                &work[itaup - 1], &work[iwork - 1], lwork - 
                                iwork + 1, info );
            
                        //                    Multiply Q in U by left vectors bidiagonalizing R
                        //                    (Workspace: need 3*N+M, prefer 3*N+M*NB)
            
                        dormbr( 'Q', 'R', 'N', m, n, n, a, lda, &work[itauq - 1], 
                                u, ldu, &work[iwork - 1], lwork - iwork + 
                                1, info );
            
                        //                    Generate right vectors bidiagonalizing R in A
                        //                    (Workspace: need 4*N-1, prefer 3*N+(N-1)*NB)
            
                        dorgbr( 'P', n, n, n, a, lda, &work[itaup - 1], 
                                &work[iwork - 1], lwork - iwork + 1, info );
                        iwork = ie + n;
            
                        //                    Perform bidiagonal QR iteration, computing left
                        //                    singular vectors of A in U and computing right
                        //                    singular vectors of A in A
                        //                    (Workspace: need 5*N-4)
            
                        dbdsqr( 'U', n, n, m, 0, s, &work[ie - 1], 
                                a, lda, u, ldu, dum, 1, &work[iwork - 1], 
                                info );
            
                    }
          
                }
                else if( wntvas ) { 
          
                    //                 Path 6 (M much larger than N, JOBU='S', JOBVT='S'
                    //                         or 'A')
                    //                 N left singular vectors to be computed in U and
                    //                 N right singular vectors to be computed in VT
          
                    if( lwork >= n*n + max( 4*n, 5*n - 4 ) ) { 
            
                        //                    Sufficient workspace for a fast algorithm
            
                        iu = 1;
                        if( lwork >= wrkbl + lda*n ) { 
              
                            //                       WORK(IU) is LDA by N
              
                            ldwrku = lda;
                        }
                        else { 
              
                            //                       WORK(IU) is N by N
              
                            ldwrku = n;
                        }
                        itau = iu + ldwrku*n;
                        iwork = itau + n;
            
                        //                    Compute A=Q*R
                        //                    (Workspace: need N*N+2*N, prefer N*N+N+N*NB)
            
                        dgeqrf( m, n, a, lda, &work[itau - 1], &work[iwork - 1], 
                                lwork - iwork + 1, info );
            
                        //                    Copy R to WORK(IU), zeroing out below it
            
                        dlacpy( 'U', n, n, a, lda, &work[iu - 1], 
                                ldwrku );
                        dlaset( 'L', n - 1, n - 1, ZERO, ZERO, &work[iu], 
                                ldwrku );
            
                        //                    Generate Q in A
                        //                    (Workspace: need N*N+2*N, prefer N*N+N+N*NB)
            
                        dorgqr( m, n, n, a, lda, &work[itau - 1], 
                                &work[iwork - 1], lwork - iwork + 1, info );
                        ie = itau;
                        itauq = ie + n;
                        itaup = itauq + n;
                        iwork = itaup + n;
            
                        //                    Bidiagonalize R in WORK(IU), copying result to VT
                        //                    (Workspace: need N*N+4*N, prefer N*N+3*N+2*N*NB)
            
                        dgebrd( n, n, &work[iu - 1], ldwrku, s, &work[ie - 1], 
                                &work[itauq - 1], &work[itaup - 1], &work[iwork - 1], 
                                lwork - iwork + 1, info );
                        dlacpy( 'U', n, n, &work[iu - 1], ldwrku, 
                                vt, ldvt );
            
                        //                    Generate left bidiagonalizing vectors in WORK(IU)
                        //                    (Workspace: need N*N+4*N, prefer N*N+3*N+N*NB)
            
                        dorgbr( 'Q', n, n, n, &work[iu - 1], ldwrku, 
                                &work[itauq - 1], &work[iwork - 1], lwork - 
                                iwork + 1, info );
            
                        //                    Generate right bidiagonalizing vectors in VT
                        //                    (Workspace: need N*N+4*N-1,
                        //                                prefer N*N+3*N+(N-1)*NB)
            
                        dorgbr( 'P', n, n, n, vt, ldvt, &work[itaup - 1], 
                                &work[iwork - 1], lwork - iwork + 1, info );
                        iwork = ie + n;
            
                        //                    Perform bidiagonal QR iteration, computing left
                        //                    singular vectors of R in WORK(IU) and computing
                        //                    right singular vectors of R in VT
                        //                    (Workspace: need N*N+5*N-4)
            
                        dbdsqr( 'U', n, n, n, 0, s, &work[ie - 1], 
                                vt, ldvt, &work[iu - 1], ldwrku, dum, 1, 
                                &work[iwork - 1], info );
            
                        //                    Multiply Q in A by left singular vectors of R in
                        //                    WORK(IU), storing result in U
                        //                    (Workspace: need N*N)
            
                        dgemm( 'N', 'N', m, n, n, ONE, a, lda, &work[iu - 1], 
                               ldwrku, ZERO, u, ldu );
            
                    }
                    else { 
            
                        //                    Insufficient workspace for a fast algorithm
            
                        itau = 1;
                        iwork = itau + n;
            
                        //                    Compute A=Q*R, copying result to U
                        //                    (Workspace: need 2*N, prefer N+N*NB)
            
                        dgeqrf( m, n, a, lda, &work[itau - 1], &work[iwork - 1], 
                                lwork - iwork + 1, info );
                        dlacpy( 'L', m, n, a, lda, u, ldu );
            
                        //                    Generate Q in U
                        //                    (Workspace: need 2*N, prefer N+N*NB)
            
                        dorgqr( m, n, n, u, ldu, &work[itau - 1], 
                                &work[iwork - 1], lwork - iwork + 1, info );
            
                        //                    Copy R to VT, zeroing out below it
            
                        dlacpy( 'U', n, n, a, lda, vt, ldvt );
                        dlaset( 'L', n - 1, n - 1, ZERO, ZERO, &VT(0,1), 
                                ldvt );
                        ie = itau;
                        itauq = ie + n;
                        itaup = itauq + n;
                        iwork = itaup + n;
            
                        //                    Bidiagonalize R in VT
                        //                    (Workspace: need 4*N, prefer 3*N+2*N*NB)
            
                        dgebrd( n, n, vt, ldvt, s, &work[ie - 1], 
                                &work[itauq - 1], &work[itaup - 1], &work[iwork - 1], 
                                lwork - iwork + 1, info );
            
                        //                    Multiply Q in U by left bidiagonalizing vectors
                        //                    in VT
                        //                    (Workspace: need 3*N+M, prefer 3*N+M*NB)
            
                        dormbr( 'Q', 'R', 'N', m, n, n, vt, ldvt, 
                                &work[itauq - 1], u, ldu, &work[iwork - 1], 
                                lwork - iwork + 1, info );
            
                        //                    Generate right bidiagonalizing vectors in VT
                        //                    (Workspace: need 4*N-1, prefer 3*N+(N-1)*NB)
            
                        dorgbr( 'P', n, n, n, vt, ldvt, &work[itaup - 1], 
                                &work[iwork - 1], lwork - iwork + 1, info );
                        iwork = ie + n;
            
                        //                    Perform bidiagonal QR iteration, computing left
                        //                    singular vectors of A in U and computing right
                        //                    singular vectors of A in VT
                        //                    (Workspace: need 5*N-4)
            
                        dbdsqr( 'U', n, n, m, 0, s, &work[ie - 1], 
                                vt, ldvt, u, ldu, dum, 1, &work[iwork - 1], 
                                info );
            
                    }
          
                }
        
            }
            else if( wntua ) { 
        
                if( wntvn ) { 
          
                    //                 Path 7 (M much larger than N, JOBU='A', JOBVT='N')
                    //                 M left singular vectors to be computed in U and
                    //                 no right singular vectors to be computed
          
                    if( lwork >= n*n + vmax( n + m, 4*n, 5*n - 4, 
                                             IEND ) ) { 
            
                        //                    Sufficient workspace for a fast algorithm
            
                        ir = 1;
                        if( lwork >= wrkbl + lda*n ) { 
              
                            //                       WORK(IR) is LDA by N
              
                            ldwrkr = lda;
                        }
                        else { 
              
                            //                       WORK(IR) is N by N
              
                            ldwrkr = n;
                        }
                        itau = ir + ldwrkr*n;
                        iwork = itau + n;
            
                        //                    Compute A=Q*R, copying result to U
                        //                    (Workspace: need N*N+2*N, prefer N*N+N+N*NB)
            
                        dgeqrf( m, n, a, lda, &work[itau - 1], &work[iwork - 1], 
                                lwork - iwork + 1, info );
                        dlacpy( 'L', m, n, a, lda, u, ldu );
            
                        //                    Copy R to WORK(IR), zeroing out below it
            
                        dlacpy( 'U', n, n, a, lda, &work[ir - 1], 
                                ldwrkr );
                        dlaset( 'L', n - 1, n - 1, ZERO, ZERO, &work[ir], 
                                ldwrkr );
            
                        //                    Generate Q in U
                        //                    (Workspace: need N*N+N+M, prefer N*N+N+M*NB)
            
                        dorgqr( m, m, n, u, ldu, &work[itau - 1], 
                                &work[iwork - 1], lwork - iwork + 1, info );
                        ie = itau;
                        itauq = ie + n;
                        itaup = itauq + n;
                        iwork = itaup + n;
            
                        //                    Bidiagonalize R in WORK(IR)
                        //                    (Workspace: need N*N+4*N, prefer N*N+3*N+2*N*NB)
            
                        dgebrd( n, n, &work[ir - 1], ldwrkr, s, &work[ie - 1], 
                                &work[itauq - 1], &work[itaup - 1], &work[iwork - 1], 
                                lwork - iwork + 1, info );
            
                        //                    Generate left bidiagonalizing vectors in WORK(IR)
                        //                    (Workspace: need N*N+4*N, prefer N*N+3*N+N*NB)
            
                        dorgbr( 'Q', n, n, n, &work[ir - 1], ldwrkr, 
                                &work[itauq - 1], &work[iwork - 1], lwork - 
                                iwork + 1, info );
                        iwork = ie + n;
            
                        //                    Perform bidiagonal QR iteration, computing left
                        //                    singular vectors of R in WORK(IR)
                        //                    (Workspace: need N*N+5*N-4)
            
                        dbdsqr( 'U', n, 0, n, 0, s, &work[ie - 1], 
                                dum, 1, &work[ir - 1], ldwrkr, dum, 1, &work[iwork - 1], 
                                info );
            
                        //                    Multiply Q in U by left singular vectors of R in
                        //                    WORK(IR), storing result in A
                        //                    (Workspace: need N*N)
            
                        dgemm( 'N', 'N', m, n, n, ONE, u, ldu, &work[ir - 1], 
                               ldwrkr, ZERO, a, lda );
            
                        //                    Copy left singular vectors of A from A to U
            
                        dlacpy( 'F', m, n, a, lda, u, ldu );
            
                    }
                    else { 
            
                        //                    Insufficient workspace for a fast algorithm
            
                        itau = 1;
                        iwork = itau + n;
            
                        //                    Compute A=Q*R, copying result to U
                        //                    (Workspace: need 2*N, prefer N+N*NB)
            
                        dgeqrf( m, n, a, lda, &work[itau - 1], &work[iwork - 1], 
                                lwork - iwork + 1, info );
                        dlacpy( 'L', m, n, a, lda, u, ldu );
            
                        //                    Generate Q in U
                        //                    (Workspace: need N+M, prefer N+M*NB)
            
                        dorgqr( m, m, n, u, ldu, &work[itau - 1], 
                                &work[iwork - 1], lwork - iwork + 1, info );
                        ie = itau;
                        itauq = ie + n;
                        itaup = itauq + n;
                        iwork = itaup + n;
            
                        //                    Zero out below R in A
            
                        dlaset( 'L', n - 1, n - 1, ZERO, ZERO, &A(0,1), 
                                lda );
            
                        //                    Bidiagonalize R in A
                        //                    (Workspace: need 4*N, prefer 3*N+2*N*NB)
            
                        dgebrd( n, n, a, lda, s, &work[ie - 1], &work[itauq - 1], 
                                &work[itaup - 1], &work[iwork - 1], lwork - 
                                iwork + 1, info );
            
                        //                    Multiply Q in U by left bidiagonalizing vectors
                        //                    in A
                        //                    (Workspace: need 3*N+M, prefer 3*N+M*NB)
            
                        dormbr( 'Q', 'R', 'N', m, n, n, a, lda, &work[itauq - 1], 
                                u, ldu, &work[iwork - 1], lwork - iwork + 
                                1, info );
                        iwork = ie + n;
            
                        //                    Perform bidiagonal QR iteration, computing left
                        //                    singular vectors of A in U
                        //                    (Workspace: need 5*N-4)
            
                        dbdsqr( 'U', n, 0, m, 0, s, &work[ie - 1], 
                                dum, 1, u, ldu, dum, 1, &work[iwork - 1], 
                                info );
            
                    }
          
                }
                else if( wntvo ) { 
          
                    //                 Path 8 (M much larger than N, JOBU='A', JOBVT='O')
                    //                 M left singular vectors to be computed in U and
                    //                 N right singular vectors to be overwritten on A
          
                    if( lwork >= 2*n*n + vmax( n + m, 4*n, 5*n - 4, 
                                               IEND ) ) { 
            
                        //                    Sufficient workspace for a fast algorithm
            
                        iu = 1;
                        if( lwork >= wrkbl + 2*lda*n ) { 
              
                            //                       WORK(IU) is LDA by N and WORK(IR) is LDA by N
              
                            ldwrku = lda;
                            ir = iu + ldwrku*n;
                            ldwrkr = lda;
                        }
                        else if( lwork >= wrkbl + (lda + n)*n ) { 
              
                            //                       WORK(IU) is LDA by N and WORK(IR) is N by N
              
                            ldwrku = lda;
                            ir = iu + ldwrku*n;
                            ldwrkr = n;
                        }
                        else { 
              
                            //                       WORK(IU) is N by N and WORK(IR) is N by N
              
                            ldwrku = n;
                            ir = iu + ldwrku*n;
                            ldwrkr = n;
                        }
                        itau = ir + ldwrkr*n;
                        iwork = itau + n;
            
                        //                    Compute A=Q*R, copying result to U
                        //                    (Workspace: need 2*N*N+2*N, prefer 2*N*N+N+N*NB)
            
                        dgeqrf( m, n, a, lda, &work[itau - 1], &work[iwork - 1], 
                                lwork - iwork + 1, info );
                        dlacpy( 'L', m, n, a, lda, u, ldu );
            
                        //                    Generate Q in U
                        //                    (Workspace: need 2*N*N+N+M, prefer 2*N*N+N+M*NB)
            
                        dorgqr( m, m, n, u, ldu, &work[itau - 1], 
                                &work[iwork - 1], lwork - iwork + 1, info );
            
                        //                    Copy R to WORK(IU), zeroing out below it
            
                        dlacpy( 'U', n, n, a, lda, &work[iu - 1], 
                                ldwrku );
                        dlaset( 'L', n - 1, n - 1, ZERO, ZERO, &work[iu], 
                                ldwrku );
                        ie = itau;
                        itauq = ie + n;
                        itaup = itauq + n;
                        iwork = itaup + n;
            
                        //                    Bidiagonalize R in WORK(IU), copying result to
                        //                    WORK(IR)
                        //                    (Workspace: need 2*N*N+4*N,
                        //                                prefer 2*N*N+3*N+2*N*NB)
            
                        dgebrd( n, n, &work[iu - 1], ldwrku, s, &work[ie - 1], 
                                &work[itauq - 1], &work[itaup - 1], &work[iwork - 1], 
                                lwork - iwork + 1, info );
                        dlacpy( 'U', n, n, &work[iu - 1], ldwrku, 
                                &work[ir - 1], ldwrkr );
            
                        //                    Generate left bidiagonalizing vectors in WORK(IU)
                        //                    (Workspace: need 2*N*N+4*N, prefer 2*N*N+3*N+N*NB)
            
                        dorgbr( 'Q', n, n, n, &work[iu - 1], ldwrku, 
                                &work[itauq - 1], &work[iwork - 1], lwork - 
                                iwork + 1, info );
            
                        //                    Generate right bidiagonalizing vectors in WORK(IR)
                        //                    (Workspace: need 2*N*N+4*N-1,
                        //                                prefer 2*N*N+3*N+(N-1)*NB)
            
                        dorgbr( 'P', n, n, n, &work[ir - 1], ldwrkr, 
                                &work[itaup - 1], &work[iwork - 1], lwork - 
                                iwork + 1, info );
                        iwork = ie + n;
            
                        //                    Perform bidiagonal QR iteration, computing left
                        //                    singular vectors of R in WORK(IU) and computing
                        //                    right singular vectors of R in WORK(IR)
                        //                    (Workspace: need 2*N*N+5*N-4)
            
                        dbdsqr( 'U', n, n, n, 0, s, &work[ie - 1], 
                                &work[ir - 1], ldwrkr, &work[iu - 1], ldwrku, 
                                dum, 1, &work[iwork - 1], info );
            
                        //                    Multiply Q in U by left singular vectors of R in
                        //                    WORK(IU), storing result in A
                        //                    (Workspace: need N*N)
            
                        dgemm( 'N', 'N', m, n, n, ONE, u, ldu, &work[iu - 1], 
                               ldwrku, ZERO, a, lda );
            
                        //                    Copy left singular vectors of A from A to U
            
                        dlacpy( 'F', m, n, a, lda, u, ldu );
            
                        //                    Copy right singular vectors of R from WORK(IR) to A
            
                        dlacpy( 'F', n, n, &work[ir - 1], ldwrkr, 
                                a, lda );
            
                    }
                    else { 
            
                        //                    Insufficient workspace for a fast algorithm
            
                        itau = 1;
                        iwork = itau + n;
            
                        //                    Compute A=Q*R, copying result to U
                        //                    (Workspace: need 2*N, prefer N+N*NB)
            
                        dgeqrf( m, n, a, lda, &work[itau - 1], &work[iwork - 1], 
                                lwork - iwork + 1, info );
                        dlacpy( 'L', m, n, a, lda, u, ldu );
            
                        //                    Generate Q in U
                        //                    (Workspace: need N+M, prefer N+M*NB)
            
                        dorgqr( m, m, n, u, ldu, &work[itau - 1], 
                                &work[iwork - 1], lwork - iwork + 1, info );
                        ie = itau;
                        itauq = ie + n;
                        itaup = itauq + n;
                        iwork = itaup + n;
            
                        //                    Zero out below R in A
            
                        dlaset( 'L', n - 1, n - 1, ZERO, ZERO, &A(0,1), 
                                lda );
            
                        //                    Bidiagonalize R in A
                        //                    (Workspace: need 4*N, prefer 3*N+2*N*NB)
            
                        dgebrd( n, n, a, lda, s, &work[ie - 1], &work[itauq - 1], 
                                &work[itaup - 1], &work[iwork - 1], lwork - 
                                iwork + 1, info );
            
                        //                    Multiply Q in U by left bidiagonalizing vectors
                        //                    in A
                        //                    (Workspace: need 3*N+M, prefer 3*N+M*NB)
            
                        dormbr( 'Q', 'R', 'N', m, n, n, a, lda, &work[itauq - 1], 
                                u, ldu, &work[iwork - 1], lwork - iwork + 
                                1, info );
            
                        //                    Generate right bidiagonalizing vectors in A
                        //                    (Workspace: need 4*N-1, prefer 3*N+(N-1)*NB)
            
                        dorgbr( 'P', n, n, n, a, lda, &work[itaup - 1], 
                                &work[iwork - 1], lwork - iwork + 1, info );
                        iwork = ie + n;
            
                        //                    Perform bidiagonal QR iteration, computing left
                        //                    singular vectors of A in U and computing right
                        //                    singular vectors of A in A
                        //                    (Workspace: need 5*N-4)
            
                        dbdsqr( 'U', n, n, m, 0, s, &work[ie - 1], 
                                a, lda, u, ldu, dum, 1, &work[iwork - 1], 
                                info );
            
                    }
          
                }
                else if( wntvas ) { 
          
                    //                 Path 9 (M much larger than N, JOBU='A', JOBVT='S'
                    //                         or 'A')
                    //                 M left singular vectors to be computed in U and
                    //                 N right singular vectors to be computed in VT
          
                    if( lwork >= n*n + vmax( n + m, 4*n, 5*n - 4, 
                                             IEND ) ) { 
            
                        //                    Sufficient workspace for a fast algorithm
            
                        iu = 1;
                        if( lwork >= wrkbl + lda*n ) { 
              
                            //                       WORK(IU) is LDA by N
              
                            ldwrku = lda;
                        }
                        else { 
              
                            //                       WORK(IU) is N by N
              
                            ldwrku = n;
                        }
                        itau = iu + ldwrku*n;
                        iwork = itau + n;
            
                        //                    Compute A=Q*R, copying result to U
                        //                    (Workspace: need N*N+2*N, prefer N*N+N+N*NB)
            
                        dgeqrf( m, n, a, lda, &work[itau - 1], &work[iwork - 1], 
                                lwork - iwork + 1, info );
                        dlacpy( 'L', m, n, a, lda, u, ldu );
            
                        //                    Generate Q in U
                        //                    (Workspace: need N*N+N+M, prefer N*N+N+M*NB)
            
                        dorgqr( m, m, n, u, ldu, &work[itau - 1], 
                                &work[iwork - 1], lwork - iwork + 1, info );
            
                        //                    Copy R to WORK(IU), zeroing out below it
            
                        dlacpy( 'U', n, n, a, lda, &work[iu - 1], 
                                ldwrku );
                        dlaset( 'L', n - 1, n - 1, ZERO, ZERO, &work[iu], 
                                ldwrku );
                        ie = itau;
                        itauq = ie + n;
                        itaup = itauq + n;
                        iwork = itaup + n;
            
                        //                    Bidiagonalize R in WORK(IU), copying result to VT
                        //                    (Workspace: need N*N+4*N, prefer N*N+3*N+2*N*NB)
            
                        dgebrd( n, n, &work[iu - 1], ldwrku, s, &work[ie - 1], 
                                &work[itauq - 1], &work[itaup - 1], &work[iwork - 1], 
                                lwork - iwork + 1, info );
                        dlacpy( 'U', n, n, &work[iu - 1], ldwrku, 
                                vt, ldvt );
            
                        //                    Generate left bidiagonalizing vectors in WORK(IU)
                        //                    (Workspace: need N*N+4*N, prefer N*N+3*N+N*NB)
            
                        dorgbr( 'Q', n, n, n, &work[iu - 1], ldwrku, 
                                &work[itauq - 1], &work[iwork - 1], lwork - 
                                iwork + 1, info );
            
                        //                    Generate right bidiagonalizing vectors in VT
                        //                    (Workspace: need N*N+4*N-1,
                        //                                prefer N*N+3*N+(N-1)*NB)
            
                        dorgbr( 'P', n, n, n, vt, ldvt, &work[itaup - 1], 
                                &work[iwork - 1], lwork - iwork + 1, info );
                        iwork = ie + n;
            
                        //                    Perform bidiagonal QR iteration, computing left
                        //                    singular vectors of R in WORK(IU) and computing
                        //                    right singular vectors of R in VT
                        //                    (Workspace: need N*N+5*N-4)
            
                        dbdsqr( 'U', n, n, n, 0, s, &work[ie - 1], 
                                vt, ldvt, &work[iu - 1], ldwrku, dum, 1, 
                                &work[iwork - 1], info );
            
                        //                    Multiply Q in U by left singular vectors of R in
                        //                    WORK(IU), storing result in A
                        //                    (Workspace: need N*N)
            
                        dgemm( 'N', 'N', m, n, n, ONE, u, ldu, &work[iu - 1], 
                               ldwrku, ZERO, a, lda );
            
                        //                    Copy left singular vectors of A from A to U
            
                        dlacpy( 'F', m, n, a, lda, u, ldu );
            
                    }
                    else { 
            
                        //                    Insufficient workspace for a fast algorithm
            
                        itau = 1;
                        iwork = itau + n;
            
                        //                    Compute A=Q*R, copying result to U
                        //                    (Workspace: need 2*N, prefer N+N*NB)
            
                        dgeqrf( m, n, a, lda, &work[itau - 1], &work[iwork - 1], 
                                lwork - iwork + 1, info );
                        dlacpy( 'L', m, n, a, lda, u, ldu );
            
                        //                    Generate Q in U
                        //                    (Workspace: need N+M, prefer N+M*NB)
            
                        dorgqr( m, m, n, u, ldu, &work[itau - 1], 
                                &work[iwork - 1], lwork - iwork + 1, info );
            
                        //                    Copy R from A to VT, zeroing out below it
            
                        dlacpy( 'U', n, n, a, lda, vt, ldvt );
                        dlaset( 'L', n - 1, n - 1, ZERO, ZERO, &VT(0,1), 
                                ldvt );
                        ie = itau;
                        itauq = ie + n;
                        itaup = itauq + n;
                        iwork = itaup + n;
            
                        //                    Bidiagonalize R in VT
                        //                    (Workspace: need 4*N, prefer 3*N+2*N*NB)
            
                        dgebrd( n, n, vt, ldvt, s, &work[ie - 1], 
                                &work[itauq - 1], &work[itaup - 1], &work[iwork - 1], 
                                lwork - iwork + 1, info );
            
                        //                    Multiply Q in U by left bidiagonalizing vectors
                        //                    in VT
                        //                    (Workspace: need 3*N+M, prefer 3*N+M*NB)
            
                        dormbr( 'Q', 'R', 'N', m, n, n, vt, ldvt, 
                                &work[itauq - 1], u, ldu, &work[iwork - 1], 
                                lwork - iwork + 1, info );
            
                        //                    Generate right bidiagonalizing vectors in VT
                        //                    (Workspace: need 4*N-1, prefer 3*N+(N-1)*NB)
            
                        dorgbr( 'P', n, n, n, vt, ldvt, &work[itaup - 1], 
                                &work[iwork - 1], lwork - iwork + 1, info );
                        iwork = ie + n;
            
                        //                    Perform bidiagonal QR iteration, computing left
                        //                    singular vectors of A in U and computing right
                        //                    singular vectors of A in VT
                        //                    (Workspace: need 5*N-4)
            
                        dbdsqr( 'U', n, n, m, 0, s, &work[ie - 1], 
                                vt, ldvt, u, ldu, dum, 1, &work[iwork - 1], 
                                info );
            
                    }
          
                }
        
            }
      
        }
        else { 
      
            //           M .LT. MNTHR
      
            //           Path 10 (M at least N, but not much larger)
            //           Reduce to bidiagonal form without QR decomposition
      
            ie = 1;
            itauq = ie + n;
            itaup = itauq + n;
            iwork = itaup + n;
      
            //           Bidiagonalize A
            //           (Workspace: need 3*N+M, prefer 3*N+(M+N)*NB)
      
            dgebrd( m, n, a, lda, s, &work[ie - 1], &work[itauq - 1], 
                    &work[itaup - 1], &work[iwork - 1], lwork - iwork + 1, 
                    info );
            if( wntuas ) { 
        
                //              If left singular vectors desired in U, copy result to U
                //              and generate left bidiagonalizing vectors in U
                //              (Workspace: need 3*N+NCU, prefer 3*N+NCU*NB)
        
                dlacpy( 'L', m, n, a, lda, u, ldu );
                if( wntus ) 
                    ncu = n;
                if( wntua ) 
                    ncu = m;
                dorgbr( 'Q', m, ncu, n, u, ldu, &work[itauq - 1], 
                        &work[iwork - 1], lwork - iwork + 1, info );
            }
            if( wntvas ) { 
        
                //              If right singular vectors desired in VT, copy result to
                //              VT and generate right bidiagonalizing vectors in VT
                //              (Workspace: need 4*N-1, prefer 3*N+(N-1)*NB)
        
                dlacpy( 'U', n, n, a, lda, vt, ldvt );
                dorgbr( 'P', n, n, n, vt, ldvt, &work[itaup - 1], 
                        &work[iwork - 1], lwork - iwork + 1, info );
            }
            if( wntuo ) { 
        
                //              If left singular vectors desired in A, generate left
                //              bidiagonalizing vectors in A
                //              (Workspace: need 4*N, prefer 3*N+N*NB)
        
                dorgbr( 'Q', m, n, n, a, lda, &work[itauq - 1], &work[iwork - 1], 
                        lwork - iwork + 1, info );
            }
            if( wntvo ) { 
        
                //              If right singular vectors desired in A, generate right
                //              bidiagonalizing vectors in A
                //              (Workspace: need 4*N-1, prefer 3*N+(N-1)*NB)
        
                dorgbr( 'P', n, n, n, a, lda, &work[itaup - 1], &work[iwork - 1], 
                        lwork - iwork + 1, info );
            }
            iwork = ie + n;
            if( wntuas || wntuo ) 
                nru = m;
            if( wntun ) 
                nru = 0;
            if( wntvas || wntvo ) 
                ncvt = n;
            if( wntvn ) 
                ncvt = 0;
            if( (!wntuo) && (!wntvo) ) { 
        
                //              Perform bidiagonal QR iteration, if desired, computing
                //              left singular vectors in U and computing right singular
                //              vectors in VT
                //              (Workspace: need 5*N-4)
        
                dbdsqr( 'U', n, ncvt, nru, 0, s, &work[ie - 1], vt, 
                        ldvt, u, ldu, dum, 1, &work[iwork - 1], info );
            }
            else if( (!wntuo) && wntvo ) { 
        
                //              Perform bidiagonal QR iteration, if desired, computing
                //              left singular vectors in U and computing right singular
                //              vectors in A
                //              (Workspace: need 5*N-4)
        
                dbdsqr( 'U', n, ncvt, nru, 0, s, &work[ie - 1], a, 
                        lda, u, ldu, dum, 1, &work[iwork - 1], info );
            }
            else { 
        
                //              Perform bidiagonal QR iteration, if desired, computing
                //              left singular vectors in A and computing right singular
                //              vectors in VT
                //              (Workspace: need 5*N-4)
        
                dbdsqr( 'U', n, ncvt, nru, 0, s, &work[ie - 1], vt, 
                        ldvt, a, lda, dum, 1, &work[iwork - 1], info );
            }
      
        }
    
    }
    else { 
    
        //        A has more columns than rows. If A has sufficiently more
        //        columns than rows, first reduce using the LQ decomposition (if
        //        sufficient workspace available)
    
        if( n >= mnthr ) { 
      
            if( wntvn ) { 
        
                //              Path 1t(N much larger than M, JOBVT='N')
                //              No right singular vectors to be computed
        
                itau = 1;
                iwork = itau + m;
        
                //              Compute A=L*Q
                //              (Workspace: need 2*M, prefer M+M*NB)
        
                dgelqf( m, n, a, lda, &work[itau - 1], &work[iwork - 1], 
                        lwork - iwork + 1, info );
        
                //              Zero out above L
        
                dlaset( 'U', m - 1, m - 1, ZERO, ZERO, &A(1,0), lda );
                ie = 1;
                itauq = ie + m;
                itaup = itauq + m;
                iwork = itaup + m;
        
                //              Bidiagonalize L in A
                //              (Workspace: need 4*M, prefer 3*M+2*M*NB)
        
                dgebrd( m, m, a, lda, s, &work[ie - 1], &work[itauq - 1], 
                        &work[itaup - 1], &work[iwork - 1], lwork - iwork + 
                        1, info );
                if( wntuo || wntuas ) { 
          
                    //                 If left singular vectors desired, generate Q
                    //                 (Workspace: need 4*M, prefer 3*M+M*NB)
          
                    dorgbr( 'Q', m, m, m, a, lda, &work[itauq - 1], 
                            &work[iwork - 1], lwork - iwork + 1, info );
                }
                iwork = ie + m;
                nru = 0;
                if( wntuo || wntuas ) 
                    nru = m;
        
                //              Perform bidiagonal QR iteration, computing left singular
                //              vectors of A in A if desired
                //              (Workspace: need 5*M-4)
        
                dbdsqr( 'U', m, 0, nru, 0, s, &work[ie - 1], dum, 
                        1, a, lda, dum, 1, &work[iwork - 1], info );
        
                //              If left singular vectors desired in U, copy them there
        
                if( wntuas ) 
                    dlacpy( 'F', m, m, a, lda, u, ldu );
        
            }
            else if( wntvo && wntun ) { 
        
                //              Path 2t(N much larger than M, JOBU='N', JOBVT='O')
                //              M right singular vectors to be overwritten on A and
                //              no left singular vectors to be computed
        
                if( lwork >= m*m + max( 4*m, 5*m - 4 ) ) { 
          
                    //                 Sufficient workspace for a fast algorithm
          
                    ir = 1;
                    if( lwork >= max( wrkbl, lda*n ) + lda*m ) { 
            
                        //                    WORK(IU) is LDA by N and WORK(IR) is LDA by M
            
                        ldwrku = lda;
                        chunk = n;
                        ldwrkr = lda;
                    }
                    else if( lwork >= max( wrkbl, lda*n ) + m*m ) { 
            
                        //                    WORK(IU) is LDA by N and WORK(IR) is M by M
            
                        ldwrku = lda;
                        chunk = n;
                        ldwrkr = m;
                    }
                    else { 
            
                        //                    WORK(IU) is M by CHUNK and WORK(IR) is M by M
            
                        ldwrku = m;
                        chunk = (lwork - m*m)/m;
                        ldwrkr = m;
                    }
                    itau = ir + ldwrkr*m;
                    iwork = itau + m;
          
                    //                 Compute A=L*Q
                    //                 (Workspace: need M*M+2*M, prefer M*M+M+M*NB)
          
                    dgelqf( m, n, a, lda, &work[itau - 1], &work[iwork - 1], 
                            lwork - iwork + 1, info );
          
                    //                 Copy L to WORK(IR) and zero out above it
          
                    dlacpy( 'L', m, m, a, lda, &work[ir - 1], ldwrkr );
                    dlaset( 'U', m - 1, m - 1, ZERO, ZERO, &work[ir + ldwrkr - 1], 
                            ldwrkr );
          
                    //                 Generate Q in A
                    //                 (Workspace: need M*M+2*M, prefer M*M+M+M*NB)
          
                    dorglq( m, n, m, a, lda, &work[itau - 1], &work[iwork - 1], 
                            lwork - iwork + 1, info );
                    ie = itau;
                    itauq = ie + m;
                    itaup = itauq + m;
                    iwork = itaup + m;
          
                    //                 Bidiagonalize L in WORK(IR)
                    //                 (Workspace: need M*M+4*M, prefer M*M+3*M+2*M*NB)
          
                    dgebrd( m, m, &work[ir - 1], ldwrkr, s, &work[ie - 1], 
                            &work[itauq - 1], &work[itaup - 1], &work[iwork - 1], 
                            lwork - iwork + 1, info );
          
                    //                 Generate right vectors bidiagonalizing L
                    //                 (Workspace: need M*M+4*M-1, prefer M*M+3*M+(M-1)*NB)
          
                    dorgbr( 'P', m, m, m, &work[ir - 1], ldwrkr, &work[itaup - 1], 
                            &work[iwork - 1], lwork - iwork + 1, info );
                    iwork = ie + m;
          
                    //                 Perform bidiagonal QR iteration, computing right
                    //                 singular vectors of L in WORK(IR)
                    //                 (Workspace: need M*M+5*M-4)
          
                    dbdsqr( 'U', m, m, 0, 0, s, &work[ie - 1], &work[ir - 1], 
                            ldwrkr, dum, 1, dum, 1, &work[iwork - 1], info );
                    iu = ie;
          
                    //                 Multiply right singular vectors of L in WORK(IR) by Q
                    //                 in A, storing result in WORK(IU) and copying to A
                    //                 (Workspace: need M*M+M, prefer M*M+M*N)
          
                    for( i = 1, i_ = i - 1, _do4=docnt(i,n,_do5 = chunk); _do4 > 0; i += _do5, i_ += _do5, _do4-- ) { 
                        blk = min( n - i + 1, chunk );
                        dgemm( 'N', 'N', m, blk, m, ONE, &work[ir - 1], 
                               ldwrkr, &A(i_,0), lda, ZERO, &work[iu - 1], 
                               ldwrku );
                        dlacpy( 'F', m, blk, &work[iu - 1], ldwrku, 
                                &A(i_,0), lda );
                    }
          
                }
                else { 
          
                    //                 Insufficient workspace for a fast algorithm
          
                    ie = 1;
                    itauq = ie + m;
                    itaup = itauq + m;
                    iwork = itaup + m;
          
                    //                 Bidiagonalize A
                    //                 (Workspace: need 3*M+N, prefer 3*M+(M+N)*NB)
          
                    dgebrd( m, n, a, lda, s, &work[ie - 1], &work[itauq - 1], 
                            &work[itaup - 1], &work[iwork - 1], lwork - iwork + 
                            1, info );
          
                    //                 Generate right vectors bidiagonalizing A
                    //                 (Workspace: need 4*M, prefer 3*M+M*NB)
          
                    dorgbr( 'P', m, n, m, a, lda, &work[itaup - 1], 
                            &work[iwork - 1], lwork - iwork + 1, info );
                    iwork = ie + m;
          
                    //                 Perform bidiagonal QR iteration, computing right
                    //                 singular vectors of A in A
                    //                 (Workspace: need 5*M-4)
          
                    dbdsqr( 'L', m, n, 0, 0, s, &work[ie - 1], a, 
                            lda, dum, 1, dum, 1, &work[iwork - 1], info );
          
                }
        
            }
            else if( wntvo && wntuas ) { 
        
                //              Path 3t(N much larger than M, JOBU='S' or 'A', JOBVT='O')
                //              M right singular vectors to be overwritten on A and
                //              M left singular vectors to be computed in U
        
                if( lwork >= m*m + max( 4*m, 5*m - 4 ) ) { 
          
                    //                 Sufficient workspace for a fast algorithm
          
                    ir = 1;
                    if( lwork >= max( wrkbl, lda*n ) + lda*m ) { 
            
                        //                    WORK(IU) is LDA by N and WORK(IR) is LDA by M
            
                        ldwrku = lda;
                        chunk = n;
                        ldwrkr = lda;
                    }
                    else if( lwork >= max( wrkbl, lda*n ) + m*m ) { 
            
                        //                    WORK(IU) is LDA by N and WORK(IR) is M by M
            
                        ldwrku = lda;
                        chunk = n;
                        ldwrkr = m;
                    }
                    else { 
            
                        //                    WORK(IU) is M by CHUNK and WORK(IR) is M by M
            
                        ldwrku = m;
                        chunk = (lwork - m*m)/m;
                        ldwrkr = m;
                    }
                    itau = ir + ldwrkr*m;
                    iwork = itau + m;
          
                    //                 Compute A=L*Q
                    //                 (Workspace: need M*M+2*M, prefer M*M+M+M*NB)
          
                    dgelqf( m, n, a, lda, &work[itau - 1], &work[iwork - 1], 
                            lwork - iwork + 1, info );
          
                    //                 Copy L to U, zeroing about above it
          
                    dlacpy( 'L', m, m, a, lda, u, ldu );
                    dlaset( 'U', m - 1, m - 1, ZERO, ZERO, &U(1,0), 
                            ldu );
          
                    //                 Generate Q in A
                    //                 (Workspace: need M*M+2*M, prefer M*M+M+M*NB)
          
                    dorglq( m, n, m, a, lda, &work[itau - 1], &work[iwork - 1], 
                            lwork - iwork + 1, info );
                    ie = itau;
                    itauq = ie + m;
                    itaup = itauq + m;
                    iwork = itaup + m;
          
                    //                 Bidiagonalize L in U, copying result to WORK(IR)
                    //                 (Workspace: need M*M+4*M, prefer M*M+3*M+2*M*NB)
          
                    dgebrd( m, m, u, ldu, s, &work[ie - 1], &work[itauq - 1], 
                            &work[itaup - 1], &work[iwork - 1], lwork - iwork + 
                            1, info );
                    dlacpy( 'U', m, m, u, ldu, &work[ir - 1], ldwrkr );
          
                    //                 Generate right vectors bidiagonalizing L in WORK(IR)
                    //                 (Workspace: need M*M+4*M-1, prefer M*M+3*M+(M-1)*NB)
          
                    dorgbr( 'P', m, m, m, &work[ir - 1], ldwrkr, &work[itaup - 1], 
                            &work[iwork - 1], lwork - iwork + 1, info );
          
                    //                 Generate left vectors bidiagonalizing L in U
                    //                 (Workspace: need M*M+4*M, prefer M*M+3*M+M*NB)
          
                    dorgbr( 'Q', m, m, m, u, ldu, &work[itauq - 1], 
                            &work[iwork - 1], lwork - iwork + 1, info );
                    iwork = ie + m;
          
                    //                 Perform bidiagonal QR iteration, computing left
                    //                 singular vectors of L in U, and computing right
                    //                 singular vectors of L in WORK(IR)
                    //                 (Workspace: need M*M+5*M-4)
          
                    dbdsqr( 'U', m, m, m, 0, s, &work[ie - 1], &work[ir - 1], 
                            ldwrkr, u, ldu, dum, 1, &work[iwork - 1], info );
                    iu = ie;
          
                    //                 Multiply right singular vectors of L in WORK(IR) by Q
                    //                 in A, storing result in WORK(IU) and copying to A
                    //                 (Workspace: need M*M+M, prefer M*M+M*N))
          
                    for( i = 1, i_ = i - 1, _do6=docnt(i,n,_do7 = chunk); _do6 > 0; i += _do7, i_ += _do7, _do6-- ) { 
                        blk = min( n - i + 1, chunk );
                        dgemm( 'N', 'N', m, blk, m, ONE, &work[ir - 1], 
                               ldwrkr, &A(i_,0), lda, ZERO, &work[iu - 1], 
                               ldwrku );
                        dlacpy( 'F', m, blk, &work[iu - 1], ldwrku, 
                                &A(i_,0), lda );
                    }
          
                }
                else { 
          
                    //                 Insufficient workspace for a fast algorithm
          
                    itau = 1;
                    iwork = itau + m;
          
                    //                 Compute A=L*Q
                    //                 (Workspace: need 2*M, prefer M+M*NB)
          
                    dgelqf( m, n, a, lda, &work[itau - 1], &work[iwork - 1], 
                            lwork - iwork + 1, info );
          
                    //                 Copy L to U, zeroing out above it
          
                    dlacpy( 'L', m, m, a, lda, u, ldu );
                    dlaset( 'U', m - 1, m - 1, ZERO, ZERO, &U(1,0), 
                            ldu );
          
                    //                 Generate Q in A
                    //                 (Workspace: need 2*M, prefer M+M*NB)
          
                    dorglq( m, n, m, a, lda, &work[itau - 1], &work[iwork - 1], 
                            lwork - iwork + 1, info );
                    ie = itau;
                    itauq = ie + m;
                    itaup = itauq + m;
                    iwork = itaup + m;
          
                    //                 Bidiagonalize L in U
                    //                 (Workspace: need 4*M, prefer 3*M+2*M*NB)
          
                    dgebrd( m, m, u, ldu, s, &work[ie - 1], &work[itauq - 1], 
                            &work[itaup - 1], &work[iwork - 1], lwork - iwork + 
                            1, info );
          
                    //                 Multiply right vectors bidiagonalizing L by Q in A
                    //                 (Workspace: need 3*M+N, prefer 3*M+N*NB)
          
                    dormbr( 'P', 'L', 'T', m, n, m, u, ldu, &work[itaup - 1], 
                            a, lda, &work[iwork - 1], lwork - iwork + 1, 
                            info );
          
                    //                 Generate left vectors bidiagonalizing L in U
                    //                 (Workspace: need 4*M, prefer 3*M+M*NB)
          
                    dorgbr( 'Q', m, m, m, u, ldu, &work[itauq - 1], 
                            &work[iwork - 1], lwork - iwork + 1, info );
                    iwork = ie + m;
          
                    //                 Perform bidiagonal QR iteration, computing left
                    //                 singular vectors of A in U and computing right
                    //                 singular vectors of A in A
                    //                 (Workspace: need 5*M-4)
          
                    dbdsqr( 'U', m, n, m, 0, s, &work[ie - 1], a, 
                            lda, u, ldu, dum, 1, &work[iwork - 1], info );
          
                }
        
            }
            else if( wntvs ) { 
        
                if( wntun ) { 
          
                    //                 Path 4t(N much larger than M, JOBU='N', JOBVT='S')
                    //                 M right singular vectors to be computed in VT and
                    //                 no left singular vectors to be computed
          
                    if( lwork >= m*m + max( 4*m, 5*m - 4 ) ) { 
            
                        //                    Sufficient workspace for a fast algorithm
            
                        ir = 1;
                        if( lwork >= wrkbl + lda*m ) { 
              
                            //                       WORK(IR) is LDA by M
              
                            ldwrkr = lda;
                        }
                        else { 
              
                            //                       WORK(IR) is M by M
              
                            ldwrkr = m;
                        }
                        itau = ir + ldwrkr*m;
                        iwork = itau + m;
            
                        //                    Compute A=L*Q
                        //                    (Workspace: need M*M+2*M, prefer M*M+M+M*NB)
            
                        dgelqf( m, n, a, lda, &work[itau - 1], &work[iwork - 1], 
                                lwork - iwork + 1, info );
            
                        //                    Copy L to WORK(IR), zeroing out above it
            
                        dlacpy( 'L', m, m, a, lda, &work[ir - 1], 
                                ldwrkr );
                        dlaset( 'U', m - 1, m - 1, ZERO, ZERO, &work[ir + ldwrkr - 1], 
                                ldwrkr );
            
                        //                    Generate Q in A
                        //                    (Workspace: need M*M+2*M, prefer M*M+M+M*NB)
            
                        dorglq( m, n, m, a, lda, &work[itau - 1], 
                                &work[iwork - 1], lwork - iwork + 1, info );
                        ie = itau;
                        itauq = ie + m;
                        itaup = itauq + m;
                        iwork = itaup + m;
            
                        //                    Bidiagonalize L in WORK(IR)
                        //                    (Workspace: need M*M+4*M, prefer M*M+3*M+2*M*NB)
            
                        dgebrd( m, m, &work[ir - 1], ldwrkr, s, &work[ie - 1], 
                                &work[itauq - 1], &work[itaup - 1], &work[iwork - 1], 
                                lwork - iwork + 1, info );
            
                        //                    Generate right vectors bidiagonalizing L in
                        //                    WORK(IR)
                        //                    (Workspace: need M*M+4*M, prefer M*M+3*M+(M-1)*NB)
            
                        dorgbr( 'P', m, m, m, &work[ir - 1], ldwrkr, 
                                &work[itaup - 1], &work[iwork - 1], lwork - 
                                iwork + 1, info );
                        iwork = ie + m;
            
                        //                    Perform bidiagonal QR iteration, computing right
                        //                    singular vectors of L in WORK(IR)
                        //                    (Workspace: need M*M+5*M-4)
            
                        dbdsqr( 'U', m, m, 0, 0, s, &work[ie - 1], 
                                &work[ir - 1], ldwrkr, dum, 1, dum, 1, &work[iwork - 1], 
                                info );
            
                        //                    Multiply right singular vectors of L in WORK(IR) by
                        //                    Q in A, storing result in VT
                        //                    (Workspace: need M*M)
            
                        dgemm( 'N', 'N', m, n, m, ONE, &work[ir - 1], 
                               ldwrkr, a, lda, ZERO, vt, ldvt );
            
                    }
                    else { 
            
                        //                    Insufficient workspace for a fast algorithm
            
                        itau = 1;
                        iwork = itau + m;
            
                        //                    Compute A=L*Q
                        //                    (Workspace: need 2*M, prefer M+M*NB)
            
                        dgelqf( m, n, a, lda, &work[itau - 1], &work[iwork - 1], 
                                lwork - iwork + 1, info );
            
                        //                    Copy result to VT
            
                        dlacpy( 'U', m, n, a, lda, vt, ldvt );
            
                        //                    Generate Q in VT
                        //                    (Workspace: need 2*M, prefer M+M*NB)
            
                        dorglq( m, n, m, vt, ldvt, &work[itau - 1], 
                                &work[iwork - 1], lwork - iwork + 1, info );
                        ie = itau;
                        itauq = ie + m;
                        itaup = itauq + m;
                        iwork = itaup + m;
            
                        //                    Zero out above L in A
            
                        dlaset( 'U', m - 1, m - 1, ZERO, ZERO, &A(1,0), 
                                lda );
            
                        //                    Bidiagonalize L in A
                        //                    (Workspace: need 4*M, prefer 3*M+2*M*NB)
            
                        dgebrd( m, m, a, lda, s, &work[ie - 1], &work[itauq - 1], 
                                &work[itaup - 1], &work[iwork - 1], lwork - 
                                iwork + 1, info );
            
                        //                    Multiply right vectors bidiagonalizing L by Q in VT
                        //                    (Workspace: need 3*M+N, prefer 3*M+N*NB)
            
                        dormbr( 'P', 'L', 'T', m, n, m, a, lda, &work[itaup - 1], 
                                vt, ldvt, &work[iwork - 1], lwork - iwork + 
                                1, info );
                        iwork = ie + m;
            
                        //                    Perform bidiagonal QR iteration, computing right
                        //                    singular vectors of A in VT
                        //                    (Workspace: need 5*M-4)
            
                        dbdsqr( 'U', m, n, 0, 0, s, &work[ie - 1], 
                                vt, ldvt, dum, 1, dum, 1, &work[iwork - 1], 
                                info );
            
                    }
          
                }
                else if( wntuo ) { 
          
                    //                 Path 5t(N much larger than M, JOBU='O', JOBVT='S')
                    //                 M right singular vectors to be computed in VT and
                    //                 M left singular vectors to be overwritten on A
          
                    if( lwork >= 2*m*m + max( 4*m, 5*m - 4 ) ) { 
            
                        //                    Sufficient workspace for a fast algorithm
            
                        iu = 1;
                        if( lwork >= wrkbl + 2*lda*m ) { 
              
                            //                       WORK(IU) is LDA by M and WORK(IR) is LDA by M
              
                            ldwrku = lda;
                            ir = iu + ldwrku*m;
                            ldwrkr = lda;
                        }
                        else if( lwork >= wrkbl + (lda + m)*m ) { 
              
                            //                       WORK(IU) is LDA by M and WORK(IR) is M by M
              
                            ldwrku = lda;
                            ir = iu + ldwrku*m;
                            ldwrkr = m;
                        }
                        else { 
              
                            //                       WORK(IU) is M by M and WORK(IR) is M by M
              
                            ldwrku = m;
                            ir = iu + ldwrku*m;
                            ldwrkr = m;
                        }
                        itau = ir + ldwrkr*m;
                        iwork = itau + m;
            
                        //                    Compute A=L*Q
                        //                    (Workspace: need 2*M*M+2*M, prefer 2*M*M+M+M*NB)
            
                        dgelqf( m, n, a, lda, &work[itau - 1], &work[iwork - 1], 
                                lwork - iwork + 1, info );
            
                        //                    Copy L to WORK(IU), zeroing out below it
            
                        dlacpy( 'L', m, m, a, lda, &work[iu - 1], 
                                ldwrku );
                        dlaset( 'U', m - 1, m - 1, ZERO, ZERO, &work[iu + ldwrku - 1], 
                                ldwrku );
            
                        //                    Generate Q in A
                        //                    (Workspace: need 2*M*M+2*M, prefer 2*M*M+M+M*NB)
            
                        dorglq( m, n, m, a, lda, &work[itau - 1], 
                                &work[iwork - 1], lwork - iwork + 1, info );
                        ie = itau;
                        itauq = ie + m;
                        itaup = itauq + m;
                        iwork = itaup + m;
            
                        //                    Bidiagonalize L in WORK(IU), copying result to
                        //                    WORK(IR)
                        //                    (Workspace: need 2*M*M+4*M,
                        //                                prefer 2*M*M+3*M+2*M*NB)
            
                        dgebrd( m, m, &work[iu - 1], ldwrku, s, &work[ie - 1], 
                                &work[itauq - 1], &work[itaup - 1], &work[iwork - 1], 
                                lwork - iwork + 1, info );
                        dlacpy( 'L', m, m, &work[iu - 1], ldwrku, 
                                &work[ir - 1], ldwrkr );
            
                        //                    Generate right bidiagonalizing vectors in WORK(IU)
                        //                    (Workspace: need 2*M*M+4*M-1,
                        //                                prefer 2*M*M+3*M+(M-1)*NB)
            
                        dorgbr( 'P', m, m, m, &work[iu - 1], ldwrku, 
                                &work[itaup - 1], &work[iwork - 1], lwork - 
                                iwork + 1, info );
            
                        //                    Generate left bidiagonalizing vectors in WORK(IR)
                        //                    (Workspace: need 2*M*M+4*M, prefer 2*M*M+3*M+M*NB)
            
                        dorgbr( 'Q', m, m, m, &work[ir - 1], ldwrkr, 
                                &work[itauq - 1], &work[iwork - 1], lwork - 
                                iwork + 1, info );
                        iwork = ie + m;
            
                        //                    Perform bidiagonal QR iteration, computing left
                        //                    singular vectors of L in WORK(IR) and computing
                        //                    right singular vectors of L in WORK(IU)
                        //                    (Workspace: need 2*M*M+5*M-4)
            
                        dbdsqr( 'U', m, m, m, 0, s, &work[ie - 1], 
                                &work[iu - 1], ldwrku, &work[ir - 1], ldwrkr, 
                                dum, 1, &work[iwork - 1], info );
            
                        //                    Multiply right singular vectors of L in WORK(IU) by
                        //                    Q in A, storing result in VT
                        //                    (Workspace: need M*M)
            
                        dgemm( 'N', 'N', m, n, m, ONE, &work[iu - 1], 
                               ldwrku, a, lda, ZERO, vt, ldvt );
            
                        //                    Copy left singular vectors of L to A
                        //                    (Workspace: need M*M)
            
                        dlacpy( 'F', m, m, &work[ir - 1], ldwrkr, 
                                a, lda );
            
                    }
                    else { 
            
                        //                    Insufficient workspace for a fast algorithm
            
                        itau = 1;
                        iwork = itau + m;
            
                        //                    Compute A=L*Q, copying result to VT
                        //                    (Workspace: need 2*M, prefer M+M*NB)
            
                        dgelqf( m, n, a, lda, &work[itau - 1], &work[iwork - 1], 
                                lwork - iwork + 1, info );
                        dlacpy( 'U', m, n, a, lda, vt, ldvt );
            
                        //                    Generate Q in VT
                        //                    (Workspace: need 2*M, prefer M+M*NB)
            
                        dorglq( m, n, m, vt, ldvt, &work[itau - 1], 
                                &work[iwork - 1], lwork - iwork + 1, info );
                        ie = itau;
                        itauq = ie + m;
                        itaup = itauq + m;
                        iwork = itaup + m;
            
                        //                    Zero out above L in A
            
                        dlaset( 'U', m - 1, m - 1, ZERO, ZERO, &A(1,0), 
                                lda );
            
                        //                    Bidiagonalize L in A
                        //                    (Workspace: need 4*M, prefer 3*M+2*M*NB)
            
                        dgebrd( m, m, a, lda, s, &work[ie - 1], &work[itauq - 1], 
                                &work[itaup - 1], &work[iwork - 1], lwork - 
                                iwork + 1, info );
            
                        //                    Multiply right vectors bidiagonalizing L by Q in VT
                        //                    (Workspace: need 3*M+N, prefer 3*M+N*NB)
            
                        dormbr( 'P', 'L', 'T', m, n, m, a, lda, &work[itaup - 1], 
                                vt, ldvt, &work[iwork - 1], lwork - iwork + 
                                1, info );
            
                        //                    Generate left bidiagonalizing vectors of L in A
                        //                    (Workspace: need 4*M, prefer 3*M+M*NB)
            
                        dorgbr( 'Q', m, m, m, a, lda, &work[itauq - 1], 
                                &work[iwork - 1], lwork - iwork + 1, info );
                        iwork = ie + m;
            
                        //                    Perform bidiagonal QR iteration, compute left
                        //                    singular vectors of A in A and compute right
                        //                    singular vectors of A in VT
                        //                    (Workspace: need 5*M-4)
            
                        dbdsqr( 'U', m, n, m, 0, s, &work[ie - 1], 
                                vt, ldvt, a, lda, dum, 1, &work[iwork - 1], 
                                info );
            
                    }
          
                }
                else if( wntuas ) { 
          
                    //                 Path 6t(N much larger than M, JOBU='S' or 'A',
                    //                         JOBVT='S')
                    //                 M right singular vectors to be computed in VT and
                    //                 M left singular vectors to be computed in U
          
                    if( lwork >= m*m + max( 4*m, 5*m - 4 ) ) { 
            
                        //                    Sufficient workspace for a fast algorithm
            
                        iu = 1;
                        if( lwork >= wrkbl + lda*m ) { 
              
                            //                       WORK(IU) is LDA by N
              
                            ldwrku = lda;
                        }
                        else { 
              
                            //                       WORK(IU) is LDA by M
              
                            ldwrku = m;
                        }
                        itau = iu + ldwrku*m;
                        iwork = itau + m;
            
                        //                    Compute A=L*Q
                        //                    (Workspace: need M*M+2*M, prefer M*M+M+M*NB)
            
                        dgelqf( m, n, a, lda, &work[itau - 1], &work[iwork - 1], 
                                lwork - iwork + 1, info );
            
                        //                    Copy L to WORK(IU), zeroing out above it
            
                        dlacpy( 'L', m, m, a, lda, &work[iu - 1], 
                                ldwrku );
                        dlaset( 'U', m - 1, m - 1, ZERO, ZERO, &work[iu + ldwrku - 1], 
                                ldwrku );
            
                        //                    Generate Q in A
                        //                    (Workspace: need M*M+2*M, prefer M*M+M+M*NB)
            
                        dorglq( m, n, m, a, lda, &work[itau - 1], 
                                &work[iwork - 1], lwork - iwork + 1, info );
                        ie = itau;
                        itauq = ie + m;
                        itaup = itauq + m;
                        iwork = itaup + m;
            
                        //                    Bidiagonalize L in WORK(IU), copying result to U
                        //                    (Workspace: need M*M+4*M, prefer M*M+3*M+2*M*NB)
            
                        dgebrd( m, m, &work[iu - 1], ldwrku, s, &work[ie - 1], 
                                &work[itauq - 1], &work[itaup - 1], &work[iwork - 1], 
                                lwork - iwork + 1, info );
                        dlacpy( 'L', m, m, &work[iu - 1], ldwrku, 
                                u, ldu );
            
                        //                    Generate right bidiagonalizing vectors in WORK(IU)
                        //                    (Workspace: need M*M+4*M-1,
                        //                                prefer M*M+3*M+(M-1)*NB)
            
                        dorgbr( 'P', m, m, m, &work[iu - 1], ldwrku, 
                                &work[itaup - 1], &work[iwork - 1], lwork - 
                                iwork + 1, info );
            
                        //                    Generate left bidiagonalizing vectors in U
                        //                    (Workspace: need M*M+4*M, prefer M*M+3*M+M*NB)
            
                        dorgbr( 'Q', m, m, m, u, ldu, &work[itauq - 1], 
                                &work[iwork - 1], lwork - iwork + 1, info );
                        iwork = ie + m;
            
                        //                    Perform bidiagonal QR iteration, computing left
                        //                    singular vectors of L in U and computing right
                        //                    singular vectors of L in WORK(IU)
                        //                    (Workspace: need M*M+5*M-4)
            
                        dbdsqr( 'U', m, m, m, 0, s, &work[ie - 1], 
                                &work[iu - 1], ldwrku, u, ldu, dum, 1, &work[iwork - 1], 
                                info );
            
                        //                    Multiply right singular vectors of L in WORK(IU) by
                        //                    Q in A, storing result in VT
                        //                    (Workspace: need M*M)
            
                        dgemm( 'N', 'N', m, n, m, ONE, &work[iu - 1], 
                               ldwrku, a, lda, ZERO, vt, ldvt );
            
                    }
                    else { 
            
                        //                    Insufficient workspace for a fast algorithm
            
                        itau = 1;
                        iwork = itau + m;
            
                        //                    Compute A=L*Q, copying result to VT
                        //                    (Workspace: need 2*M, prefer M+M*NB)
            
                        dgelqf( m, n, a, lda, &work[itau - 1], &work[iwork - 1], 
                                lwork - iwork + 1, info );
                        dlacpy( 'U', m, n, a, lda, vt, ldvt );
            
                        //                    Generate Q in VT
                        //                    (Workspace: need 2*M, prefer M+M*NB)
            
                        dorglq( m, n, m, vt, ldvt, &work[itau - 1], 
                                &work[iwork - 1], lwork - iwork + 1, info );
            
                        //                    Copy L to U, zeroing out above it
            
                        dlacpy( 'L', m, m, a, lda, u, ldu );
                        dlaset( 'U', m - 1, m - 1, ZERO, ZERO, &U(1,0), 
                                ldu );
                        ie = itau;
                        itauq = ie + m;
                        itaup = itauq + m;
                        iwork = itaup + m;
            
                        //                    Bidiagonalize L in U
                        //                    (Workspace: need 4*M, prefer 3*M+2*M*NB)
            
                        dgebrd( m, m, u, ldu, s, &work[ie - 1], &work[itauq - 1], 
                                &work[itaup - 1], &work[iwork - 1], lwork - 
                                iwork + 1, info );
            
                        //                    Multiply right bidiagonalizing vectors in U by Q
                        //                    in VT
                        //                    (Workspace: need 3*M+N, prefer 3*M+N*NB)
            
                        dormbr( 'P', 'L', 'T', m, n, m, u, ldu, &work[itaup - 1], 
                                vt, ldvt, &work[iwork - 1], lwork - iwork + 
                                1, info );
            
                        //                    Generate left bidiagonalizing vectors in U
                        //                    (Workspace: need 4*M, prefer 3*M+M*NB)
            
                        dorgbr( 'Q', m, m, m, u, ldu, &work[itauq - 1], 
                                &work[iwork - 1], lwork - iwork + 1, info );
                        iwork = ie + m;
            
                        //                    Perform bidiagonal QR iteration, computing left
                        //                    singular vectors of A in U and computing right
                        //                    singular vectors of A in VT
                        //                    (Workspace: need 5*M-4)
            
                        dbdsqr( 'U', m, n, m, 0, s, &work[ie - 1], 
                                vt, ldvt, u, ldu, dum, 1, &work[iwork - 1], 
                                info );
            
                    }
          
                }
        
            }
            else if( wntva ) { 
        
                if( wntun ) { 
          
                    //                 Path 7t(N much larger than M, JOBU='N', JOBVT='A')
                    //                 N right singular vectors to be computed in VT and
                    //                 no left singular vectors to be computed
          
                    if( lwork >= m*m + vmax( n + m, 4*m, 5*m - 4, 
                                             IEND ) ) { 
            
                        //                    Sufficient workspace for a fast algorithm
            
                        ir = 1;
                        if( lwork >= wrkbl + lda*m ) { 
              
                            //                       WORK(IR) is LDA by M
              
                            ldwrkr = lda;
                        }
                        else { 
              
                            //                       WORK(IR) is M by M
              
                            ldwrkr = m;
                        }
                        itau = ir + ldwrkr*m;
                        iwork = itau + m;
            
                        //                    Compute A=L*Q, copying result to VT
                        //                    (Workspace: need M*M+2*M, prefer M*M+M+M*NB)
            
                        dgelqf( m, n, a, lda, &work[itau - 1], &work[iwork - 1], 
                                lwork - iwork + 1, info );
                        dlacpy( 'U', m, n, a, lda, vt, ldvt );
            
                        //                    Copy L to WORK(IR), zeroing out above it
            
                        dlacpy( 'L', m, m, a, lda, &work[ir - 1], 
                                ldwrkr );
                        dlaset( 'U', m - 1, m - 1, ZERO, ZERO, &work[ir + ldwrkr - 1], 
                                ldwrkr );
            
                        //                    Generate Q in VT
                        //                    (Workspace: need M*M+M+N, prefer M*M+M+N*NB)
            
                        dorglq( n, n, m, vt, ldvt, &work[itau - 1], 
                                &work[iwork - 1], lwork - iwork + 1, info );
                        ie = itau;
                        itauq = ie + m;
                        itaup = itauq + m;
                        iwork = itaup + m;
            
                        //                    Bidiagonalize L in WORK(IR)
                        //                    (Workspace: need M*M+4*M, prefer M*M+3*M+2*M*NB)
            
                        dgebrd( m, m, &work[ir - 1], ldwrkr, s, &work[ie - 1], 
                                &work[itauq - 1], &work[itaup - 1], &work[iwork - 1], 
                                lwork - iwork + 1, info );
            
                        //                    Generate right bidiagonalizing vectors in WORK(IR)
                        //                    (Workspace: need M*M+4*M-1,
                        //                                prefer M*M+3*M+(M-1)*NB)
            
                        dorgbr( 'P', m, m, m, &work[ir - 1], ldwrkr, 
                                &work[itaup - 1], &work[iwork - 1], lwork - 
                                iwork + 1, info );
                        iwork = ie + m;
            
                        //                    Perform bidiagonal QR iteration, computing right
                        //                    singular vectors of L in WORK(IR)
                        //                    (Workspace: need M*M+5*M-4)
            
                        dbdsqr( 'U', m, m, 0, 0, s, &work[ie - 1], 
                                &work[ir - 1], ldwrkr, dum, 1, dum, 1, &work[iwork - 1], 
                                info );
            
                        //                    Multiply right singular vectors of L in WORK(IR) by
                        //                    Q in VT, storing result in A
                        //                    (Workspace: need M*M)
            
                        dgemm( 'N', 'N', m, n, m, ONE, &work[ir - 1], 
                               ldwrkr, vt, ldvt, ZERO, a, lda );
            
                        //                    Copy right singular vectors of A from A to VT
            
                        dlacpy( 'F', m, n, a, lda, vt, ldvt );
            
                    }
                    else { 
            
                        //                    Insufficient workspace for a fast algorithm
            
                        itau = 1;
                        iwork = itau + m;
            
                        //                    Compute A=L*Q, copying result to VT
                        //                    (Workspace: need 2*M, prefer M+M*NB)
            
                        dgelqf( m, n, a, lda, &work[itau - 1], &work[iwork - 1], 
                                lwork - iwork + 1, info );
                        dlacpy( 'U', m, n, a, lda, vt, ldvt );
            
                        //                    Generate Q in VT
                        //                    (Workspace: need M+N, prefer M+N*NB)
            
                        dorglq( n, n, m, vt, ldvt, &work[itau - 1], 
                                &work[iwork - 1], lwork - iwork + 1, info );
                        ie = itau;
                        itauq = ie + m;
                        itaup = itauq + m;
                        iwork = itaup + m;
            
                        //                    Zero out above L in A
            
                        dlaset( 'U', m - 1, m - 1, ZERO, ZERO, &A(1,0), 
                                lda );
            
                        //                    Bidiagonalize L in A
                        //                    (Workspace: need 4*M, prefer 3*M+2*M*NB)
            
                        dgebrd( m, m, a, lda, s, &work[ie - 1], &work[itauq - 1], 
                                &work[itaup - 1], &work[iwork - 1], lwork - 
                                iwork + 1, info );
            
                        //                    Multiply right bidiagonalizing vectors in A by Q
                        //                    in VT
                        //                    (Workspace: need 3*M+N, prefer 3*M+N*NB)
            
                        dormbr( 'P', 'L', 'T', m, n, m, a, lda, &work[itaup - 1], 
                                vt, ldvt, &work[iwork - 1], lwork - iwork + 
                                1, info );
                        iwork = ie + m;
            
                        //                    Perform bidiagonal QR iteration, computing right
                        //                    singular vectors of A in VT
                        //                    (Workspace: need 5*M-4)
            
                        dbdsqr( 'U', m, n, 0, 0, s, &work[ie - 1], 
                                vt, ldvt, dum, 1, dum, 1, &work[iwork - 1], 
                                info );
            
                    }
          
                }
                else if( wntuo ) { 
          
                    //                 Path 8t(N much larger than M, JOBU='O', JOBVT='A')
                    //                 N right singular vectors to be computed in VT and
                    //                 M left singular vectors to be overwritten on A
          
                    if( lwork >= 2*m*m + vmax( n + m, 4*m, 5*m - 4, 
                                               IEND ) ) { 
            
                        //                    Sufficient workspace for a fast algorithm
            
                        iu = 1;
                        if( lwork >= wrkbl + 2*lda*m ) { 
              
                            //                       WORK(IU) is LDA by M and WORK(IR) is LDA by M
              
                            ldwrku = lda;
                            ir = iu + ldwrku*m;
                            ldwrkr = lda;
                        }
                        else if( lwork >= wrkbl + (lda + m)*m ) { 
              
                            //                       WORK(IU) is LDA by M and WORK(IR) is M by M
              
                            ldwrku = lda;
                            ir = iu + ldwrku*m;
                            ldwrkr = m;
                        }
                        else { 
              
                            //                       WORK(IU) is M by M and WORK(IR) is M by M
              
                            ldwrku = m;
                            ir = iu + ldwrku*m;
                            ldwrkr = m;
                        }
                        itau = ir + ldwrkr*m;
                        iwork = itau + m;
            
                        //                    Compute A=L*Q, copying result to VT
                        //                    (Workspace: need 2*M*M+2*M, prefer 2*M*M+M+M*NB)
            
                        dgelqf( m, n, a, lda, &work[itau - 1], &work[iwork - 1], 
                                lwork - iwork + 1, info );
                        dlacpy( 'U', m, n, a, lda, vt, ldvt );
            
                        //                    Generate Q in VT
                        //                    (Workspace: need 2*M*M+M+N, prefer 2*M*M+M+N*NB)
            
                        dorglq( n, n, m, vt, ldvt, &work[itau - 1], 
                                &work[iwork - 1], lwork - iwork + 1, info );
            
                        //                    Copy L to WORK(IU), zeroing out above it
            
                        dlacpy( 'L', m, m, a, lda, &work[iu - 1], 
                                ldwrku );
                        dlaset( 'U', m - 1, m - 1, ZERO, ZERO, &work[iu + ldwrku - 1], 
                                ldwrku );
                        ie = itau;
                        itauq = ie + m;
                        itaup = itauq + m;
                        iwork = itaup + m;
            
                        //                    Bidiagonalize L in WORK(IU), copying result to
                        //                    WORK(IR)
                        //                    (Workspace: need 2*M*M+4*M,
                        //                                prefer 2*M*M+3*M+2*M*NB)
            
                        dgebrd( m, m, &work[iu - 1], ldwrku, s, &work[ie - 1], 
                                &work[itauq - 1], &work[itaup - 1], &work[iwork - 1], 
                                lwork - iwork + 1, info );
                        dlacpy( 'L', m, m, &work[iu - 1], ldwrku, 
                                &work[ir - 1], ldwrkr );
            
                        //                    Generate right bidiagonalizing vectors in WORK(IU)
                        //                    (Workspace: need 2*M*M+4*M-1,
                        //                                prefer 2*M*M+3*M+(M-1)*NB)
            
                        dorgbr( 'P', m, m, m, &work[iu - 1], ldwrku, 
                                &work[itaup - 1], &work[iwork - 1], lwork - 
                                iwork + 1, info );
            
                        //                    Generate left bidiagonalizing vectors in WORK(IR)
                        //                    (Workspace: need 2*M*M+4*M, prefer 2*M*M+3*M+M*NB)
            
                        dorgbr( 'Q', m, m, m, &work[ir - 1], ldwrkr, 
                                &work[itauq - 1], &work[iwork - 1], lwork - 
                                iwork + 1, info );
                        iwork = ie + m;
            
                        //                    Perform bidiagonal QR iteration, computing left
                        //                    singular vectors of L in WORK(IR) and computing
                        //                    right singular vectors of L in WORK(IU)
                        //                    (Workspace: need 2*M*M+5*M-4)
            
                        dbdsqr( 'U', m, m, m, 0, s, &work[ie - 1], 
                                &work[iu - 1], ldwrku, &work[ir - 1], ldwrkr, 
                                dum, 1, &work[iwork - 1], info );
            
                        //                    Multiply right singular vectors of L in WORK(IU) by
                        //                    Q in VT, storing result in A
                        //                    (Workspace: need M*M)
            
                        dgemm( 'N', 'N', m, n, m, ONE, &work[iu - 1], 
                               ldwrku, vt, ldvt, ZERO, a, lda );
            
                        //                    Copy right singular vectors of A from A to VT
            
                        dlacpy( 'F', m, n, a, lda, vt, ldvt );
            
                        //                    Copy left singular vectors of A from WORK(IR) to A
            
                        dlacpy( 'F', m, m, &work[ir - 1], ldwrkr, 
                                a, lda );
            
                    }
                    else { 
            
                        //                    Insufficient workspace for a fast algorithm
            
                        itau = 1;
                        iwork = itau + m;
            
                        //                    Compute A=L*Q, copying result to VT
                        //                    (Workspace: need 2*M, prefer M+M*NB)
            
                        dgelqf( m, n, a, lda, &work[itau - 1], &work[iwork - 1], 
                                lwork - iwork + 1, info );
                        dlacpy( 'U', m, n, a, lda, vt, ldvt );
            
                        //                    Generate Q in VT
                        //                    (Workspace: need M+N, prefer M+N*NB)
            
                        dorglq( n, n, m, vt, ldvt, &work[itau - 1], 
                                &work[iwork - 1], lwork - iwork + 1, info );
                        ie = itau;
                        itauq = ie + m;
                        itaup = itauq + m;
                        iwork = itaup + m;
            
                        //                    Zero out above L in A
            
                        dlaset( 'U', m - 1, m - 1, ZERO, ZERO, &A(1,0), 
                                lda );
            
                        //                    Bidiagonalize L in A
                        //                    (Workspace: need 4*M, prefer 3*M+2*M*NB)
            
                        dgebrd( m, m, a, lda, s, &work[ie - 1], &work[itauq - 1], 
                                &work[itaup - 1], &work[iwork - 1], lwork - 
                                iwork + 1, info );
            
                        //                    Multiply right bidiagonalizing vectors in A by Q
                        //                    in VT
                        //                    (Workspace: need 3*M+N, prefer 3*M+N*NB)
            
                        dormbr( 'P', 'L', 'T', m, n, m, a, lda, &work[itaup - 1], 
                                vt, ldvt, &work[iwork - 1], lwork - iwork + 
                                1, info );
            
                        //                    Generate left bidiagonalizing vectors in A
                        //                    (Workspace: need 4*M, prefer 3*M+M*NB)
            
                        dorgbr( 'Q', m, m, m, a, lda, &work[itauq - 1], 
                                &work[iwork - 1], lwork - iwork + 1, info );
                        iwork = ie + m;
            
                        //                    Perform bidiagonal QR iteration, computing left
                        //                    singular vectors of A in A and computing right
                        //                    singular vectors of A in VT
                        //                    (Workspace: need 5*M-4)
            
                        dbdsqr( 'U', m, n, m, 0, s, &work[ie - 1], 
                                vt, ldvt, a, lda, dum, 1, &work[iwork - 1], 
                                info );
            
                    }
          
                }
                else if( wntuas ) { 
          
                    //                 Path 9t(N much larger than M, JOBU='S' or 'A',
                    //                         JOBVT='A')
                    //                 N right singular vectors to be computed in VT and
                    //                 M left singular vectors to be computed in U
          
                    if( lwork >= m*m + vmax( n + m, 4*m, 5*m - 4, 
                                             IEND ) ) { 
            
                        //                    Sufficient workspace for a fast algorithm
            
                        iu = 1;
                        if( lwork >= wrkbl + lda*m ) { 
              
                            //                       WORK(IU) is LDA by M
              
                            ldwrku = lda;
                        }
                        else { 
              
                            //                       WORK(IU) is M by M
              
                            ldwrku = m;
                        }
                        itau = iu + ldwrku*m;
                        iwork = itau + m;
            
                        //                    Compute A=L*Q, copying result to VT
                        //                    (Workspace: need M*M+2*M, prefer M*M+M+M*NB)
            
                        dgelqf( m, n, a, lda, &work[itau - 1], &work[iwork - 1], 
                                lwork - iwork + 1, info );
                        dlacpy( 'U', m, n, a, lda, vt, ldvt );
            
                        //                    Generate Q in VT
                        //                    (Workspace: need M*M+M+N, prefer M*M+M+N*NB)
            
                        dorglq( n, n, m, vt, ldvt, &work[itau - 1], 
                                &work[iwork - 1], lwork - iwork + 1, info );
            
                        //                    Copy L to WORK(IU), zeroing out above it
            
                        dlacpy( 'L', m, m, a, lda, &work[iu - 1], 
                                ldwrku );
                        dlaset( 'U', m - 1, m - 1, ZERO, ZERO, &work[iu + ldwrku - 1], 
                                ldwrku );
                        ie = itau;
                        itauq = ie + m;
                        itaup = itauq + m;
                        iwork = itaup + m;
            
                        //                    Bidiagonalize L in WORK(IU), copying result to U
                        //                    (Workspace: need M*M+4*M, prefer M*M+3*M+2*M*NB)
            
                        dgebrd( m, m, &work[iu - 1], ldwrku, s, &work[ie - 1], 
                                &work[itauq - 1], &work[itaup - 1], &work[iwork - 1], 
                                lwork - iwork + 1, info );
                        dlacpy( 'L', m, m, &work[iu - 1], ldwrku, 
                                u, ldu );
            
                        //                    Generate right bidiagonalizing vectors in WORK(IU)
                        //                    (Workspace: need M*M+4*M, prefer M*M+3*M+(M-1)*NB)
            
                        dorgbr( 'P', m, m, m, &work[iu - 1], ldwrku, 
                                &work[itaup - 1], &work[iwork - 1], lwork - 
                                iwork + 1, info );
            
                        //                    Generate left bidiagonalizing vectors in U
                        //                    (Workspace: need M*M+4*M, prefer M*M+3*M+M*NB)
            
                        dorgbr( 'Q', m, m, m, u, ldu, &work[itauq - 1], 
                                &work[iwork - 1], lwork - iwork + 1, info );
                        iwork = ie + m;
            
                        //                    Perform bidiagonal QR iteration, computing left
                        //                    singular vectors of L in U and computing right
                        //                    singular vectors of L in WORK(IU)
                        //                    (Workspace: need M*M+5*M-4)
            
                        dbdsqr( 'U', m, m, m, 0, s, &work[ie - 1], 
                                &work[iu - 1], ldwrku, u, ldu, dum, 1, &work[iwork - 1], 
                                info );
            
                        //                    Multiply right singular vectors of L in WORK(IU) by
                        //                    Q in VT, storing result in A
                        //                    (Workspace: need M*M)
            
                        dgemm( 'N', 'N', m, n, m, ONE, &work[iu - 1], 
                               ldwrku, vt, ldvt, ZERO, a, lda );
            
                        //                    Copy right singular vectors of A from A to VT
            
                        dlacpy( 'F', m, n, a, lda, vt, ldvt );
            
                    }
                    else { 
            
                        //                    Insufficient workspace for a fast algorithm
            
                        itau = 1;
                        iwork = itau + m;
            
                        //                    Compute A=L*Q, copying result to VT
                        //                    (Workspace: need 2*M, prefer M+M*NB)
            
                        dgelqf( m, n, a, lda, &work[itau - 1], &work[iwork - 1], 
                                lwork - iwork + 1, info );
                        dlacpy( 'U', m, n, a, lda, vt, ldvt );
            
                        //                    Generate Q in VT
                        //                    (Workspace: need M+N, prefer M+N*NB)
            
                        dorglq( n, n, m, vt, ldvt, &work[itau - 1], 
                                &work[iwork - 1], lwork - iwork + 1, info );
            
                        //                    Copy L to U, zeroing out above it
            
                        dlacpy( 'L', m, m, a, lda, u, ldu );
                        dlaset( 'U', m - 1, m - 1, ZERO, ZERO, &U(1,0), 
                                ldu );
                        ie = itau;
                        itauq = ie + m;
                        itaup = itauq + m;
                        iwork = itaup + m;
            
                        //                    Bidiagonalize L in U
                        //                    (Workspace: need 4*M, prefer 3*M+2*M*NB)
            
                        dgebrd( m, m, u, ldu, s, &work[ie - 1], &work[itauq - 1], 
                                &work[itaup - 1], &work[iwork - 1], lwork - 
                                iwork + 1, info );
            
                        //                    Multiply right bidiagonalizing vectors in U by Q
                        //                    in VT
                        //                    (Workspace: need 3*M+N, prefer 3*M+N*NB)
            
                        dormbr( 'P', 'L', 'T', m, n, m, u, ldu, &work[itaup - 1], 
                                vt, ldvt, &work[iwork - 1], lwork - iwork + 
                                1, info );
            
                        //                    Generate left bidiagonalizing vectors in U
                        //                    (Workspace: need 4*M, prefer 3*M+M*NB)
            
                        dorgbr( 'Q', m, m, m, u, ldu, &work[itauq - 1], 
                                &work[iwork - 1], lwork - iwork + 1, info );
                        iwork = ie + m;
            
                        //                    Perform bidiagonal QR iteration, computing left
                        //                    singular vectors of A in U and computing right
                        //                    singular vectors of A in VT
                        //                    (Workspace: need 5*M-4)
            
                        dbdsqr( 'U', m, n, m, 0, s, &work[ie - 1], 
                                vt, ldvt, u, ldu, dum, 1, &work[iwork - 1], 
                                info );
            
                    }
          
                }
        
            }
      
        }
        else { 
      
            //           N .LT. MNTHR
      
            //           Path 10t(N greater than M, but not much larger)
            //           Reduce to bidiagonal form without LQ decomposition
      
            ie = 1;
            itauq = ie + m;
            itaup = itauq + m;
            iwork = itaup + m;
      
            //           Bidiagonalize A
            //           (Workspace: need 3*M+N, prefer 3*M+(M+N)*NB)
      
            dgebrd( m, n, a, lda, s, &work[ie - 1], &work[itauq - 1], 
                    &work[itaup - 1], &work[iwork - 1], lwork - iwork + 1, 
                    info );
            if( wntuas ) { 
        
                //              If left singular vectors desired in U, copy result to U
                //              and generate left bidiagonalizing vectors in U
                //              (Workspace: need 4*M-1, prefer 3*M+(M-1)*NB)
        
                dlacpy( 'L', m, m, a, lda, u, ldu );
                dorgbr( 'Q', m, m, n, u, ldu, &work[itauq - 1], &work[iwork - 1], 
                        lwork - iwork + 1, info );
            }
            if( wntvas ) { 
        
                //              If right singular vectors desired in VT, copy result to
                //              VT and generate right bidiagonalizing vectors in VT
                //              (Workspace: need 3*M+NRVT, prefer 3*M+NRVT*NB)
        
                dlacpy( 'U', m, n, a, lda, vt, ldvt );
                if( wntva ) 
                    nrvt = n;
                if( wntvs ) 
                    nrvt = m;
                dorgbr( 'P', nrvt, n, m, vt, ldvt, &work[itaup - 1], 
                        &work[iwork - 1], lwork - iwork + 1, info );
            }
            if( wntuo ) { 
        
                //              If left singular vectors desired in A, generate left
                //              bidiagonalizing vectors in A
                //              (Workspace: need 4*M-1, prefer 3*M+(M-1)*NB)
        
                dorgbr( 'Q', m, m, n, a, lda, &work[itauq - 1], &work[iwork - 1], 
                        lwork - iwork + 1, info );
            }
            if( wntvo ) { 
        
                //              If right singular vectors desired in A, generate right
                //              bidiagonalizing vectors in A
                //              (Workspace: need 4*M, prefer 3*M+M*NB)
        
                dorgbr( 'P', m, n, m, a, lda, &work[itaup - 1], &work[iwork - 1], 
                        lwork - iwork + 1, info );
            }
            iwork = ie + m;
            if( wntuas || wntuo ) 
                nru = m;
            if( wntun ) 
                nru = 0;
            if( wntvas || wntvo ) 
                ncvt = n;
            if( wntvn ) 
                ncvt = 0;
            if( (!wntuo) && (!wntvo) ) { 
        
                //              Perform bidiagonal QR iteration, if desired, computing
                //              left singular vectors in U and computing right singular
                //              vectors in VT
                //              (Workspace: need 5*M-4)
        
                dbdsqr( 'L', m, ncvt, nru, 0, s, &work[ie - 1], vt, 
                        ldvt, u, ldu, dum, 1, &work[iwork - 1], info );
            }
            else if( (!wntuo) && wntvo ) { 
        
                //              Perform bidiagonal QR iteration, if desired, computing
                //              left singular vectors in U and computing right singular
                //              vectors in A
                //              (Workspace: need 5*M-4)
        
                dbdsqr( 'L', m, ncvt, nru, 0, s, &work[ie - 1], a, 
                        lda, u, ldu, dum, 1, &work[iwork - 1], info );
            }
            else { 
        
                //              Perform bidiagonal QR iteration, if desired, computing
                //              left singular vectors in A and computing right singular
                //              vectors in VT
                //              (Workspace: need 5*M-4)
        
                dbdsqr( 'L', m, ncvt, nru, 0, s, &work[ie - 1], vt, 
                        ldvt, a, lda, dum, 1, &work[iwork - 1], info );
            }
      
        }
    
    }
  
    //     Undo scaling if necessary
  
    if( iscl == 1 ) { 
        if( anrm > bignum ) 
            dlascl( 'G', 0, 0, bignum, anrm, minmn, 1, s, minmn, info );
        if( anrm < smlnum ) 
            dlascl( 'G', 0, 0, smlnum, anrm, minmn, 1, s, minmn, info );
    }
  
    //     Return optimal workspace in WORK(1)
  
    work[0] = maxwrk;
  
    rel_chrtmp(_c,NCHRTMPS);
    return;
  
    //     End of DGESVD
  
#undef  NCHRTMPS
#undef  VT
#undef  U
#undef  A
} // end of function 


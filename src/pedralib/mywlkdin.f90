!
!    WLKDIN on the basis of ABACUS abawalk.F
!    written and/or modified by Krzysztof Mozgawa 
!    and Ville Weijo, 2010
!

SUBROUTINE WLKDIN(COR, TMASS, N, ANGMOM, TINERT, OMEGA, CEPVAL, CEPVEC, DOCOPY, PLANAR, LINEAR)

  IMPLICIT NONE
  REAL(8), DIMENSION(N,3), INTENT(IN) :: COR
  REAL(8), DIMENSION(N), INTENT(IN) :: TMASS
  INTEGER, INTENT(IN) :: N
  REAL(8), DIMENSION(3), INTENT(IN) :: ANGMOM
  REAL(8), DIMENSION(3,3), INTENT(OUT) :: TINERT, CEPVEC
  real(8), dimension(3), INTENT(OUT) :: OMEGA, CEPVAL
  LOGICAL, INTENT(OUT) :: PLANAR, LINEAR
  LOGICAL, INTENT(IN) :: DOCOPY
  
  INTEGER :: i, j, k, INFO
  REAL(8) :: EIGVAL(3), EIGVEC(3,3), IEIGVAL(3), WORK(15), TINVER(3,3), EIGVALINV(3,3), TEMP(3,3)
  REAL(8), parameter :: TSTLIN = 1D-10

  TINERT = 0D0 
!km  write(LUPRI,*) 'Zeroing succesfully complete, beginnig calculating'

  do i=1, N
     do j=1,3
        do k=1,3
           TINERT(j,k) = TINERT(j,k)+ TMASS(i)*COR(i,j)*COR(i,k)
        enddo
     enddo
  enddo
  TEMP = TINERT

  call DGEEV('V','N',3,TEMP,3,EIGVAL,IEIGVAL,EIGVEC,3,1,1,WORK,15,INFO)

  IF ( ABS(EIGVAL(3)-EIGVAL(2)-EIGVAL(1)) .LT. TSTLIN) THEN
     PLANAR = .TRUE.
  ELSE
     PLANAR = .FALSE.
  END IF
  IF (EIGVAL(3) .LT. TSTLIN) THEN
     LINEAR = .TRUE.
     PLANAR = .FALSE.
     EIGVAL(3) = 0D0
  ELSE
     LINEAR = .FALSE.
     EIGVAL(3) = 1D0/EIGVAL(3)
  END IF
  EIGVAL(2) = 1D0/EIGVAL(2)
  EIGVAL(1) = 1D0/EIGVAL(1)
  EIGVALINV = 0D0
  EIGVALINV(1,1) = EIGVAL(1)
  EIGVALINV(2,2) = EIGVAL(2)
  EIGVALINV(3,3) = EIGVAL(3)
  
  TINVER= matmul(matmul(EIGVEC, EIGVALINV), transpose(EIGVEC))

  OMEGA = matmul(TINVER,ANGMOM)

  if (DOCOPY) then
     CEPVAL= EIGVAL
     CEPVEC= EIGVEC 
  endif


END SUBROUTINE WLKDIN



      
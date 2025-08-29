module base
  implicit none
  private

  public :: point, frotate
  
  real, parameter :: PI = 3.14159265358979323846

  type :: point
    real :: x
    real :: y
  end type point

contains

  function frotate(points, center, theta) result(nPoint)
    implicit none

    type(point), intent(in) :: points(0:8)
    type(point), intent(in) :: center
    real, intent(in) :: theta

    type(point) :: nPoint(0:8)

    real :: rad_theta
    integer :: i

    ! Convert degrees to radians if theta is in degrees, otherwise, it's a no-op
    rad_theta = theta / 180 * PI

    do i = 0, 8
      ! Translate to origin, rotate, then translate back
      nPoint(i)%x = (points(i)%x - center%x) * cos(rad_theta) - (points(i)%y - center%y) * sin(rad_theta) + center%x
      nPoint(i)%y = (points(i)%x - center%x) * sin(rad_theta) + (points(i)%y - center%y) * cos(rad_theta) + center%y
    end do
  end function frotate
end module base

program hello
  use base
  implicit none

  integer, parameter :: count = 3
  integer, parameter :: tcount = count ** 2 - 1

  real :: list(0:tcount)
  type(point) :: points(0:tcount)
  type(point) :: rotated_points(0:tcount)

  integer :: i

  list = [(i, i = 0, tcount)]

  do i = 0, tcount
    points(i)%x = mod(list(i), real(count))
    points(i)%y = floor(list(i) / count)
    write(*, '(A, F3.1, A, F3.1, A)', advance='no') '(', points(i)%x, ', ', points(i)%y, ') '
  end do
  write(*,*) "" ! Newline

  ! Call the rotation function
  rotated_points = frotate(points, point(1.0, 1.0), 90.0)

  write(*, '(A)') "Rotated points:"
  do i = 0, tcount
    write(*, '(A, F3.1, A, F3.1, A)', advance='no') '(', rotated_points(i)%x, ', ', rotated_points(i)%y, ') '
  end do
  write(*,*) ""
end program hello


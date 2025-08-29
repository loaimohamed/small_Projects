module base
  implicit none
  private

  public :: point, grid_points, frotate, frotate2
  
  real, parameter :: PI = 3.14159265358979323846

  type :: point
    real :: x
    real :: y
  end type point

  type :: grid_points
    type(point) :: center
    real :: theta
    integer :: count
  end type grid_points

contains

  function frotate(points, center, theta, count) result(nPoint)
    implicit none
    integer, intent(in) :: count
    type(point), intent(in) :: points(0:count)
    type(point), intent(in) :: center
    real, intent(in) :: theta

    type(point) :: nPoint(0:count)

    real :: rad_theta
    integer :: i

    ! Convert degrees to radians if theta is in degrees, otherwise, it's a no-op
    rad_theta = theta / 180 * PI

    do i = 0, count
      ! Translate to origin, rotate, then translate back
      nPoint(i)%x = (points(i)%x - center%x) * cos(rad_theta) - (points(i)%y - center%y) * sin(rad_theta) + center%x
      nPoint(i)%y = (points(i)%x - center%x) * sin(rad_theta) + (points(i)%y - center%y) * cos(rad_theta) + center%y
    end do
  end function frotate

function frotate2(points, gridPs) result(nPoint)
    implicit none
    type(grid_points), intent(in) :: gridps
    type(point), intent(in) :: points(0:gridps%count)

    type(point) :: nPoint(0:gridps%count)

    real :: rad_theta
    integer :: i

    ! Convert degrees to radians if theta is in degrees, otherwise, it's a no-op
    rad_theta = gridps%theta / 180 * PI

    do i = 0, gridps%count
      ! Translate to origin, rotate, then translate back
      nPoint(i)%x = (points(i)%x - gridps%center%x) * cos(rad_theta) - (points(i)%y - gridps%center%y) * sin(rad_theta) + gridps%center%x
      nPoint(i)%y = (points(i)%x - gridps%center%x) * sin(rad_theta) + (points(i)%y - gridps%center%y) * cos(rad_theta) + gridps%center%y
    end do
  end function frotate2

end module base

program hello
  use base
  implicit none

  integer, parameter :: count = 3
  integer, parameter :: tcount = count ** 2 - 1

  real :: list(0:tcount)
  type(point) :: points(0:tcount)
  type(point) :: rotated_points(0:tcount)
  type(grid_points) :: gridPs

  integer :: i

  list = [(i, i = 0, tcount)]

  write(*, '(A)') "Original points:"
  do i = 0, tcount
    points(i)%x = mod(list(i), real(count))
    points(i)%y = floor(list(i) / count)
    write(*, '(I0, A, F3.1, A, F3.1, A)', advance='no') i, ': (', points(i)%x, ', ', points(i)%y, ') '
  end do
  write(*,*) "" ! Newline

  ! rotated_points = frotate(points, point(1.0, 1.0), 90.0, tcount)

  ! write(*, '(A)') "Rotated points:"
  ! do i = 0, tcount
  !   write(*, '(I0, A, F3.1, A, F3.1, A)', advance='no') i, ': (', rotated_points(i)%x, ', ', rotated_points(i)%y, ') '
  ! end do
  ! write(*,*) ""

  gridPs%center = point(1.0, 1.0)
  gridPs%theta = 90.0
  gridPs%count = tcount
  rotated_points = frotate2(points, gridPs)

  write(*, '(A)') "Rotated points:"
  do i = 0, tcount
    write(*, '(I0, A, F3.1, A, F3.1, A)', advance='no') i, ': (', rotated_points(i)%x, ', ', rotated_points(i)%y, ') '
  end do
  write(*,*) ""

end program hello


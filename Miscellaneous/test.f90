program test
    implicit none
    real:: n1
    real:: n2
    real:: n3

    n1 = 1.5
    n2 = 2.5
    n3 = n1 + n2

    print *, "Testing: ", n1, n2, n3
    write(*,'(F6.2, F6.2)') n1, n2

end program test

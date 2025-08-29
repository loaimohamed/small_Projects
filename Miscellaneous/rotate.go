package main

import (
	"fmt"
)

type Point struct {
	x float64
	y float64
}

func addPoint(a Point, b Point) Point {
	var c Point = Point{
		x: a.x + b.x,
		y: a.y + b.y,
	}
	return c
}

func main() {
	fmt.Println("Welcome")
	a := Point{x: 0.50, y: 1.50}
	b := Point{x: 1.50, y: 1.50}

	c := addPoint(a, b)

	fmt.Println(c)
}

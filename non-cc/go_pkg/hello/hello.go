package main

import (
	"fmt"
	"go_erlang_projects/go_pkg/stringutil"
	"math/rand"
	"math"
	//"runtime"
)

func add(x int, y int) int {
	return x + y;	
}

// decl
func add2(x, y int) int {
	return x + y;	
}

func swap(x, y string) (string, string) {
	return y, x
}

func pow(x, n, lim float64) float64 {
	if v := math.Pow(x, n); v < lim {  // calc before
		return v
	}
	return lim
}

func main() {
    fmt.Printf(stringutil.Reverse("hello, world"))
    fmt.Printf("\n")

    rand.Seed(42)
    fmt.Println("Number: ", rand.Int())
    fmt.Println("Number: ", rand.Int())

    fmt.Println(math.Pi)  // import with Big letter

    fmt.Println(add(42, 13))

    a, b := swap("hello", "word")
    fmt.Println(a, b)

    var c, python, java = true, false, "no!"
    fmt.Println(c, python, java)

	// error: no new var's left
    //c, python, java := true, false, "no!"

    // Elems 
    // for.go
    sum := 0
    for i := 0; i < 10; i++ {
    	sum += i
    }

    // case
}
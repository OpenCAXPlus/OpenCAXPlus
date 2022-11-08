package isFlagPassed

import (
	"flag"
)

// func isFlagPassed(name string) bool {
// 	found := false
// 	flag.Visit(func(f *flag.Flag) {
// 		if f.Name == name {
// 			found = true
// 		}
// 	})
// 	fmt.Println("Passed ", name, found)
// 	return found
// }

/*
判断是否FlagSet上有指定的选项
*/
func FS(fs flag.FlagSet, name string) bool {
	found := false
	fs.Visit(func(f *flag.Flag) {
		if f.Name == name {
			found = true
		}
	})
	// fmt.Println("PassedFS ", name, found)
	return found
}

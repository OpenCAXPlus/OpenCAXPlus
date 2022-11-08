package plugin

import (
	"fmt"
)

func Info(anything string) {
	fmt.Println("Hello world from pkg/plugin with ", anything)
}

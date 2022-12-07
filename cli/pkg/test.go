package pkg

import (
	"fmt"
	"runtime"
)

func commandTest(build string, compiler string, cmakedir string) string {
	system := runtime.GOOS
	ctest := cmakeCmd(compiler, cmakedir, "ctest")
	if runtime.GOOS == "windows" {
		return fmt.Sprintf(`%s --preset="%s-%s-%s"\r
		`, ctest, system, compiler, build)
	} else {
		return fmt.Sprintf(`%s --preset="%s-%s-%s"
		`, ctest, system, compiler, build)
	}
}

func Test(build string, compiler string, cmakedir string) {
	commands := ""
	if compiler == "intel" {
		commands = commands + prepareIntel()
	}
	commands = commands + commandTest(build, compiler, cmakedir)
	executeCommand(commands)
}

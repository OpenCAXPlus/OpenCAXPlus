package pkg

import (
	"fmt"
	"runtime"
)

func commandTest(build string, compiler string, cmakedir string) string {
	system := runtime.GOOS
	ctest := cmakeCmd(compiler, cmakedir, "ctest")
	cmd := fmt.Sprintf(`%s --preset="%s-%s-%s"`, ctest, system, compiler, build)
	return cmd
}

func Test(build string, compiler string, cmakedir string) {
	commands := ""
	if compiler == "intel" {
		commands = commands + prepareIntel()
	}
	commands = commands + commandTest(build, compiler, cmakedir)
	executeCommand(commands)
}

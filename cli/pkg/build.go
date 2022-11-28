package pkg

import (
	"fmt"
	"os"
	"runtime"
)

func commandBuild(build string, compiler string, target string, cmakedir string) string {
	system := runtime.GOOS
	cmake := cmakeCmd(compiler, cmakedir, "cmake")
	cmd := fmt.Sprintf(`%s --preset="%s-%s-%s" -S "."
%s --build --preset="%s-%s-%s" --target %s
`, cmake, system, compiler, build, cmake, system, compiler, build, target)
	return cmd
}

func Build(build string, compiler string, target string, cmakedir string, path string) {
	commands := ""
	if compiler == "intel" {
		commands = commands + prepareIntel()
	}
	cwd, _ := os.Getwd()
	commands = commands + fmt.Sprintf(`cd %s
	`, path)
	commands = commands + commandBuild(build, compiler, target, cmakedir)
	commands = commands + fmt.Sprintf(`cd %s
	`, cwd)
	executeCommand(commands)
}

package pkg

import (
	"fmt"
	"os"
	"runtime"
)

func commandBuild(build string, compiler string, target string, cmakedir string) string {
	system := runtime.GOOS
	cmake := cmakeCmd(compiler, cmakedir, "cmake")
	if runtime.GOOS == "windows" {
		return fmt.Sprintf("%s --preset=\"%s-%s-%s\" -S \".\"\r\n%s --build --preset=\"%s-%s-%s\" --target %s\r\n", cmake, system, compiler, build, cmake, system, compiler, build, target)
	} else {
		return fmt.Sprintf("%s --preset=\"%s-%s-%s\" -S \".\"\n%s --build --preset=\"%s-%s-%s\" --target %s\n", cmake, system, compiler, build, cmake, system, compiler, build, target)
	}
}

func Build(build string, compiler string, target string, cmakedir string, path string) {
	commands := ""
	if compiler == "intel" {
		commands = commands + prepareIntel()
	}
	cwd, _ := os.Getwd()

	if runtime.GOOS == "windows" {
		commands = commands + fmt.Sprintf("cd %s\r\n", path)
		commands = commands + commandBuild(build, compiler, target, cmakedir)
		commands = commands + fmt.Sprintf("cd %s\r\n", cwd)
	} else {
		commands = commands + fmt.Sprintf("cd %s\n", path)
		commands = commands + commandBuild(build, compiler, target, cmakedir)
		commands = commands + fmt.Sprintf("cd %s\n", cwd)
	}
	executeCommand(commands)
}

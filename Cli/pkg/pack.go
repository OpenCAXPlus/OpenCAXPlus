package pkg

import (
	"fmt"
	"runtime"
)

func commandPack(build string, compiler string, cmakedir string) string {
	system := runtime.GOOS
	cpack := cmakeCmd(compiler, cmakedir, "cmake")
	return fmt.Sprintf("%s --build --preset=\"%s-%s-%s\" --target package"+newline(), cpack, system, compiler, build)
}

func Pack(build string, compiler string, cmakedir string) {
	commands := commandPack(build, compiler, cmakedir)
	executeCommand(commands)
}

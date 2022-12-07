package pkg

import (
	"fmt"
	"runtime"
)

func commandPack(build string, compiler string, cmakedir string) string {
	system := runtime.GOOS
	cpack := cmakeCmd(compiler, cmakedir, "cmake")
	if runtime.GOOS == "windows" {
		return fmt.Sprintf(`%s --build --preset="%s-%s-%s" --target package\r
		`, cpack, system, compiler, build)
	} else {
		return fmt.Sprintf(`%s --build --preset="%s-%s-%s" --target package
		`, cpack, system, compiler, build)
	}
}

func Pack(build string, compiler string, cmakedir string) {
	commands := commandPack(build, compiler, cmakedir)
	executeCommand(commands)
}

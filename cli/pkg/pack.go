package pkg

import (
	"fmt"
	"runtime"
)

func commandPack(build string, compiler string, cmakedir string) string {
	system := runtime.GOOS
	cpack := cmakeCmd(compiler, cmakedir, "cmake")
	// generator := "TGZ"
	cmd := fmt.Sprintf(`%s --build --preset="%s-%s-%s" --target package`, cpack, system, compiler, build)
	// cmd := fmt.Sprintf(`%s -G %s `, cpack, generator)
	return cmd
}

func Pack(build string, compiler string, cmakedir string) {
	commands := commandPack(build, compiler, cmakedir)
	executeCommand(commands)
}

package pkg

import (
	"fmt"
	"os"
)

func commandBuild(preset string, target string) string {
	// system := runtime.GOOS
	return fmt.Sprintf("cmake --preset=\"%s\" -S \".\""+newline()+"cmake --build --preset=\"%s\" --target %s"+newline(), preset, preset, target)
}

func Build(preset string, target string, path string) {
	commands := ""
	cwd, _ := os.Getwd()

	commands = commands + fmt.Sprintf("cd %s"+newline(), path)
	commands = commands + commandBuild(preset, target)
	commands = commands + fmt.Sprintf("cd %s"+newline(), cwd)
	executeCommand(commands)
}

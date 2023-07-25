package pkg

import (
	"fmt"
	"os"
)

func commandConfigure(id string, preset string) string {
	// yml, _ := ParseOCPYaml(path)
	// script, err := InstallConfigurationExists(selectedPackage)
	// if err != nil {
	// 	panic(err)
	// }
	// RunScript(script)
	return fmt.Sprintf("cmake --preset=\"%s\" -S \".\"", preset)
}

func Configure(id string, preset string, path string) {

	commands := ""
	cwd, _ := os.Getwd()

	commands = commands + fmt.Sprintf("cd %s"+newline(), path)
	commands = commands + commandConfigure(id, preset)
	commands = commands + fmt.Sprintf("cd %s"+newline(), cwd)

	executeCommand(commands)
}

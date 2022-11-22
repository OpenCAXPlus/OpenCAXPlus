package app

import (
	"fmt"
	"os/exec"

	log "github.com/sirupsen/logrus"
)

func Build(build string, compiler string, target string, system string, cmakedir string) {
	commands := ""
	if compiler == "intel" {
		commands = commands + prepareIntel(system)
	}
	commands = commands + commandBuild(build, compiler, target, system, cmakedir)
	executeCommand(system, compiler, commands)
}

func commandBuild(build string, compiler string, target string, system string, cmakedir string) string {
	cmake := cmakeCmd(compiler, system, cmakedir)
	cmd := fmt.Sprintf(`%s --preset="%s-%s-%s" -S "."
%s --build --preset="%s-%s-%s" --target %s
`, cmake, system, compiler, build, cmake, system, compiler, build, target)
	cmds, _ := exec.Command("bash", "-c", "ls -la").Output()
	log.Info(string(cmds))

	return cmd
}

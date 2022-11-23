package pkg

func SdkBuild(build string, compiler string, target string, system string, cmakedir string) {
	commands := ""
	if compiler == "intel" {
		commands = commands + prepareIntel(system)
	}
	commands = commands + commandBuild(build, compiler, target, system, cmakedir)
	executeCommand(system, compiler, commands)
}

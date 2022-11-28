package pkg

func Clean(dir string) {
	commands := "rm -rf " + dir
	executeCommand(commands)
}

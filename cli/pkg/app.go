package pkg

import (
	"fmt"
	"path/filepath"
)

func AppBuild(build string, compiler string, target string, system string, cmakedir string) {
	commands := ""
	if compiler == "intel" {
		commands = commands + prepareIntel(system)
	}
	commands = commands + commandBuild(build, compiler, target, system, cmakedir)
	executeCommand(system, compiler, commands)
}

func AppCreate(sdk string, template string, path string) {
	template_path := filepath.Join(sdk, "starter", template)
	workflow_path := filepath.Join(sdk, "starter", ".github")
	workflow_destination := filepath.Join(path, ".github")
	cmakepreset_path := filepath.Join(sdk, "CMakePresets.json")
	if FindFolder(template_path) {
		// copy the template folder to the path location
		CopyDir(template_path, path)
		CopyDir(workflow_path, workflow_destination)
		CopyFile(cmakepreset_path, filepath.Join(path, "CMakePresets.json"))
	} else {
		fmt.Errorf("The template you choose does not exist, check with the app create -l option")
	}
}

package app

import (
	"OpenCAXPlus/cli/pkg"
	"fmt"
	"path/filepath"
)

func Create(sdk string, template string, path string) {
	template_path := filepath.Join(sdk, "starter", template)
	workflow_path := filepath.Join(sdk, "starter", ".github")
	workflow_destination := filepath.Join(path, ".github")
	cmakepreset_path := filepath.Join(sdk, "CMakePresets.json")
	if pkg.FindFolder(template_path) {
		// copy the template folder to the path location
		pkg.CopyDir(template_path, path)
		pkg.CopyDir(workflow_path, workflow_destination)
		pkg.CopyFile(cmakepreset_path, filepath.Join(path, "CMakePresets.json"))
	} else {
		fmt.Errorf("The template you choose does not exist, check with the app create -l option")
	}
}

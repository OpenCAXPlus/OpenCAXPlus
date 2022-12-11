package pkg

import (
	"fmt"
	"path/filepath"
)

func CreateApp(sdk string, template string, path string) {
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

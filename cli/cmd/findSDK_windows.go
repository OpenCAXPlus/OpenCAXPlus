package cmd

import (
	"OpenCAXPlus/cli/pkg"
	"os"
	"path/filepath"
)

func findSDK() string {
	path := ""
	cwd, _ := os.Getwd()
	paths := [5]string{
		"C:/Program Files/opencaxplus/sdk",
		filepath.Join(os.Getenv("HOME"), "AppData", "Local", "Programs", "opencaxplus", "sdk"),
		filepath.Join(os.Getenv("HOME"), "opencaxplus", "sdk"),
		filepath.Join(cwd, "..", "..", "OpenCAXPlusSDK"),
	}
	for i := 0; i < len(paths); i++ {
		if pkg.FindFolder(paths[i]) {
			path = paths[i]
		}
	}
	return path
}

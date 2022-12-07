package cmd

import (
	"OpenCAXPlus/cli/pkg"
	"os"
	"path/filepath"
	"runtime"
)

func findSDK() string {
	path := ""
	cwd, _ := os.Getwd()
	paths := [4]string{}
	if runtime.GOOS == "windows" {
		paths = [4]string{
			"C:/Program Files/opencaxplus/sdk",
			filepath.Join(os.Getenv("HOME"), "AppData", "Local", "Programs", "opencaxplus", "sdk"),
			filepath.Join(os.Getenv("HOME"), "opencaxplus", "sdk"),
			filepath.Join(cwd, "..", "..", "OpenCAXPlusSDK"),
		}
	} else {
		paths = [4]string{
			"/opt/opencaxplus/sdk",
			"/usr/local/opencaxplus/sdk",
			filepath.Join(os.Getenv("HOME"), "opencaxplus", "sdk"),
			filepath.Join(cwd, "..", "..", "OpenCAXPlusSDK"),
		}
	}
	for i := 0; i < len(paths); i++ {
		if pkg.FindFolder(paths[i]) {
			path = paths[i]
		}
	}
	return path
}

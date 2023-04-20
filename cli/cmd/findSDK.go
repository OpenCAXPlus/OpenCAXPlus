package cmd

import (
	"OpenCAXPlusCli/pkg"
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
			"C:/Program Files/OCP",
			filepath.Join(os.Getenv("HOME"), "AppData", "Local", "Programs", "OCP"),
			filepath.Join(os.Getenv("HOME"), "OCP"),
			filepath.Join(cwd, ".."),
		}
	} else {
		paths = [4]string{
			"/opt/OCP/",
			"/usr/local/OCP/",
			filepath.Join(os.Getenv("HOME"), "OCP"),
			filepath.Join(cwd, ".."),
		}
	}
	for i := 0; i < len(paths); i++ {
		if pkg.FindFolder(paths[i]) {
			path = paths[i]
		}
	}
	return path
}

package pkg

import (
	"fmt"
	"os"
	"path/filepath"
	"runtime"

	log "github.com/sirupsen/logrus"
)

// ! TODO need to run the Download recursively on config dependencies
func Download(selectedPackage InstallPackage) (InstallPackage, error) {
	t := selectedPackage.Type
	id := selectedPackage.ID
	uid := selectedPackage.UID
	version := selectedPackage.Version

	// Read the packages.yml file
	homeDir, _ := os.UserHomeDir()
	downloadPath := filepath.Join(homeDir, "ocp", "download")

	downloadUrl := "https://ocp-" + t + ".oss-cn-hongkong.aliyuncs.com" + "/" + uid + "/" + uid + "-" + version + ".tar.xz"
	downloadFile := filepath.Join(downloadPath, id+"-"+version+".tar.xz")

	err := DownloadFile(downloadUrl, downloadFile)
	if err != nil {
		panic(err)
	}
	log.Println("Successfully downloaded ", downloadUrl, " to ", downloadFile)

	// xzFile := filepath.Join(downloadPath, id+"-"+version+".tar.xz")
	DecompressTarXZ(downloadFile, downloadPath)

	destPath := filepath.Join(homeDir, "ocp", t, uid, version)
	srcPath := filepath.Join(downloadPath, "ocp", t, uid, version)
	CopyDir(srcPath, destPath)
	log.Println("Successfully decompressed ", downloadFile, " to ", destPath)

	//! TODO go to the source folder and find dependencies
	// need to
	depPackages := GetConfigurationDependencies(selectedPackage)
	for _, dep := range depPackages {
		Download(dep)
	}

	return selectedPackage, err
}

func Install(selectedPackage InstallPackage) {
	depPackages := GetConfigurationDependencies(selectedPackage)
	for _, dep := range depPackages {
		log.Debug("dependency", dep)
		Install(dep)
	}
	// log.Debug("depPackage", depPackages)
	// log.Debug("Selected", selectedPackage)
	script, err := InstallConfigurationExists(selectedPackage)
	if err != nil {
		panic(err)
	}
	RunScript(script)
}

func RunScript(script Script) {

	ext := ""
	call := ""
	switch runtime.GOOS {
	case "linux":
		ext = "sh"
		call = "source"
	case "windows":
		ext = "bat"
		call = "call"
	}
	command := fmt.Sprintf("%v %v.%v", call, script.Run, ext)
	for _, arg := range script.Args {
		command = fmt.Sprintf("%v %v", command, arg)
	}

	cwdPath, err := os.Getwd()
	if err != nil {
		panic(err)
	}

	err = os.Chdir(script.Path)
	if err != nil {
		panic(err)
	}

	executeCommand(command)

	err = os.Chdir(cwdPath)
	if err != nil {
		panic(err)
	}
}

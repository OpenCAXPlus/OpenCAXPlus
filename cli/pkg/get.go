package pkg

import (
	"fmt"
	"os"
	"path/filepath"
	"runtime"

	log "github.com/sirupsen/logrus"
)

func DownloadSDK(selectedPackage InstallPackage) (InstallPackage, error) {
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

	DecompressTarXZ(downloadFile, downloadPath)

	realVersion := version
	if version == "latest" {
		sdkPath := filepath.Join(downloadPath, "ocp", uid)
		realVersion, _, err = MostRecentSubFolder(sdkPath)
	}

	destPath := filepath.Join(homeDir, "ocp", uid, realVersion)
	srcPath := filepath.Join(downloadPath, "ocp", uid, realVersion)
	CopyDir(srcPath, destPath)
	latestDir := filepath.Join(homeDir, "ocp", "sdk", "latest")
	realDir := filepath.Join(homeDir, "ocp", "sdk", realVersion)
	executeCommand("rm -f " + latestDir)
	executeCommand("ln -sf " + realDir + " " + latestDir)

	log.Println("Successfully decompressed ", downloadFile, " to ", destPath)

	return selectedPackage, err
} // ! TODO need to run the Download recursively on config dependencies
func Download(selectedPackage InstallPackage) (InstallPackage, error) {
	t := selectedPackage.Type
	id := selectedPackage.ID
	uid := selectedPackage.UID
	version := selectedPackage.Version

	// Read the packages.yml file
	homeDir, err := os.UserHomeDir()
	downloadPath := filepath.Join(homeDir, "ocp", "download")

	downloadUrl := "https://ocp-" + t + ".oss-cn-hongkong.aliyuncs.com" + "/" + uid + "/" + uid + "-" + version + ".tar.xz"
	downloadFile := filepath.Join(downloadPath, id+"-"+version+".tar.xz")

	err = DownloadFile(downloadUrl, downloadFile)
	if err != nil {
		panic(err)
	}
	log.Println("Successfully downloaded ", downloadUrl, " to ", downloadFile)

	// xzFile := filepath.Join(downloadPath, id+"-"+version+".tar.xz")
	err = DecompressTarXZ(downloadFile, downloadPath)
	if err != nil {
		panic(err)
	}

	destPath := filepath.Join(homeDir, "ocp", t, uid, version)
	srcPath := filepath.Join(downloadPath, "ocp", t, uid, version)
	CopyDir(srcPath, destPath)
	log.Println("Successfully decompressed ", downloadFile, " to ", destPath)

	//! TODO go to the source folder and find dependencies
	// need to
	if selectedPackage.Configuration != "" {
		ocpPath := GetPackageSourcePath(selectedPackage)
		depPackages := GetConfigurationDependencies(ocpPath, selectedPackage)
		for _, dep := range depPackages {
			Download(dep)
		}
	}

	return selectedPackage, err
}

func Install(selectedPackage InstallPackage) {
	if selectedPackage.Configuration != "" {
		ocpPath := GetPackageSourcePath(selectedPackage)
		depPackages := GetConfigurationDependencies(ocpPath, selectedPackage)
		for _, dep := range depPackages {
			log.Debug("Dependency", dep)
			Install(dep)
		}
		// log.Debug("depPackage", depPackages)
		// log.Debug("Selected", selectedPackage)
		script, err := InstallConfigurationExists(selectedPackage)
		if err != nil {
			panic(err)
		}

		ocpCompiler := os.Getenv("OCP_COMPILER")

		installDir:=filepath.Join(ocpPath, "install", ocpCompiler, selectedPackage.Configuration)

		if FolderExists(installDir) {
			log.Debug("Configuration already installed ", installDir)
		} else {
			RunScript(script)
		}
	}
}

func ScriptToString(script Script) string {
	ext := ""
	call := ""
	cd := ""
	switch runtime.GOOS {
	case "linux":
		ext = "sh"
		call = "source"
		cd = "cd"
	case "windows":
		ext = "bat"
		call = "call"
		cd = "chdir"
	}
	command := fmt.Sprintf("%v %v\n", cd, script.Path)
	command = command + fmt.Sprintf("%v %v.%v", call, script.Run, ext)
	for _, arg := range script.Args {
		command = fmt.Sprintf("%v %v", command, arg)
	}

	cwdPath, err := os.Getwd()
	if err != nil {
		panic(err)
	}
	command = command + fmt.Sprintf("\n%v %v", cd, cwdPath)

	return command

}

func RunScript(script Script) {

	command := ScriptToString(script)
	executeCommand(command)

}

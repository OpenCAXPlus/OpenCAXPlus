package pkg

import (
	"log"
	"os"
	"path/filepath"
)

// ! TODO need to run the Download recursively on config dependencies
func Download(selectedPackage Package, config string) (Package, error) {
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

	return selectedPackage, err
}

func Install(selectedPackage Package, config string) {
	homeDir, _ := os.UserHomeDir()
	t := selectedPackage.Type
	uid := selectedPackage.UID
	version := selectedPackage.Version
	srcPath := filepath.Join(homeDir, "ocp", t, uid, version, "source")
	ocpPath := filepath.Join(homeDir, "ocp", t, uid, version)
	cmdFromOCP, _ := ConfigCommand(ocpPath, version, config)

	cwdPath, err := os.Getwd()
	if err != nil {
		panic(err)
	}

	err = os.Chdir(srcPath)
	if err != nil {
		panic(err)
	}

	executeCommand(cmdFromOCP)

	err = os.Chdir(cwdPath)
	if err != nil {
		panic(err)
	}
}

package pkg

import (
	"log"
	"os"
	"path/filepath"
)

type DownloadPackage struct {
	ID       string   `yaml:"id"`  // id is for the download command
	UID      string   `yaml:"uid"` // uid is for the download url
	Versions []string `yaml:"versions"`
}

type DownloadPackages struct {
	ID    string
	Items []DownloadPackage
}

// type Packages struct {
// 	Packages []DownloadPackages
// }

// type yamlStruct struct {
// 	External []DownloadPackage `yaml:"external"`
// 	Toolkit  []DownloadPackage `yaml:"toolkit"`
// 	Starter  []DownloadPackage `yaml:"starter"`
// }

// need to get the ocp.toml from base url then download the external and dependencies
func Download(id string, version string, force bool) {
	// Read the packages.yml file
	homeDir, _ := os.UserHomeDir()
	downloadPath := filepath.Join(homeDir, "ocp", "download")

	selectedPackage, err := PackageExist(id, version)
	if err != nil {
		panic(err)
	}

	t := selectedPackage.Type
	uid := selectedPackage.UID
	// check if the package exists locally, if no go on, if yes check if force is true
	if FolderExists(filepath.Join(homeDir, "ocp", t, uid, version)) && !force {
		log.Printf("You have %+v@%+v installed locally, force install %+v use -f to force reinstall.\n", id, version, force)
	} else {
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
	}

}

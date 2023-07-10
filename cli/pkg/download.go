package pkg

import (
	"io/ioutil"
	"log"
	"os"
	"path/filepath"

	"gopkg.in/yaml.v2"
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

type Packages struct {
	Packages []DownloadPackages
}

type yamlStruct struct {
	External []DownloadPackage `yaml:"external"`
	Toolkit  []DownloadPackage `yaml:"toolkit"`
	Starter  []DownloadPackage `yaml:"starter"`
}

// need to get the ocp.toml from base url then download the external and dependencies
func Download(id string, version string, force bool) {
	// Read the packages.yml file
	homeDir, _ := os.UserHomeDir()
	downloadPath := filepath.Join(homeDir, "ocp", "download")
	//! TODO: should automatically choose the packages.yml file in home/ocp or dev folder
	//! based on the build type. Release build will read from home, while debug build from dev folder
	data, err := ioutil.ReadFile(filepath.Join(homeDir, "ocp", "packages.yml"))
	if err != nil {
		log.Fatal(err)
	}

	var yml yamlStruct
	err = yaml.Unmarshal(data, &yml)
	if err != nil {
		panic(err)
	}

	// packages contains all three types of the packages that we can download
	var packages Packages
	packages.Packages = append(packages.Packages, DownloadPackages{
		ID:    "external",
		Items: yml.External,
	}, DownloadPackages{
		ID:    "toolkit",
		Items: yml.Toolkit,
	}, DownloadPackages{
		ID:    "starter",
		Items: yml.Starter,
	})

	// find the item with the provided ID
	foundID := false
	foundVersion := false
OuterLoop:
	for _, combinedPackages := range packages.Packages { // loop through the external, toolkit, starter arrays
		for _, universalPackage := range combinedPackages.Items { // loop through the packages
			if universalPackage.ID == id {
				foundID = true
				uid := universalPackage.UID
				log.Printf("Found package: %+v\n", universalPackage)
				// check if version exists, if yes, go download
				for _, v := range universalPackage.Versions {
					if v == version {
						foundVersion = true
						// check if the package exists locally, if no go on, if yes check if force is true
						if FolderExists(filepath.Join(homeDir, "ocp", combinedPackages.ID, uid, version)) && !force {
							log.Printf("You have %+v@%+v installed locally, force install %+v use -f to force reinstall.\n", id, version, force)
						} else {
							downloadUrl := "https://ocp-" + combinedPackages.ID + ".oss-cn-hongkong.aliyuncs.com" + "/" + uid + "/" + uid + "-" + version + ".tar.xz"
							downloadFile := filepath.Join(downloadPath, id+"-"+version+".tar.xz")

							err := DownloadFile(downloadUrl, downloadFile)
							if err != nil {
								panic(err)
							}
							log.Println("Successfully downloaded ", downloadUrl, " to ", downloadFile)

							// xzFile := filepath.Join(downloadPath, id+"-"+version+".tar.xz")
							DecompressTarXZ(downloadFile, downloadPath)

							destPath := filepath.Join(homeDir, "ocp", combinedPackages.ID, uid, version)
							srcPath := filepath.Join(downloadPath, "ocp", combinedPackages.ID, uid, version)
							CopyDir(srcPath, destPath)
							log.Println("Successfully decompressed ", downloadFile, " to ", destPath)
						}
						break OuterLoop
					}
				}
			}
		}
	}

	// if the id does not exist, panic
	if !foundID {
		log.Panicln(id, " not found in package.yml, you may need to update ocp.")
	} else {
		if !foundVersion {
			log.Panicln(id, " found, but "+version+" not found in package.yml, you may need to update ocp.")
		}
	}

}

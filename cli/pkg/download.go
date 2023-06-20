package pkg

import (
	"io/ioutil"
	"log"
	"os"
	"path/filepath"

	"gopkg.in/yaml.v2"
)

type DownloadPackage struct {
	ID       string   `yaml:"id"`
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
	data, err := ioutil.ReadFile(filepath.Join(homeDir, "ocp", "packages.yml"))
	if err != nil {
		log.Fatal(err)
	}

	var yml yamlStruct
	err = yaml.Unmarshal(data, &yml)
	if err != nil {
		panic(err)
	}

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
	for _, combinedPackages := range packages.Packages {
		for _, universalPackage := range combinedPackages.Items {
			if universalPackage.ID == id {
				foundID = true
				log.Printf("Found package: %+v\n", universalPackage)
				// check if version exists, if yes, go download
				for _, v := range universalPackage.Versions {
					if v == version {
						foundVersion = true
						// check if the package exists locally, if no go on, if yes check if force is true
						if FolderExists(filepath.Join(homeDir, "ocp", combinedPackages.ID, id, version)) && !force {
							log.Printf("You have %+v@%+v installed locally, force install %+v use -f to force reinstall.\n", id, version, force)
						} else {
							downloadUrl := "https://ocp-" + combinedPackages.ID + ".oss-cn-hongkong.aliyuncs.com" + "/" + id + "/" + id + "-" + version + ".tar.xz"
							downloadFile := filepath.Join(downloadPath, id+"-"+version+".tar.xz")

							err := DownloadFile(downloadUrl, downloadFile)
							if err != nil {
								panic(err)
							}
							log.Println("Successfully downloaded ", downloadUrl, " to ", downloadFile)
							xzFile := filepath.Join(downloadPath, id+"-"+version+".tar.xz")
							DecompressTarXZ(xzFile, downloadPath)

							destPath := filepath.Join(homeDir, "ocp", combinedPackages.ID, id, version)
							srcPath := filepath.Join(downloadPath, "ocp", combinedPackages.ID, id, version)
							CopyDir(srcPath, destPath)
							log.Println("Successfully decompressed ", xzFile, " to ", destPath)
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

package pkg

import (
	"errors"
	"io/ioutil"
	"log"
	"os"
	"path/filepath"

	"gopkg.in/yaml.v2"
)

// map to the actual yml file
type YamlPackage struct {
	ID       string   `yaml:"id"`  // id is for the download command
	UID      string   `yaml:"uid"` // uid is for the download url
	Type     string   `yaml:"type"`
	Versions []string `yaml:"versions"`
}

func PackageExist(id string, version string) (YamlPackage, error) {
	// Read the packages.yml file
	homeDir, _ := os.UserHomeDir()
	//! TODO: should automatically choose the packages.yml file in home/ocp or dev folder
	//! based on the build type. Release build will read from home, while debug build from dev folder
	data, err := ioutil.ReadFile(filepath.Join(homeDir, "ocp", "packages.yml"))
	if err != nil {
		log.Fatal(err)
	}

	var yml []YamlPackage
	err = yaml.Unmarshal(data, &yml)
	if err != nil {
		panic(err)
	}

	// find the item with the provided ID
	foundID := false
	foundVersion := false
	for _, universalPackage := range yml { // loop through the packages
		if universalPackage.ID == id {
			foundID = true
			log.Printf("Found package: %+v\n", id)
			// check if version exists, if yes, go download
			for _, v := range universalPackage.Versions {
				if v == version {
					foundVersion = true
					log.Printf("Found version: %+v\n", version)
					return universalPackage, nil
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

	return YamlPackage{}, errors.New("Package not found") // return nil when nothing found
}

package pkg

import (
	"errors"
	"fmt"
	"io/ioutil"
	"log"
	"path/filepath"
	"runtime"

	"gopkg.in/yaml.v2"
)

type YamlOCPDefault struct {
	Version       string
	Configuration string
	Scripts       []string
	Dependencies  []string
}

type YamlOCPVersion struct {
	ID             string
	Default        string
	Scripts        []string
	Configurations []string
}

type YamlOCPScript struct {
	ID   string
	Run  string
	Args []string
}

type YamlOCPDependency struct {
	ID       string
	UID      string
	Type     string
	Versions []YamlOCPVersion
}

type YamlOCPConfiguration struct {
	ID           string
	Run          string
	dependencies []string
}

type YamlOCP struct {
	Name           string
	UID            string
	Type           string
	Licenses       []string
	Default        YamlOCPDefault
	Versions       []YamlOCPVersion
	Scripts        []YamlOCPScript
	Dependencies   []YamlOCPDependency
	Configurations []YamlOCPConfiguration
}

// ! TODO need to install recursively
func ConfigCommand(ocpPath string, v string, c string) (string, error) {
	// Read the ocp.yml file
	data, err := ioutil.ReadFile(filepath.Join(ocpPath, "ocp.yml"))
	if err != nil {
		log.Fatal(err)
	}

	var yml YamlOCP
	err = yaml.Unmarshal(data, &yml)
	if err != nil {
		panic(err)
	}

	foundVersion := false
	foundConfig := false
	foundCF := false
	// First, check if the config is supported for the package's version
	for _, version := range yml.Versions {
		if version.ID == v {
			foundVersion = true
			for _, config := range version.Configurations {
				if config == c {
					foundConfig = true
					log.Print("Find the configuration ", c, " of ", v)
					for _, cf := range yml.Configurations {
						if config == cf.ID {
							foundCF = true
							cmd := ""
							ext := ""
							call := ""
							switch runtime.GOOS {
							case "Linux":
								ext = "sh"
								call = "source"
							case "Windows":
								ext = "bat"
								call = "call"
							}
							cmd = fmt.Sprintf("%v %v.%v", call, cf.Run, ext)
							return cmd, nil
						}
					}
				}
			}
		}
	}

	if !foundVersion {
		log.Fatal("The version does not exist in ocp.yml file", v)
	} else {
		if !foundConfig {
			log.Fatal("The configuration is not found for version", v)
		} else {
			if !foundCF {
				log.Fatal("The configuration is not found in ocp.yml file", c)
			}
		}
	}
	return "", errors.New("Configuration not found")

}

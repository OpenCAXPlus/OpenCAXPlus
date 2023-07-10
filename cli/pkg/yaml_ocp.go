package pkg

import (
	"errors"
	"io/ioutil"
	"os"
	"path/filepath"

	log "github.com/sirupsen/logrus"

	"gopkg.in/yaml.v2"
)

type YamlOCPDefault struct {
	Version       string   `yaml:"version"`
	Configuration string   `yaml:"configuration"`
	Scripts       []string `yaml:"scripts"`
	Dependencies  []string `yaml:"dependencies"`
}

type YamlOCPVersion struct {
	ID             string   `yaml:"id"`
	Default        string   `yaml:"default"`
	Scripts        []string `yaml:"scripts"`
	Configurations []string `yaml:"configurations"`
}

type YamlOCPScript struct {
	ID   string   `yaml:"id"`
	Run  string   `yaml:"run"`
	Args []string `yaml:"arguments"`
}

type Script struct {
	Path string
	Run  string
	Args []string
}

type YamlOCPDependency struct {
	ID       string           `yaml:"id"`
	UID      string           `yaml:"uid"`
	Type     string           `yaml:"type"`
	Versions []YamlOCPVersion `yaml:"versions"`
}

type YamlOCPConfiguration struct {
	ID           string   `yaml:"id"`
	Run          string   `yaml:"run"`
	Args         []string `yaml:"arguments"`
	dependencies []string `yaml:"dependencies"`
}

type YamlOCP struct {
	Name           string                 `yaml:"name"`
	UID            string                 `yaml:"uid"`
	Type           string                 `yaml:"type"`
	Licenses       []string               `yaml:"licenses"`
	Default        YamlOCPDefault         `yaml:"default"`
	Versions       []YamlOCPVersion       `yaml:"versions"`
	Scripts        []YamlOCPScript        `yaml:"scripts"`
	Dependencies   []YamlOCPDependency    `yaml:"dependencies"`
	Configurations []YamlOCPConfiguration `yaml:"configurations"`
}

func parseArguments(args []string, p InstallPackage) []string {
	out := args
	for i, arg := range args {

		switch arg {
		case "@id":
			out[i] = p.ID
		case "@uid":
			out[i] = p.UID
		case "@type":
			out[i] = p.Type
		case "@version":
			out[i] = p.Version
		case "@configuration":
			out[i] = p.Configuration
		default:
			out[i] = args[i]
		}
	}
	return out
}

// ! TODO need to install recursively
func InstallConfigurationExists(selectedPackage InstallPackage) (Script, error) {
	homeDir, _ := os.UserHomeDir()
	t := selectedPackage.Type
	uid := selectedPackage.UID
	v := selectedPackage.Version
	c := selectedPackage.Configuration
	// srcPath := filepath.Join(homeDir, "ocp", t, uid, v, "source")
	ocpPath := filepath.Join(homeDir, "ocp", t, uid, v)

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
			log.Debug("Find version in ocp.yml", v)
			if len(version.Configurations) == 0 {
				version.Configurations = append(version.Configurations, yml.Default.Configuration)
			}
			for _, config := range version.Configurations {
				if config == c {
					foundConfig = true
					log.Debug("Find configuration in ocp.yml", c)
					// then, find the configuration details in the ocp.yml/configurations list
					for _, cf := range yml.Configurations {
						if config == cf.ID {
							log.Debug("Find configuration details")
							foundCF = true
							// prepare the arguments array
							parsedArgs := parseArguments(cf.Args, selectedPackage)
							return Script{Path: ocpPath, Run: cf.Run, Args: parsedArgs}, nil
						}
					}
				}
			}
		}
	}

	if !foundVersion {
		log.Fatal("The version does not exist in ocp.yml file ", v)
	} else {
		if !foundConfig {
			log.Fatal("The configuration ", c, " is not found for version ", v)
		} else {
			if !foundCF {
				log.Fatal("The configuration is not found in ocp.yml file ", c)
			}
		}
	}
	return Script{}, errors.New("configuration not found")

}

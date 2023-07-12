package pkg

import (
	"errors"
	"io/ioutil"
	"os"
	"path/filepath"
	"strings"

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
	Default  YamlOCPDefault   `yaml:"default"`
	Versions []YamlOCPVersion `yaml:"versions"`
}

type YamlOCPConfiguration struct {
	ID           string   `yaml:"id"`
	Run          string   `yaml:"run"`
	Args         []string `yaml:"arguments"`
	Dependencies []string `yaml:"dependencies"`
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

func parseArguments(args []string, p InstallPackage, dependencies []InstallPackage) []string {
	out := args
	base := p
	for i, arg := range args {

		s := strings.Split(arg, "@")

		if len(s) > 1 {
			if s[0] == "" {
				base = p
			} else {
				for _, dep := range dependencies {
					if dep.ID == s[0] {
						base = dep
						break
					}
				}
			}
			switch s[1] {
			case "id":
				out[i] = base.ID
			case "uid":
				out[i] = base.UID
			case "type":
				out[i] = base.Type
			case "version":
				out[i] = base.Version
			case "configuration":
				out[i] = base.Configuration
			default:
				out[i] = s[0]
			}
		} else {
			out[i] = s[0]
		}
	}
	return out
}

func GetConfigurationDependencies(selectedPackage InstallPackage) []InstallPackage {
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

	var out []InstallPackage
	// the load the dependencies of configurations
	// log.Debug("yml config", yml.Configurationss)
	for _, config := range yml.Configurations {
		if config.ID == c {
			for _, dep := range config.Dependencies {
				s := strings.Split(dep, "@")
				dep_id := s[0]
				dep_version := ""
				dep_config := ""

				dep_uid := dep_id
				dep_type := "external"
				for _, d := range yml.Dependencies {
					if d.ID == dep_id {
						dep_uid = d.UID
						dep_type = d.Type
						dep_version = d.Default.Version
						dep_config = d.Default.Configuration
					}
				}
				if len(s) == 2 {
					dep_version = s[1]
				}
				if len(s) == 3 {
					dep_config = s[2]
				}
				out = append(out, InstallPackage{
					ID:            dep_id,
					UID:           dep_uid,
					Type:          dep_type,
					Version:       dep_version,
					Configuration: dep_config,
				})
			}
			break
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
							dependencies := GetConfigurationDependencies(selectedPackage)
							parsedArgs := parseArguments(cf.Args, selectedPackage, dependencies)
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

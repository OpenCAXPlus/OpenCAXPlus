/*
Copyright © 2023 Xiaoxing Cheng <hello@chengxiaoxing.me>
*/
package cmd

import (
	"OpenCAXPlusCli/pkg"
	"os"
	"path/filepath"
	"runtime"
	"strings"

	log "github.com/sirupsen/logrus"
	"github.com/spf13/cobra"
	"github.com/spf13/viper"
)

// the download command will
// 1. download the tar.xz file based on the uid and version you specified
// 2. extract the context of the compress file into your local directory
var getCmd = &cobra.Command{
	Use:   "get",
	Short: "ocp get package@version",
	Long: `
	The command will download and install the OpenCAX+ external packages, toolkits, starters, 
	applications, and its dependencies. If the item already exists 
	locally and you want to force re-download everything, you need to pass the option -f.
	parse the ocp.yml configuration file and download the necessary compressed files.`,
	Args: cobra.ExactArgs(1),
	Run: func(cmd *cobra.Command, args []string) {
		s := strings.Split(args[0], "@")
		id := s[0]
		version := s[1]
		config := s[2]

		force := viper.GetBool("get.force")
		download := viper.GetBool("get.download")
		install := viper.GetBool("get.install")

		log.Debug("get command")
		log.Debug("Package id is ", id)
		log.Debug("Package version is ", version)
		log.Debug("Package configuration is ", config)
		log.Debug("System is ", runtime.GOOS)
		selectedSourcePackage, err := pkg.SourcePackageExist(id, version)
		if err != nil {
			panic(err)
		}
		selectedPackage := pkg.InstallPackage{
			ID:            selectedSourcePackage.ID,
			UID:           selectedSourcePackage.UID,
			Type:          selectedSourcePackage.Type,
			Version:       selectedSourcePackage.Version,
			Configuration: config,
		}

		homeDir, _ := os.UserHomeDir()

		if download {
			// check if the package exists locally, if no go on, if yes check if force is true
			if pkg.FolderExists(filepath.Join(homeDir, "ocp", selectedSourcePackage.Type, selectedSourcePackage.UID, version)) && !force {
				log.Printf("You have %+v@%+v sources locally, force re-download %+v use -f to force reinstall.\n", id, version, force)
			} else {
				pkg.Download(selectedPackage)
			}
		}

		script, err := pkg.InstallConfigurationExists(selectedPackage)
		if err != nil {
			panic(err)
		}

		if install {
			pkg.RunScript(script)
		}
	},
}

func init() {
	rootCmd.AddCommand(getCmd)

	// force download
	getCmd.Flags().BoolP("force", "f", false, "force the package to be downloaded.")
	viper.BindPFlag("get.force", getCmd.Flags().Lookup("force"))
	getCmd.Flags().Lookup("force").NoOptDefVal = "true"
	viper.SetDefault("get.force", false)

	// download
	getCmd.Flags().BoolP("download", "d", true, "turn on download step for the package.")
	viper.BindPFlag("get.download", getCmd.Flags().Lookup("download"))
	getCmd.Flags().Lookup("download").NoOptDefVal = "true"
	viper.SetDefault("get.download", true)

	// install
	getCmd.Flags().BoolP("install", "i", true, "turn on install step for the package.")
	viper.BindPFlag("get.install", getCmd.Flags().Lookup("install"))
	getCmd.Flags().Lookup("install").NoOptDefVal = "true"
	viper.SetDefault("get.install", true)
}

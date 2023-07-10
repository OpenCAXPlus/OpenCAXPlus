/*
Copyright © 2023 Xiaoxing Cheng <hello@chengxiaoxing.me>
*/
package cmd

import (
	"OpenCAXPlusCli/pkg"
	"runtime"
	"strings"

	log "github.com/sirupsen/logrus"
	"github.com/spf13/cobra"
	"github.com/spf13/viper"
)

// the download command will
// 1. download the tar.xz file based on the uid and version you specified
// 2. extract the context of the compress file into your local directory
var downloadCmd = &cobra.Command{
	Use:   "download",
	Short: "ocp download package@version",
	Long: `
	The command will download the OpenCAX+ external packages, toolkits, starters, 
	applications, and its dependencies. If the item already exists 
	locally and you want to force re-download everything, you need to pass the option -f.
	parse the ocp.yml configuration file and download the necessary compressed files`,
	Args: cobra.ExactArgs(1),
	Run: func(cmd *cobra.Command, args []string) {
		s := strings.Split(args[0], "@")
		id := s[0]
		version := s[1]

		force := viper.GetBool("download.force")

		log.Debug("Download command")
		log.Debug("Package id is ", id)
		log.Debug("Package version is ", version)
		log.Debug("System is ", runtime.GOOS)
		pkg.Download(id, version, force)
	},
}

func init() {
	rootCmd.AddCommand(downloadCmd)

	// force download
	downloadCmd.Flags().BoolP("force", "f", false, "force the package to be downloaded.")
	viper.BindPFlag("download.force", downloadCmd.Flags().Lookup("force"))
	downloadCmd.Flags().Lookup("force").NoOptDefVal = "true"
	viper.SetDefault("download.force", false)
}

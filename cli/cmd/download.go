/*
Copyright © 2022 NAME HERE <EMAIL ADDRESS>

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

// buildCmd represents the build command
//  need to parse the local ocp.toml file to get the necessary download information
var downloadCmd = &cobra.Command{
	Use:   "download",
	Short: "Download OpenCAX+ toolkits, starters, applications, and etc.",
	Long: `
	The command will download the package and its dependencies. If the item already exists 
	locally and you want to force re-download everything, you need to pass the option -f.
	parse the ocp.yml configuration file 
	and download the necessary compressed files`,
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

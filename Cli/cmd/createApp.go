/*
Copyright © 2022 NAME HERE <EMAIL ADDRESS>

*/
package cmd

import (
	"OpenCAXPlus/Cli/pkg"
	"os"
	"path/filepath"

	log "github.com/sirupsen/logrus"
	"github.com/spf13/cobra"
)

// appCmd represents the app command
var appCmd = &cobra.Command{
	Use:   "app",
	Short: "Create a new OpenCAXPlus application",
	Long: `Create a new OpenCAXPlus application from the starter template in the OpenCAXPlusSDK.
You can create a new application using a specific starter template. For example:
	ocp app create -t CAE_Static_Console
If no template argument is passed, the create command will use the default Custom_Console template.
To get all available starter templates, you can use the command:
	ocp app create -l`,
	Run: func(cmd *cobra.Command, args []string) {
		template, _ := cmd.Flags().GetString("template")
		path, _ := cmd.Flags().GetString("path")
		cwd, _ := os.Getwd()
		if path == "" {
			path = filepath.Join(cwd, template)
		}
		sdk := findSDK()
		log.Debug("create called", sdk, template, path)
		pkg.CreateApp(sdk, template, path)
	},
}

func init() {
	createCmd.AddCommand(appCmd)

	// Here you will define your flags and configuration settings.

	// Cobra supports Persistent Flags which will work for this command
	// and all subcommands, e.g.:
	// appCmd.PersistentFlags().String("foo", "", "A help for foo")

	// Cobra supports local flags which will only run when this command
	// is called directly, e.g.:
	// appCmd.Flags().BoolP("toggle", "t", false, "Help message for toggle")
	appCmd.Flags().BoolP("list", "l", false, "List all available starter templates")
	appCmd.Flags().StringP("template", "t", "Custom_Console", "Choose the starter template you want to use")
	appCmd.Flags().StringP("path", "p", "", "Set the path for your OpenCAXPlus app. If it is empty then the template name will be used as folder name.")
}

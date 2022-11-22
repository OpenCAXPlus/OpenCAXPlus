/*
Copyright © 2022 NAME HERE <EMAIL ADDRESS>

*/
package cmd

import (
	"OpenCAXPlus/cli/pkg/app"
	"runtime"

	log "github.com/sirupsen/logrus"
	"github.com/spf13/cobra"
)

// buildCmd represents the build command
var buildCmd = &cobra.Command{
	Use:   "build",
	Short: "A brief description of your command",
	Long: `A longer description that spans multiple lines and likely contains examples
and usage of using your command. For example:

Cobra is a CLI library for Go that empowers applications.
This application is a tool to generate the needed files
to quickly create a Cobra application.`,
	Run: func(cmd *cobra.Command, args []string) {
		build, _ := cmd.Flags().GetString("build")
		compiler, _ := cmd.Flags().GetString("compiler")
		target, _ := cmd.Flags().GetString("target")
		system, _ := cmd.Flags().GetString("system")
		cmakedir, _ := cmd.Flags().GetString("cmakedir")
		log.Debug("build called", build, compiler, target, system, cmakedir)

		app.Build(build, compiler, target, system, cmakedir)
	},
}

func init() {
	appCmd.AddCommand(buildCmd)

	// Here you will define your flags and configuration settings.

	// Cobra supports Persistent Flags which will work for this command
	// and all subcommands, e.g.:
	// buildCmd.PersistentFlags().String("foo", "", "A help for foo")

	// Cobra supports local flags which will only run when this command
	// is called directly, e.g.:
	// buildCmd.Flags().BoolP("toggle", "t", false, "Help message for toggle")
	buildCmd.Flags().StringP("build", "b", "Debug", "Set the build type for your OpenCAXPlus app. Default is Debug.")
	buildCmd.Flags().StringP("compiler", "c", "gnu", "Set the compiler for your OpenCAXPlus app. Default is gnu.")
	buildCmd.Flags().StringP("target", "t", "all", "Set the build target for your OpenCAXPlus app. Default is all.")
	buildCmd.Flags().StringP("system", "s", runtime.GOOS, "Set the system for your OpenCAXPlus app. Default is the current system.")
	buildCmd.Flags().StringP("cmakedir", "d", "", "Set the cmake command directory. Default is system path.")

}

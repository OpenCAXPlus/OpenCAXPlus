/*
Copyright © 2022 NAME HERE <EMAIL ADDRESS>

*/
package cmd

import (
	"OpenCAXPlus/cli/pkg"
	"runtime"

	log "github.com/sirupsen/logrus"
	"github.com/spf13/cobra"
)

// buildCmd represents the build command
var appBuildCmd = &cobra.Command{
	Use:   "build",
	Short: "Build your OpenCAXPlus application",
	Long: `The command calls cmake to build the application. 
	You can specify the build type, compiler, build target name, 
	current system type, and the cmake bin directory.`,
	Run: func(cmd *cobra.Command, args []string) {
		build := pkg.GetFlag(cmd, "build")
		compiler := pkg.GetFlag(cmd, "compiler")
		target := pkg.GetFlag(cmd, "target")
		system := pkg.GetFlag(cmd, "system")
		cmakedir := pkg.GetFlag(cmd, "cmakedir")
		log.Debug("build called", build, compiler, target, system, cmakedir)

		pkg.AppBuild(build, compiler, target, system, cmakedir)
	},
}

func init() {
	appCmd.AddCommand(appBuildCmd)

	// Here you will define your flags and configuration settings.

	// Cobra supports Persistent Flags which will work for this command
	// and all subcommands, e.g.:
	// buildCmd.PersistentFlags().String("foo", "", "A help for foo")

	// Cobra supports local flags which will only run when this command
	// is called directly, e.g.:
	// buildCmd.Flags().BoolP("toggle", "t", false, "Help message for toggle")
	appBuildCmd.Flags().StringP("build", "b", "Debug", "Set the build type for your OpenCAXPlus app. Default is Debug.")
	appBuildCmd.Flags().StringP("compiler", "c", "gnu", "Set the compiler for your OpenCAXPlus app. Default is gnu.")
	appBuildCmd.Flags().StringP("target", "t", "all", "Set the build target for your OpenCAXPlus app. Default is all.")
	appBuildCmd.Flags().StringP("system", "s", runtime.GOOS, "Set the system for your OpenCAXPlus app. Default is the current system.")
	appBuildCmd.Flags().StringP("cmakedir", "d", "", "Set the cmake command directory. Default is system path.")

}

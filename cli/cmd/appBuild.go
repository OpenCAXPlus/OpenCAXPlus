/*
Copyright © 2022 NAME HERE <EMAIL ADDRESS>

*/
package cmd

import (
	"OpenCAXPlus/cli/pkg"
	"runtime"

	log "github.com/sirupsen/logrus"
	"github.com/spf13/cobra"
	"github.com/spf13/viper"
)

// buildCmd represents the build command
var appBuildCmd = &cobra.Command{
	Use:   "build",
	Short: "Build your OpenCAXPlus application",
	Long: `The command calls cmake to build the application. 
	You can specify the build type, compiler, build target name, 
	current system type, and the cmake bin directory.`,
	Run: func(cmd *cobra.Command, args []string) {
		build := viper.GetString("app.build")
		compiler := viper.GetString("app.compiler")
		target := viper.GetString("app.target")
		system := viper.GetString("app.system")
		cmakedir := viper.GetString("app.cmakedir")
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
	appBuildCmd.Flags().String("build", "b", "Set the build type for your OpenCAXPlus app. Default is Debug.")
	viper.BindPFlag("app.build", appBuildCmd.Flags().Lookup("build"))
	viper.SetDefault("app.build", "Debug")
	appBuildCmd.Flags().StringP("compiler", "c", "gnu", "Set the compiler for your OpenCAXPlus app. Default is gnu.")
	viper.BindPFlag("app.compiler", appBuildCmd.Flags().Lookup("compiler"))
	viper.SetDefault("app.compiler", "gnu")
	appBuildCmd.Flags().StringP("target", "t", "all", "Set the build target for your OpenCAXPlus app. Default is all.")
	viper.BindPFlag("app.target", appBuildCmd.Flags().Lookup("target"))
	viper.SetDefault("app.target", "all")
	appBuildCmd.Flags().StringP("system", "s", runtime.GOOS, "Set the system for your OpenCAXPlus app. Default is the current system.")
	viper.BindPFlag("app.system", appBuildCmd.Flags().Lookup("system"))
	viper.SetDefault("app.system", runtime.GOOS)
	appBuildCmd.Flags().StringP("cmakedir", "d", "", "Set the cmake command directory. Default is system path.")
	viper.BindPFlag("app.cmakedir", appBuildCmd.Flags().Lookup("cmakedir"))
	viper.SetDefault("app.cmakedir", "")

}

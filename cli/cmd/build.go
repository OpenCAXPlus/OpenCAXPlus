/*
Copyright © 2022 NAME HERE <EMAIL ADDRESS>

*/
package cmd

import (
	"OpenCAXPlusCli/pkg"
	"runtime"

	log "github.com/sirupsen/logrus"
	"github.com/spf13/cobra"
	"github.com/spf13/viper"
)

// buildCmd represents the build command
var buildCmd = &cobra.Command{
	Use:   "build",
	Short: "Build the OpenCAXPlus application or SDK",
	Long: `The command calls cmake to build the application. 
You can specify the build type, compiler, build target name, 
current system type, and the cmake bin directory.`,
	Run: func(cmd *cobra.Command, args []string) {
		build_type := viper.GetString("build.build_type")
		compiler := viper.GetString("build.compiler")
		target := viper.GetString("build.target")
		// system := viper.GetString("build.system")
		cmake_dir := viper.GetString("build.cmake_dir")
		app_path := viper.GetString("build.path")
		log.Debug("build called")
		log.Debug("Build type is ", build_type)
		log.Debug("System is ", runtime.GOOS)
		log.Debug("Compiler is ", compiler)
		log.Debug("Target is ", target)
		log.Debug("Cmake dir is ", cmake_dir)
		log.Debug("App Path is ", app_path)
		pkg.Build(build_type, compiler, target, cmake_dir, app_path)

	},
}

func init() {
	rootCmd.AddCommand(buildCmd)

	// Here you will define your flags and configuration settings.

	// Cobra supports Persistent Flags which will work for this command
	// and all subcommands, e.g.:
	// buildCmd.PersistentFlags().String("foo", "", "A help for foo")

	// Cobra supports local flags which will only run when this command
	// is called directly, e.g.:
	// buildCmd.Flags().BoolP("toggle", "t", false, "Help message for toggle")
	buildCmd.Flags().StringP("build_type", "b", "Debug", "Set the build type for your OpenCAXPlus app.")
	viper.BindPFlag("build.build_type", buildCmd.Flags().Lookup("build_type"))
	viper.SetDefault("build.build_type", "Debug")
	buildCmd.Flags().StringP("compiler", "c", "gnu", "Set the compiler for your OpenCAXPlus app. ")
	viper.BindPFlag("build.compiler", buildCmd.Flags().Lookup("compiler"))
	viper.SetDefault("build.compiler", "gnu")
	buildCmd.Flags().StringP("target", "t", "all", "Set the build target for your OpenCAXPlus app. ")
	viper.BindPFlag("build.target", buildCmd.Flags().Lookup("target"))
	viper.SetDefault("build.target", "all")
	buildCmd.Flags().StringP("path", "p", ".", "Set the path for your OpenCAXPlus app to be built. ")
	viper.BindPFlag("build.path", buildCmd.Flags().Lookup("path"))
	viper.SetDefault("build.path", ".")
	buildCmd.Flags().StringP("cmake_dir", "d", "default", "Set the cmake command directory.")
	viper.BindPFlag("build.cmake_dir", buildCmd.Flags().Lookup("cmake_dir"))
	viper.SetDefault("build.cmake_dir", "default")

}

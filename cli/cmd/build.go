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
		preset := viper.GetString("build.preset")
		target := viper.GetString("build.target")
		app_path := viper.GetString("build.src")
		log.Debug("build called")
		// log.Debug("Build type is ", build_type)
		log.Debug("System is ", runtime.GOOS)
		// log.Debug("Compiler is ", compiler)
		log.Debug("Preset is ", preset)
		log.Debug("Target is ", target)
		log.Debug("App Path is ", app_path)
		pkg.Build(preset, target, app_path)

	},
}

func init() {
	rootCmd.AddCommand(buildCmd)
	buildCmd.Flags().StringP("preset", "p", "linux-gnu-Debug", "Set the preset for your OpenCAXPlus app.")
	viper.BindPFlag("build.preset", buildCmd.Flags().Lookup("preset"))
	viper.SetDefault("build.preset", "linux-gnu-Debug")
	buildCmd.Flags().StringP("target", "t", "all", "Set the build target for your OpenCAXPlus app. ")
	viper.BindPFlag("build.target", buildCmd.Flags().Lookup("target"))
	viper.SetDefault("build.target", "all")
	buildCmd.Flags().StringP("src", "s", ".", "Set the path for your OpenCAXPlus app to be built. ")
	viper.BindPFlag("build.src", buildCmd.Flags().Lookup("path"))
	viper.SetDefault("build.src", ".")
}

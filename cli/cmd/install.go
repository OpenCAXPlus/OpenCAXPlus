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
var installCmd = &cobra.Command{
	Use:   "install",
	Short: "Install selected tool",
	Long:  `The command calls cmake to install the tool you specified.`,
	Args:  cobra.MinimumNArgs(1),
	Run: func(cmd *cobra.Command, args []string) {
		build_type := viper.GetString("install.build_type")
		compiler := viper.GetString("install.compiler")
		target := "install_" + args[0]
		// system := viper.GetString("build.system")
		cmake_dir := viper.GetString("install.cmake_dir")
		log.Debug("build called")
		log.Debug("Build type is ", build_type)
		log.Debug("System is ", runtime.GOOS)
		log.Debug("Compiler is ", compiler)
		log.Debug("Target is ", target)
		log.Debug("Cmake dir is ", cmake_dir)
		app_path := "/opt/ocp"
		pkg.Build(build_type, compiler, target, cmake_dir, app_path)

	},
}

func init() {
	rootCmd.AddCommand(installCmd)
	installCmd.Flags().StringP("build_type", "b", "Release", "Set the build type for your toolkit build.")
	viper.BindPFlag("install.build_type", installCmd.Flags().Lookup("build_type"))
	viper.SetDefault("install.build_type", "Debug")
	installCmd.Flags().StringP("compiler", "c", "gnu", "Set the compiler for your OpenCAXPlus app. ")
	viper.BindPFlag("install.compiler", installCmd.Flags().Lookup("compiler"))
	viper.SetDefault("install.compiler", "gnu")
	installCmd.Flags().StringP("cmake_dir", "d", "default", "Set the cmake command directory.")
	viper.BindPFlag("install.cmake_dir", installCmd.Flags().Lookup("cmake_dir"))
	viper.SetDefault("install.cmake_dir", "default")

}

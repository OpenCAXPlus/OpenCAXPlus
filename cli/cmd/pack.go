/*
Copyright © 2022 NAME HERE <EMAIL ADDRESS>

*/
package cmd

import (
	"OpenCAXPlus/cli/pkg"

	log "github.com/sirupsen/logrus"
	"github.com/spf13/cobra"
	"github.com/spf13/viper"
)

// packCmd represents the pack command
var packCmd = &cobra.Command{
	Use:   "pack",
	Short: "Package the project for release.",
	Long:  `The pack command call cpack and create packages as defined in the CMakePresets.json.`,
	Run: func(cmd *cobra.Command, args []string) {
		log.Debug("pack called")
		build_type := viper.GetString("build.build_type")
		compiler := viper.GetString("build.compiler")
		cmake_dir := viper.GetString("cmake_dir")
		pkg.Pack(build_type, compiler, cmake_dir)
	},
}

func init() {
	rootCmd.AddCommand(packCmd)

	// Here you will define your flags and configuration settings.

	// Cobra supports Persistent Flags which will work for this command
	// and all subcommands, e.g.:
	// packCmd.PersistentFlags().String("foo", "", "A help for foo")

	// Cobra supports local flags which will only run when this command
	// is called directly, e.g.:
	// packCmd.Flags().BoolP("toggle", "t", false, "Help message for toggle")
	packCmd.Flags().StringP("build_type", "b", "Debug", "Set the build type for your OpenCAXPlus app.")
	viper.BindPFlag("test.build_type", packCmd.Flags().Lookup("build_type"))
	viper.SetDefault("test.build_type", "Debug")
	packCmd.Flags().StringP("compiler", "c", "gnu", "Set the compiler for your OpenCAXPlus app. ")
	viper.BindPFlag("test.compiler", packCmd.Flags().Lookup("compiler"))
	viper.SetDefault("test.compiler", "gnu")
	packCmd.Flags().StringP("cmake_dir", "d", "", "Set the cmake command directory. Default is system path.")
	viper.BindPFlag("cmake_dir", packCmd.Flags().Lookup("cmake_dir"))
	viper.SetDefault("cmake_dir", "")
}

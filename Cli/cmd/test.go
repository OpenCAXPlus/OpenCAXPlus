/*
Copyright © 2022 NAME HERE <EMAIL ADDRESS>

*/
package cmd

import (
	"OpenCAXPlus/Cli/pkg"

	log "github.com/sirupsen/logrus"
	"github.com/spf13/cobra"
	"github.com/spf13/viper"
)

// testCmd represents the test command
var testCmd = &cobra.Command{
	Use:   "test",
	Short: "Run ctests",
	Long:  `The test command run ctest.`,
	Run: func(cmd *cobra.Command, args []string) {
		build_type := viper.GetString("test.build_type")
		compiler := viper.GetString("test.compiler")
		cmake_dir := viper.GetString("cmake_dir")
		log.Debug("test called")
		pkg.Test(build_type, compiler, cmake_dir)
	},
}

func init() {
	rootCmd.AddCommand(testCmd)
	testCmd.Flags().StringP("build_type", "b", "Debug", "Set the build type for your OpenCAXPlus app.")
	viper.BindPFlag("test.build_type", testCmd.Flags().Lookup("build_type"))
	viper.SetDefault("test.build_type", "Debug")
	testCmd.Flags().StringP("compiler", "c", "gnu", "Set the compiler for your OpenCAXPlus app. ")
	viper.BindPFlag("test.compiler", testCmd.Flags().Lookup("compiler"))
	viper.SetDefault("test.compiler", "gnu")
	testCmd.Flags().StringP("cmake_dir", "d", "", "Set the cmake command directory. Default is system path.")
	viper.BindPFlag("cmake_dir", testCmd.Flags().Lookup("cmake_dir"))
	viper.SetDefault("cmake_dir", "")
}

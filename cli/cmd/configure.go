/*
Copyright © 2022 Xiaoxing Cheng <hello@chengxiaoxing.me>
*/
package cmd

import (
	"OpenCAXPlusCli/pkg"
	"runtime"

	log "github.com/sirupsen/logrus"
	"github.com/spf13/cobra"
	"github.com/spf13/viper"
)

// the configure for different builder type should be different
// configure will run the script if it's external
// configure will use cmake and preset if
var configureCmd = &cobra.Command{
	Use:   "configure",
	Short: "Configure the OpenCAXPlus application or SDK",
	Long: `The command calls cmake to configure the application.
	You can specify the build type, compiler, build target name,
	current system type, and the cmake bin directory.`,
	Args: cobra.MaximumNArgs(0),
	Run: func(cmd *cobra.Command, args []string) {
		preset := viper.GetString("configure.preset")
		id := viper.GetString("configure.configuration")
		app_path := viper.GetString("configure.src")

		yml, _ := pkg.ParseOCPYaml(app_path)
		if preset == "" {
			preset = yml.Default.Preset
		}

		log.Debug("configure called")
		log.Debug("Configuration type is ", id)
		log.Debug("System is ", runtime.GOOS)
		log.Debug("Preset is ", preset)
		log.Debug("App Path is ", app_path)
		pkg.Configure(id, preset, app_path)
	},
}

func init() {
	rootCmd.AddCommand(configureCmd)
	configureCmd.Flags().StringP("preset", "p", "", "Set the preset for your OpenCAXPlus app.")
	viper.BindPFlag("configure.preset", configureCmd.Flags().Lookup("preset"))
	viper.SetDefault("configure.preset", "")
	configureCmd.Flags().StringP("configuration", "c", "", "Set the id for your OpenCAXPlus app. ")
	viper.BindPFlag("configure.configuration", configureCmd.Flags().Lookup("Configuration"))
	viper.SetDefault("configure.configuration", "")
	configureCmd.Flags().StringP("src", "s", ".", "Set the path for your OpenCAXPlus app to be built. ")
	viper.BindPFlag("configure.src", configureCmd.Flags().Lookup("path"))
	viper.SetDefault("configure.src", ".")
}

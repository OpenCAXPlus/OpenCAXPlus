/*
Copyright © 2023 Xiaoxing Cheng <hello@chengxiaoxing.me>
*/
package cmd

import (
	"OpenCAXPlusCli/pkg"
	"runtime"

	log "github.com/sirupsen/logrus"
	"github.com/spf13/cobra"
)

// the update command will
// 1. download the install.sh and packages.yml file
// 2. source the install.sh file
var updateCmd = &cobra.Command{
	Use:   "update",
	Short: "ocp update",
	Long: `
	The command will update the packages.yml file and the ocp cli.`,
	Args: cobra.ExactArgs(0),
	Run: func(cmd *cobra.Command, args []string) {

		log.Debug("update command")
		log.Debug("System is ", runtime.GOOS)

		pkg.Update()

	},
}

func init() {
	rootCmd.AddCommand(updateCmd)
}

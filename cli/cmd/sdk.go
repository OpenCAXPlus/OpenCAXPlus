/*
Copyright © 2022 NAME HERE <EMAIL ADDRESS>

*/
package cmd

import (
	"fmt"

	"github.com/spf13/cobra"
)

// sdkCmd represents the sdk command
var sdkCmd = &cobra.Command{
	Use:   "sdk",
	Short: "Manage the OpenCAXPlus SDK",
	Long: `You can build, test the SDK, create new lifecycle, 
create new starter, build, test, run starter, create new plugin.`,
	Run: func(cmd *cobra.Command, args []string) {
		fmt.Println("sdk called")
	},
}

func init() {
	rootCmd.AddCommand(sdkCmd)

	// Here you will define your flags and configuration settings.

	// Cobra supports Persistent Flags which will work for this command
	// and all subcommands, e.g.:
	// sdkCmd.PersistentFlags().String("foo", "", "A help for foo")

	// Cobra supports local flags which will only run when this command
	// is called directly, e.g.:
	// sdkCmd.Flags().BoolP("toggle", "t", false, "Help message for toggle")
}

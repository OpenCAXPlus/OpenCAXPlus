/*
Copyright © 2022 NAME HERE <EMAIL ADDRESS>

*/
package cmd

import (
	"fmt"

	"github.com/spf13/cobra"
)

// starterCmd represents the starter command
var starterCmd = &cobra.Command{
	Use:   "starter",
	Short: "Create a new OpenCAXPlus starter",
	Long: `Create a new OpenCAXPlus starter, which is a template that
others can use to initiate new application using the command：
ocp create app -t [starter_name]`,
	Run: func(cmd *cobra.Command, args []string) {
		fmt.Println("starter called")
	},
}

func init() {
	createCmd.AddCommand(starterCmd)

	// Here you will define your flags and configuration settings.

	// Cobra supports Persistent Flags which will work for this command
	// and all subcommands, e.g.:
	// starterCmd.PersistentFlags().String("foo", "", "A help for foo")

	// Cobra supports local flags which will only run when this command
	// is called directly, e.g.:
	// starterCmd.Flags().BoolP("toggle", "t", false, "Help message for toggle")
}

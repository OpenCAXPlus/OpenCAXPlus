/*
Copyright © 2022 NAME HERE <EMAIL ADDRESS>

*/
package cmd

import (
	"fmt"

	"github.com/spf13/cobra"
)

// lifecycleCmd represents the lifecycle command
var lifecycleCmd = &cobra.Command{
	Use:   "lifecycle",
	Short: "Create a new OpenCAXPlus SDK lifecycle",
	Long: `Create a new OpenCAXPlus SDK lifecycle. Lifecycle is part of the SDK's framework
that defines the procedures of how a program should run.`,
	Run: func(cmd *cobra.Command, args []string) {
		fmt.Println("lifecycle called")
	},
}

func init() {
	createCmd.AddCommand(lifecycleCmd)

	// Here you will define your flags and configuration settings.

	// Cobra supports Persistent Flags which will work for this command
	// and all subcommands, e.g.:
	// lifecycleCmd.PersistentFlags().String("foo", "", "A help for foo")

	// Cobra supports local flags which will only run when this command
	// is called directly, e.g.:
	// lifecycleCmd.Flags().BoolP("toggle", "t", false, "Help message for toggle")
}

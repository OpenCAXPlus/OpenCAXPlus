package main

import (
	"OpenCAXPlus/cli/pkg/plugin"
	"flag"
	"fmt"
	"os"
)

func PluginParse(cmd *flag.FlagSet) {
	fmt.Println("Hello world from cmd/plugin")
	var anything string
	cmd.StringVar(&anything, "a", "anything", "take argument")
	cmd.StringVar(&anything, "anything", "anything", "take argument")
	cmd.Parse(os.Args[2:])
	plugin.Info(anything)
}

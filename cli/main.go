package main

import (
	"OpenCAXPlus/cli/cmd"
	"flag"
	"fmt"
	"os"
)

func main() {
	fmt.Println("Hello world from cli test", os.Args)
	app_cmd := flag.NewFlagSet("app", flag.ExitOnError)
	plugin_cmd := flag.NewFlagSet("plugin", flag.ExitOnError)
	tool_cmd := flag.NewFlagSet("tool", flag.ExitOnError)

	switch os.Args[1] {
	case "app":
		cmd.AppParse(app_cmd)
	case "plugin":
		cmd.PluginParse(plugin_cmd)
	case "tool":
		cmd.PluginParse(tool_cmd)
	}
}

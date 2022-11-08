package main

import (
	"flag"
	"fmt"
	"os"
)

func main() {
	fmt.Println("Hello world from cli", os.Args)
	app_cmd := flag.NewFlagSet("app", flag.ExitOnError)
	plugin_cmd := flag.NewFlagSet("plugin", flag.ExitOnError)
	tool_cmd := flag.NewFlagSet("tool", flag.ExitOnError)

	switch os.Args[1] {
	case "app":
		AppParse(app_cmd)
	case "plugin":
		PluginParse(plugin_cmd)
	case "tool":
		PluginParse(tool_cmd)
	}
}

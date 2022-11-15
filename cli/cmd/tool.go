package cmd

import (
	"OpenCAXPlus/cli/pkg/tool"
	"flag"
	"fmt"
	"os"
)

func ToolParse(cmd *flag.FlagSet) {
	fmt.Println("Hello world from cmd/tool")
	var anything string
	cmd.StringVar(&anything, "a", "anything", "take argument")
	cmd.StringVar(&anything, "anything", "anything", "take argument")
	cmd.Parse(os.Args[2:])
	tool.Info(anything)
}

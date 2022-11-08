package main

import (
	"OpenCAXPlus/cli/cmd/internal/isFlagPassed"
	"OpenCAXPlus/cli/pkg/app"
	"flag"
	"fmt"
	"os"
)

func AppParse(cmd *flag.FlagSet) {
	fmt.Println("Hello world from cmd/app")
	var anything string
	cmd.StringVar(&anything, "a", "anything", "take argument")
	cmd.StringVar(&anything, "anything", "anything", "take argument")
	cmd.Parse(os.Args[2:])
	switch {
	case isFlagPassed.FS(*cmd, "a") || isFlagPassed.FS(*cmd, "anything"):
		app.Info(anything)
	}
}

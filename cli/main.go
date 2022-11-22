/*
Copyright © 2022 Xiaoxing Cheng hello@chengxiaoxing.me

*/
package main

import (
	"OpenCAXPlus/cli/cmd"

	log "github.com/sirupsen/logrus"
)

func init() {
	log.SetLevel(log.DebugLevel)
}

func main() {
	cmd.Execute()
}

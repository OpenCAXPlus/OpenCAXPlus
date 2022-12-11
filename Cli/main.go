/*
Copyright © 2022 Xiaoxing Cheng hello@chengxiaoxing.me

*/
package main

import (
	"OpenCAXPlusCli/cmd"

	log "github.com/sirupsen/logrus"
)

func init() {
	log.SetLevel(log.DebugLevel)
}

func main() {
	cmd.Execute()
}

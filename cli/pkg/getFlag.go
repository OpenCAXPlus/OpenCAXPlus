package pkg

import (
	"fmt"

	log "github.com/sirupsen/logrus"
	"github.com/spf13/cobra"
	"github.com/spf13/viper"
)

func GetFlag(cmd *cobra.Command, flagName string) string {
	flag := ""
	settings := viper.AllSettings()
	if v, found := settings[flagName]; found {
		flag = fmt.Sprintf("%v", v)
		log.Debug("found")
	} else {
		flag, _ = cmd.Flags().GetString(flagName)
		log.Debug("not found")
	}
	return flag
}

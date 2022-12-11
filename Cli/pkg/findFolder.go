package pkg

import (
	"os"

	log "github.com/sirupsen/logrus"
)

func FindFolder(path string) bool {
	_, err := os.Stat(path)
	if os.IsNotExist(err) {
		log.Debug("Folder %s does not exist.", path)
		return false
	}
	log.Debug("Folder %s exist.", path)
	return true
}

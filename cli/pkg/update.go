package pkg

import (
	"os"
	"path/filepath"

	log "github.com/sirupsen/logrus"
)

// ! TODO need to run the Download recursively on config dependencies
func Update(packages string) {

	// Read the packages.yml file
	homeDir, _ := os.UserHomeDir()
	downloadPath := filepath.Join(homeDir, "ocp")
	downloadUrl := "https://ocp-download.oss-cn-hongkong.aliyuncs.com/packages.yml"
	downloadFile := filepath.Join(downloadPath, "packages.yml")
	err := DownloadFile(downloadUrl, downloadFile)
	if err != nil {
		panic(err)
	}
	log.Println("Successfully downloaded ", downloadUrl, " to ", downloadFile)

	if packages == "" {
		downloadPath = filepath.Join(homeDir, "ocp", "download")
		downloadUrl = "https://ocp-download.oss-cn-hongkong.aliyuncs.com/install.sh"
		downloadFile = filepath.Join(downloadPath, "install.sh")
		err = DownloadFile(downloadUrl, downloadFile)
		if err != nil {
			panic(err)
		}
		log.Println("Successfully downloaded ", downloadUrl, " to ", downloadFile)

		executeCommand("source " + downloadFile)
	}
}

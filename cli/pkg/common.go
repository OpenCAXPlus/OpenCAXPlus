package pkg

import (
	"io"
	"os"
	"os/exec"
	"path/filepath"
	"runtime"

	log "github.com/sirupsen/logrus"
)

func prepareIntel() string {
	system := runtime.GOOS
	source_intel := ""
	switch system {
	case "linux":
		source_intel = `source /opt/intel/oneapi/setvars.sh
`
		log.Debug("inside linux")
	case "darwin":
		source_intel = `source /opt/intel/oneapi/setvars.sh
export CC=icc
export CXX=icpc
`
	case "windows":
		source_intel = "\"C:/Program Files (x86)/Intel/oneAPI/setvars.bat\"" + newline()
	}
	return source_intel
}

func cmakeCmd(compiler string, cmakedir string, bin string) string {
	// system := runtime.GOOS
	cmake := "cmake"
	// if system == "windows" {
	// 	if cmakedir == "default" {
	// 		cmakedir = "C:\\PROGRAM FILES\\CMAKE\\bin"
	// 	}
	// 	cmake = "\"" + filepath.Join(cmakedir, bin) + "\""
	// } else {
	// }
	if cmakedir == "default" {
		cmakedir = ""
	}
	cmake = filepath.Join(cmakedir, bin)
	return cmake
}

func newline() string {
	if runtime.GOOS == "windows" {
		return "\r\n"
	} else {
		return "\n"
	}
}
func executeCommand(commands string) {
	system := runtime.GOOS
	shell := "bash"
	switch system {
	case "linux":
		shell = "bash"
	case "darwin":
		shell = "zsh"
	case "windows":
		shell = "cmd.exe"
	}
	log.Info("Start a new command ")
	log.Info(newline() + commands)
	cmd := exec.Command(shell)
	cmdIn, _ := cmd.StdinPipe()
	cmd.Stdout = os.Stdout
	cmd.Stderr = os.Stderr
	cmd.Start()
	io.WriteString(cmdIn, commands)
	cmdIn.Close()
	if err := cmd.Wait(); err != nil {
		if exiterr, ok := err.(*exec.ExitError); ok {
			log.Infof("Exit Status: %d", exiterr.ExitCode())
		} else {
			log.Errorf("cmd.Wait: %v", err)
		}
	}
	log.Info("Command finished")
}

package pkg

import (
	"fmt"
	"io"
	"os"
	"os/exec"
	"path/filepath"

	log "github.com/sirupsen/logrus"
)

func prepareIntel(system string) string {
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
		source_intel = `C:/Program Files(x86)/Intel/oneAPI/setvars.bat
`
	}
	return source_intel
}

func cmakeCmd(compiler string, system string, cmakedir string) string {
	cmake := "cmake"
	if cmakedir == "" && system == "windows" && compiler == "intel" {
		cmakedir = "C:\\PROGRAM FILES (X86)\\MICROSOFT VISUAL STUDIO\\2019\\COMMUNITY\\COMMON7\\IDE\\COMMONEXTENSIONS\\MICROSOFT\\CMAKE\\CMake\\bin"
	}
	cmake = filepath.Join(cmakedir, "cmake")
	return cmake
}

func executeCommand(system string, compiler string, commands string) {
	shell := "bash"
	switch system {
	case "linux":
		shell = "bash"
	case "darwin":
		shell = "zsh"
	case "windows":
		if compiler == "intel" {
			shell = "cmd.exe"
		} else {
			shell = "mingw64.exe"
		}
	}
	log.Info("Start a new command ")
	log.Info("\n" + commands)
	cmd := exec.Command(shell)
	cmdIn, _ := cmd.StdinPipe()
	cmd.Stdout = os.Stdout
	cmd.Stderr = os.Stderr
	cmd.Start()
	io.WriteString(cmdIn, commands)
	cmdIn.Close()
	cmd.Wait()
	log.Info("Command finished")
}

func commandBuild(build string, compiler string, target string, system string, cmakedir string) string {
	cmake := cmakeCmd(compiler, system, cmakedir)
	cmd := fmt.Sprintf(`%s --preset="%s-%s-%s" -S "."
%s --build --preset="%s-%s-%s" --target %s
`, cmake, system, compiler, build, cmake, system, compiler, build, target)
	return cmd
}

func commandTest(build string, compiler string, target string, system string, cmakedir string) string {
	cmake := cmakeCmd(compiler, system, cmakedir)
	cmd := fmt.Sprintf(`%s --preset="%s-%s-%s" -S "."
%s --build --preset="%s-%s-%s" --target %s
`, cmake, system, compiler, build, cmake, system, compiler, build, target)
	return cmd
}

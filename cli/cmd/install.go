/*
Copyright © 2022 NAME HERE <EMAIL ADDRESS>
*/
package cmd

import (
	"OpenCAXPlusCli/pkg"
	"fmt"
	"os"
	"path/filepath"
	"runtime"

	log "github.com/sirupsen/logrus"
	"github.com/spf13/cobra"
)

// buildCmd represents the build command
var installCmd = &cobra.Command{
	Use:   "install",
	Short: "Install selected tool",
	Long:  `The command calls cmake to install the tool you specified.`,
	Args:  cobra.MaximumNArgs(0),
	Run: func(cmd *cobra.Command, args []string) {

		log.Debug("System is ", runtime.GOOS)
		//if not, download and install
		cwd, _ := os.Getwd()

		path := filepath.Join(cwd, "cmake")
		if _, err := os.Stat(path); os.IsNotExist(err) {
			// Create the directory if it does not exist
			err := os.MkdirAll(path, 0755)
			if err != nil {
				panic(err)
			}
			fmt.Printf("Directory %s created\n", path)
		}

		// parse the config file
		ocp := pkg.GetOCPAsInstallPackage(cwd)
		// loop through all the packages in the config file
		depPackages := pkg.GetConfigurationDependencies(cwd, ocp)
		log.Debug("Install called for", ocp)
		log.Debug("List of dependencies: ", depPackages)
		for _, dep := range depPackages {
			log.Debug("Installing dependency: ", dep)
			//check if the package is already installed
			path := pkg.GetPackageInstallPath(dep)
			if _, err := os.Stat(path); os.IsNotExist(err) {
				pkg.Download(dep)
				pkg.Install(dep)
			} else {
				log.Debug("Package already installed ", path)
			}
			// copy the necessary cmake files
			if ocp.Type == "starter" {
				srcPath := pkg.GetPackageSourcePath(dep)
				srcFile := filepath.Join(srcPath, "cmake", dep.Configuration+".cmake")
				dstFile := filepath.Join(cwd, "cmake", dep.UID+"-"+dep.Configuration+".cmake")
				log.Debug("Copying cmake files from ", srcFile, " to ", dstFile)
				pkg.CopyFile(srcFile, dstFile)
			}
		}
		if ocp.Type == "starter" {
			// prepare the cmake/Dependencies.cmake file
			file, err := os.Create(filepath.Join(cwd, "cmake", "Dependencies.cmake"))
			if err != nil {
				panic(err)
			}
			defer file.Close()

			// Write variables to the file
			fmt.Fprintf(file, "# This file is automatically generated by ocp\n")
			fmt.Fprintf(file, "# Do not modify it manually, use the DependenciesUser.cmake for customization\n\n")
			for _, dep := range depPackages {
				fmt.Fprintf(file, "set(%s_VERSION %s)\n", dep.UID, dep.Version)
				fmt.Fprintf(file, "include(%s-%s)\n", dep.UID, dep.Configuration)
			}
			fmt.Fprintf(file, "\ninclude(DependenciesUser)\n")
			fmt.Fprintf(file, "add_library(${PROJECT_NAME}_dependencies INTERFACE)\n")
			fmt.Fprintf(file, "add_library(OCP::Dependencies ALIAS ${PROJECT_NAME}_dependencies)\n\n")
			fmt.Fprintf(file, "target_link_libraries(${PROJECT_NAME}_dependencies INTERFACE")
			for _, dep := range depPackages {
				fmt.Fprintf(file, " ${%s_target}", dep.UID)
			}
			fmt.Fprintf(file, ")\n\n")
			fmt.Fprintf(file, "target_link_libraries(${StarterLibraryName} PUBLIC OCP::Interface OCP::Dependencies)\n")
			fmt.Fprintf(file, "target_include_directories(${StarterLibraryName} PUBLIC ${OCP_LIFECYCLE}/${LifeCycleName})\n")
		}
	},
}

func init() {
	rootCmd.AddCommand(installCmd)
}

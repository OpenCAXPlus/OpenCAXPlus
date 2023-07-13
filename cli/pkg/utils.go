package pkg

import (
	"archive/tar"
	"fmt"
	"io"
	"io/ioutil"
	"net/http"
	"os"
	"path/filepath"
	"time"

	"github.com/ulikunitz/xz"
)

func MostRecentSubFolder(dir string) (mostRecentSubDir string, modTime time.Time, err error) {
	files, err := ioutil.ReadDir(dir)
	if err != nil {
		return "", time.Time{}, err
	}

	for _, file := range files {
		if file.IsDir() {
			currentModTime := file.ModTime()
			if currentModTime.After(modTime) {
				modTime = currentModTime
				// mostRecentSubDir = filepath.Join(dir, file.Name())
				mostRecentSubDir = file.Name()
			}
		}
	}
	return mostRecentSubDir, modTime, nil
}

// DownloadFile will download a url to a local file.
func DownloadFile(url string, filepathStr string) error {
	// Create the parent directory if it doesn't exist
	if err := os.MkdirAll(filepath.Dir(filepathStr), 0755); err != nil {
		return err
	}

	// Create the file
	out, err := os.Create(filepathStr)
	if err != nil {
		return err
	}
	defer out.Close()

	// Get the data
	resp, err := http.Get(url)
	if err != nil {
		return err
	}
	defer resp.Body.Close()

	// Write the body to file
	_, err = io.Copy(out, resp.Body)
	return err
}

func DecompressTarXZ(src, dst string) error {
	r, err := os.Open(src)
	if err != nil {
		return fmt.Errorf("failed to open file: %w", err)
	}
	defer r.Close()

	xzr, err := xz.NewReader(r)
	if err != nil {
		return fmt.Errorf("failed to create xz reader: %w", err)
	}

	tr := tar.NewReader(xzr)

	for {
		hdr, err := tr.Next()
		if err == io.EOF {
			break // End of archive
		}
		if err != nil {
			return fmt.Errorf("failed to read tar header: %w", err)
		}

		outPath := filepath.Join(dst, hdr.Name)
		switch hdr.Typeflag {
		case tar.TypeDir:
			if _, err := os.Stat(outPath); err != nil {
				if err := os.MkdirAll(outPath, 0755); err != nil {
					return fmt.Errorf("failed to mkdir %s: %w", outPath, err)
				}
			}
		case tar.TypeReg:
			outFile, err := os.Create(outPath)
			if err != nil {
				return fmt.Errorf("failed to create file %s: %w", outPath, err)
			}
			if _, err := io.Copy(outFile, tr); err != nil {
				return fmt.Errorf("failed to copy file content: %w", err)
			}
			outFile.Close()
		default:
			return fmt.Errorf("unsupported type: %s", hdr.Typeflag)
		}
	}

	return nil
}

func FolderExists(foldername string) bool {
	_, err := os.Stat(foldername)
	if os.IsNotExist(err) {
		return false
	}
	return err == nil
}

func CopyFile(src string, dst string) error {
	in, err := os.Open(src)
	if err != nil {
		return err
	}
	defer in.Close()

	out, err := os.Create(dst)
	if err != nil {
		return err
	}
	defer out.Close()

	_, err = io.Copy(out, in)
	return err
}

func CopyDir(src string, dst string) error {
	return filepath.Walk(src, func(srcPath string, info os.FileInfo, err error) error {
		if err != nil {
			return err
		}

		relPath, err := filepath.Rel(src, srcPath)
		if err != nil {
			return err
		}

		dstPath := filepath.Join(dst, relPath)

		if info.Mode()&os.ModeSymlink != 0 {
			target, err := os.Readlink(srcPath)
			if err != nil {
				return err
			}
			return os.Symlink(target, dstPath)
		}

		if info.IsDir() {
			return os.MkdirAll(dstPath, info.Mode())
		}

		if err := CopyFile(srcPath, dstPath); err != nil {
			return err
		}

		return os.Chmod(dstPath, info.Mode())
	})
}

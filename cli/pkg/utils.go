package pkg

import (
	"archive/tar"
	"fmt"
	"io"
	"net/http"
	"os"
	"path/filepath"

	"github.com/ulikunitz/xz"
)

func DownloadFile(id, version, src, dst string) {

	downloadFile := filepath.Join(dst, id+"-"+version+".tar.xz")

	resp, err := http.Get(src + "/" + id + "/" + id + "-" + version + ".tar.xz")
	if err != nil {
		panic(err)
	}
	defer resp.Body.Close()

	out, err := os.Create(downloadFile) // update the extension based on your actual files
	if err != nil {
		panic(err)
	}
	defer out.Close()

	_, err = io.Copy(out, resp.Body)
	if err != nil {
		panic(err)
	}

	fmt.Println("Successfully downloaded", id+"-"+version+".tar.xz")
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

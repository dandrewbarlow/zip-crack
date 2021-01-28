package main

import (
	"bufio"
	"errors"
	"fmt"
	"io/ioutil"
	"os"

	"github.com/jessevdk/go-flags"
	"github.com/yeka/zip"
)

const (
	characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
)

// list of all generated or provided passwords to try out
var valueList []string

// options struct for go-flags library
type options struct {
	K       int      `short:"k" long:"permutations" description:"Length of permutations" default:"4"`
	Lists   []string `short:"l" long:"lists" description:"Provide password lists to check. Option bypasses permutation generation"`
	X       bool     `short:"x" long:"extract" description:"Extract permutation list to stdout."`
	ZipFile string   `short:"f" long:"file" description:"Provide encrypted zip filepath"`
	Help    bool     `short:"h" long:"help" description:"Show help menu"`
}

// parse options
func parseOptions() (options, error) {
	var opts options
	p := flags.NewParser(&opts, flags.Default&^flags.HelpFlag)

	_, err := p.Parse()

	if err != nil {
		os.Exit(1)
	}

	if opts.X {

	}

	if opts.ZipFile == "" && opts.X == false && !opts.Help {
		fmt.Println("Error : not enough parameters provided")
		p.WriteHelp(os.Stdout)
		os.Exit(1)
	}

	if opts.Help {
		p.WriteHelp(os.Stdout)
		os.Exit(0)
	}

	return opts, err
}

func parseLists(paths []string) error {
	for _, path := range paths {

		file, err := os.Open(path)
		defer file.Close()

		if err != nil {
			return err
		}

		scanner := bufio.NewScanner(file)

		for scanner.Scan() {
			valueList = append(valueList, scanner.Text())
		}
		if scanner.Err() != nil {
			return scanner.Err()
		}
	}
	return nil
}

// credit to https://www.geeksforgeeks.org/print-all-combinations-of-given-length/
// modified, but only a lil
// Gets all permutations of K length of a list w/ length n; prefix is for recursive implementation, empty on start
func getKPermutations(list string, prefix string, k int, x bool) {
	if k == 0 {
		// x = xtract no need to waste all that memory, just write to stdout
		if x {
			fmt.Printf("%v\n", prefix)
		} else {
			valueList = append(valueList, prefix)
		}
		return

	}

	for _, i := range list {

		newPrefix := prefix + string(i)
		getKPermutations(list, newPrefix, k-1, x)
	}

}

// get permutations of all lengths up to k
func getAllPermutations(len int, x bool) {
	/*
		to clarify why I'm seperating this from the getKPermutations function:
		getKPermutations returns all permutations strictly of length K
		when i == 1, output == 'a', 'b', 'c', etc
		i == 2, 'aa', 'ab', 'ac'
		perhaps possible put in one function, but this is simpler tbh (unless I'm missing something obvious)
	*/
	for i := 0; i <= len; i++ {
		getKPermutations(characters, "", i, x)
	}
}

// use the argument password to try to unlock the zip; return true if successful, and errors when not expected
func decrypt(password string, zipFile string) (bool, error) {

	archive, err := zip.OpenReader(zipFile)

	if err != nil {
		return false, err
	}

	defer archive.Close()

	for _, f := range archive.File {
		if f.IsEncrypted() {
			f.SetPassword(password)
		}

		r, err := f.Open()
		if err != nil {
			return false, err
		}

		defer r.Close()

		_, err = ioutil.ReadAll(r)
		if err != nil {
			// we're gonna get a lot of these
			return false, nil
		}
		return true, nil

	}

	return false, nil
}

// try all generated permutations on my poor .zip
func bruteForce(list []string, zipfile string) (string, error) {
	for _, p := range list {
		validPassword, err := decrypt(p, zipfile)
		if err != nil {
			return "", err
		} else if validPassword {
			return p, err
		}
	}
	return "", errors.New("Brute Force returned no hits")
}

// main entry
func main() {
	opts, err := parseOptions()

	if len(opts.Lists) == 0 {
		getAllPermutations(opts.K, opts.X)
	} else {
		err := parseLists(opts.Lists)
		if err != nil {
			fmt.Printf("%v\n", err)
		}
	}

	// xtraction does'nt try to bruteForce
	if opts.X {
		return
	}

	password, err := bruteForce(valueList, opts.ZipFile)

	if err != nil {
		fmt.Printf("%v\n", err)
		return
	} else if password != "" {
		fmt.Printf("password: %s\n", password)
	}
}

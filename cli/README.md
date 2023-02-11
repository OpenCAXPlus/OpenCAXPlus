# OpenCAXPlusCLI

The OpenCAXPlus CLI

### Common go command
```
go mod init ....
go run ....
go help
go mod tidy
go get ...
go work init ...
go work use ./test
go build -o ../ocp
go test
go test -cover
go test -coverprofile=ignore.out
go tool cover -html=ignore.out
```

```sh
cobra-cli init --author "Xiaoxing Cheng hello@chengxiaoxing.me" --viper
cobra-cli add app
cobra-cli add create -p 'appCmd'
cobra-cli add build -p 'appCmd'
cobra-cli add run -p 'appCmd'
cobra-cli add test -p 'appCmd'
cobra-cli add pack -p 'appCmd'
cobra-cli add sdk
cobra-cli add build -p 'sdkCmd'
```
### Design

The cli has three subcommends:
- app for dealing with an OpenCAXPlus program, such as build, doc, run, test, pack 
- plugin for dealing with an OpenCAXPlus plugin, such as build, doc, test, pack
- tool for dealing with OpenCAXPlus command line tools, such as lint, install, update, remove, etc.

### Folder structure

- cmd contains the cli main package, parsing the arguments etc., each subcommand get its own go file 
- pkg contains the actual functions for doing the hard work, each subcommand is a package within its own folder 
- docs contains the documentation
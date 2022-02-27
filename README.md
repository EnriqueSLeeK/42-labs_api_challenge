
# API
## Introduction
This is a simple REST api written in C for the 42 labs selection

## Dependencies
- Postgresql 13.6
- Mongoose Network Library
- Mjson
- gcc (GNU compiler)

## Installing the dependencies
Some of them might not be necessary to install if they are present in your system.
### Arch linux
Installing postgresql and the library required to do the connection:
```
pacman -S postgresql
```
If for some reason the library is not bundled with the package do:
```
pacman -S postgresql-libs
```
Installing basic development tools (like gcc and make):
```
pacman -S base-devel
```
### Ubuntu

```
sudo apt install postgresql
```


## Endpoints
### Available operations
|path|GET|POST|DELETE|PATCH|PUT|
|---|---|---|---|---|---|
|/|O|X|X|X|X|
|/channel|O|O|O|X|X|
|/channel/:channel_id|O|X|O|O|O|
|/channel/:channel_id/:video_id|O|X|O|O|O|
|/videos|O|O|O|X|X|
|/videos/:video_id|O|X|X|X|X|


# CLI
## Introduction
A CLI tool that will make a summary of the api log file

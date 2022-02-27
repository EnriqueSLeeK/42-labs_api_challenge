# Project documentation

## API
### Introduction
This is a simple REST api written in C

### Dependencies
- Postgresql 13.6
- Mongoose Network Library
- Mjson
- ANSI-C


### Endpoints
|path|GET|POST|DELETE|PATCH|PUT|
|---|---|---|---|---|---|
|/|O|X|X|X|X|
|/channel|O|O|X|X|X|
|/channel/:channel_id|O|X|O|O|O|
|/channel/:channel_id/:video_id|O|X|O|O|O|
|/videos|O|O|X|X|X|
|/videos/:video_id|O|X|X|X|X|


## CLI
### Introduction
A CLI tool that will make a summary of the api log file

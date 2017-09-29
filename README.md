# Get Window by Name

## Building

To compile the extension for the first time, run 

```
$ npm i
$ npm run configure
$ npm run build
```

All subsequent builds only need `npm run build`

## Compatibility

Currently, this only runs on Windows. Linux support is being considered. macOS support will probably never come, since I'm not even sure anything on macOS actually sets a window title (I'll happily add macOS support if someone proves me wrong).

## Usage

Just require the module

```
const test = require('./NativeExtension.node');
```

and use the (only) `getWindowText` function

`getWindowText([executableName])`:
- executableName (optional): the name of the executable that you want the info for. Omitting this argument will return every process

- Returns an `Array` of processes that match the argument

`process` Object structure:
```
{
    processId: Number,
    processName: String,
    processTitle: String
}
```

## Todo (pull requests welcome!)

- Add Linux support
- Fix tests
- Add automatic build server
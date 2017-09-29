# Get Window by Name

## Building

To compile the extension for the first time, run 

```
$ npm i
$ npm run configure
$ npm run build
```

All subsequent builds only need `npm run build`


## Usage

Just require the module

```
const test = require('./NativeExtension.node');
```

and use the (only) `getWindowText` function

`getWindowText([executableName])`:
- executableName: the name of the executable

- Returns an `Array` of processes that match the argument

`process` Object structure:
```
{
    processId: Number,
    processName: String,
    processTitle: String
}
```
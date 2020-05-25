Todo Table
------
```
╭─────────┬────┬────────────────────┬────────────╮
│ User ID │ ID │ Title              │ Completed? │
├─────────┼────┼────────────────────┼────────────┤
│ 1       │ 1  │ delectus aut autem │ No         │
╰─────────┴────┴────────────────────┴────────────╯
```
<sup>*Output example.*</sup>

This project was made for a C challenge. It gets a list of todos from [jsonplaceholder.typicode](https://jsonplaceholder.typicode.com/todos) and renders on a table. It uses [JSON-C](https://github.com/json-c/json-c) and [libcurl](https://curl.haxx.se/libcurl/) as dependencies.

## Dependencies
In order to compile on *Ubuntu*, the following dependencies can be installed through `apt`:

- `build-essential`
- `cmake`
- `libcurl4-openssl-dev` 


## Utility Scripts
The project have three utility scripts, that must be invoked from the source root using `script/[name]`:

##### build
Runs `cmake` at project root and then builds `with` make inside `./build`.

##### run
Runs `build` command and simply runs the generated binary at `dist/main`.

##### clean
Removes cache files from `build/*` and build artifacts from `dist/*` folders.

## Supported Architectures
This project only supports x86_64. There's no intention to support other architectures.

## License

```
Copyright 2020 Lucas Coelho

Permission is hereby granted, free of charge, to any person obtaining a copy 
of this software and associated documentation files (the "Software"), to deal 
in the Software without restriction, including without limitation the rights to 
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of 
the Software, and to permit persons to whom the Software is furnished to do so, 
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all 
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN 
THE SOFTWARE.

```

Todo Table
------

<sup>*Output example.*</sup>
```
╭─────────┬────┬────────────────────┬────────────╮
│ User ID │ ID │ Title              │ Completed? │
├─────────┼────┼────────────────────┼────────────┤
│ 1       │ 1  │ delectus aut autem │ No         │
╰─────────┴────┴────────────────────┴────────────╯
```
This project was made for a C challenge. It gets a list of todos from [jsonplaceholder.typicode](https://jsonplaceholder.typicode.com/todos) and render it in listed in a table. The project uses [JSON-C](https://github.com/json-c/json-c) and [libcurl](https://curl.haxx.se/libcurl/) as dependencies.

The project have three utility scripts:
-----
#### build
Runs cmake at project root and then build with make inside `./build` folder.

#### run
Runs *build* command and simply run the generated binary at `dist/main`.

#### clean
Removes every cache files from `build/*` and `dist/*` folders.

**Disclaimer**: One might create a `build/` directory before running one of the scripts above.

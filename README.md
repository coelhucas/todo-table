Todo Table
------

<sup>*Output example.*</sup>

![image](https://user-images.githubusercontent.com/28108272/82719956-5b9fbc80-9c85-11ea-88bb-2b1aa37278cc.png)

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

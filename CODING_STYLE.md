# Coding style

--- 
The coding style practices apply only to development on TDL itself. It is not mandatory for projects that depend on TDL.

## Coding style

---

The coding style is inspired by the [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html), with some additional modifications. If some rules are not detailed here, please refer to the Google C++ Style Guide.

## Code practices

---

### File organization

- Each class must be in a separate file.
- The class declaration is separated from the class implementation.
- The class declaration is in a `.hpp` file which must be located in the `Include` directory.
- The class implementation is in a `.cpp` file which must be located in the `Source` directory.
- The `.cpp` file must include the corresponding `.hpp` file.
- Folder and file names must be written using the `PascalCase` form (e.g. `MyClass.hpp`).

### Include guards

- Each `.hpp` file must have an include guard.
- The include guard must define a macro that follows the following syntax:
  ```cpp
  #ifndef TDL_<FOLDER_NAME>_<FILE_NAME>_HPP
  #define TDL_<FOLDER_NAME>_<FILE_NAME>_HPP`
  
  // Your code here
  
  #endif
- The macro fields specified above must be written in the `UPPER_CASE` form.

### Namespaces

- Each class must be in the `tdl` namespace.
- The namespace must be in lowercase. (e.g. `namespace tdl`).
- Do not use any `using namespace` statement in header or source files.

### Classes

- The class name must  be in `PascalCase`. (e.g. `MyClass`).
- The class should have a constructor and a destructor where appropriate.
- Classes should respect the single responsibility principle.
- Here is an example of a class declaration that follows the coding style:
```cpp
namespace tdl {
    class MyClass {
    public:
        MyClass();
        ~MyClass();
        
        void myFunction();
        
    private:
        int _myVariable;
    };
}
```

### Functions

- Function names should be in `camelCase`. (e.g. `myFunction()`).
- Functions have to respect the one responsibility principle.
- Promote the use of static functions where applicable.
- Write short functions and Keep It Simple, Stupid (KISS).

### Variables

- Variable names should be in `camelCase`. (e.g. `myVariable`).
- Private variable names should be prefixed with an underscore. (e.g. `_myVariable`).
- Variable names should be explicit.
- Promote the use of `const` variables when possible.
- Promote the use of getter and setter functions.

### Comments and Doxygen

- Write useful comments.
- Use the Doxygen format to document classes and functions.
- Doxygen comments should use the following format:

```cpp
/**
 * @brief This is a brief description of the function.
 * 
 * This is a more detailed description of the function.
 * 
 * @param myParam This is a description of the parameter.
 * @return This is a description of the return value.
 */
```

- Additional documentation regarding Doxygen-style comment can be found [here](https://www.doxygen.nl/manual/docblocks.html).
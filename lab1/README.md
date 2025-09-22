# Lab work 1 and multithreading
## Author
Журавлев Александр Антонович 24.Б81-мм
## Contacts
st129760@student.spbu.ru

## Overview

This project delivers a comprehensive set of C++ tools for efficient BMP image manipulation, encompassing core functionalities like loading, saving, 90-degree rotations (both clockwise and counter-clockwise), and applying a Gaussian blur filter. The implementation leverages multithreading to optimize performance for image processing tasks, making it suitable for handling larger images more efficiently.


### Using Makefile

```sh
make
```

### Running the Decoder
To start the programm, run the compiled executable:

```sh
make
./bin/read_BMP <path to BMP file>    # To start the program with your bmp-file
```

or

```sh
make
make run    # To start the program with 'example.bmp'
```
#### Running Tests Locally
To run tests locally, you can use the following commands:

```sh
make
make test    # Runs tests for all programm
```

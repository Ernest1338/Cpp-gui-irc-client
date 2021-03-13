# file structure
```
|-LICENSE
|-README.md
|-plan.md
|-.gitignore (/build)
|-Makefile
|-src
--|-main.cpp
  |-irc.cpp
  |-irc.h
  |-gui.cpp
  |-gui.h
|-build
--|-app
  |-*.o
```

# build system
- make (Makefile)
- g++

# dependencies
- Boost C++ libraries
- qt
- g++
- GNU make

# TODOs
- network communication over specified port (something like netcat connection)
- configure sudo make install
- remove header decoding in incoming traffic (each incomming message is parsed based on the header (length). need to change that to: newline (\n) as a message delimiter)
- ...

# Milestones
- [ ] Communicating with IRC servers throuh CLI
- [ ] Basic GUI working propertly
- ...

# Building the Y86-64 & SEQ tools on MacOS

The following instruction worked for an Apple M1 Pro machine

1. Install [Homebrew](https://brew.sh/)
2. Install flex by running `brew install flex`
3. Install XCode (using the App store on you mac)
   \* Make sure to open it for the first time and click "Install" to install additional components
4. run `xcode-select --install`
5. Download "Y86-64 tools and documentation" from [CS:APP3e Instructor Site](http://csapp.cs.cmu.edu/3e/instructors.html) (The link at the time of writing this readme is http://csapp.cs.cmu.edu/3e/sim.tar)
6. Untar with `tar -xvf sim.tar`
7. Edit sim/misc/Makefile
   Update the LEXLIB value to include the flex library
    ```
    LEXLIB = -L/opt/homebrew/Cellar/flex/2.6.4_2/lib/ -lfl
    ```
    \* Note that the version (`2.6.4_2` in this example) might be different for your installation
8. create /usr/local/include folder - `sudo mkdir /usr/local/include`
9. create symlink to X11 header files
   ```
   sudo ln -s /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/System/Library/Frameworks/Tk.framework/Versions/8.5/Headers/X11 /usr/local/include/X11
   ```
10. `make clean && make`


Optional steps (Might help for some):
- Install tcl-tk - `brew install tcl-tk`
- Install X11 - `brew install libx11`
- Run `make clean && make` again


For more info and simulators guide check - http://csapp.cs.cmu.edu/3e/simguide.pdf
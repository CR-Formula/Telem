# Cyclone Racing Formula SAE
## Iowa State University

This Repository stores the embedded code for our real-time telemetry system. The system is built off of an STM32H7 MCU that pulls data from the ECU and other sensors around the car and sends it in real-time to a wireless receiver, and logs it locally to an SD card. The system uses FreeRTOS to schedule telemetry-related tasks as well as ARM's CMSIS headers to interface with registers on the MCU. Most of the example code is built for a NUCLEO test board, while the actual system runs a custom PCB.

## Useful Links
- [Git Cheat Sheet](https://education.github.com/git-cheat-sheet-education.pdf)
- [C Struct Packing](http://www.catb.org/esr/structure-packing/)
- [STM32 Wiki](https://wiki.stmicroelectronics.cn/stm32mcu/wiki/Category:Getting_started_with_STM32_:_STM32_step_by_step)
- [Embedded Systems Design](https://iowastate.sharepoint.com/sites/CycloneRacingFormulaSAE/Shared%20Documents/Forms/AllItems.aspx?id=%2Fsites%2FCycloneRacingFormulaSAE%2FShared%20Documents%2FElectrical%2FResources%2FEmbedded%20Systems%20Introduction%20to%20Arm%20Cortex%2DM%20Microcontrollers%20Fifth%20Edition%2Epdf&parent=%2Fsites%2FCycloneRacingFormulaSAE%2FShared%20Documents%2FElectrical%2FResources)
- [Basic Linux Guide](https://www.tecmint.com/free-online-linux-learning-guide-for-beginners/)
- [C Program Memory](https://www.geeksforgeeks.org/memory-layout-of-c-program/)

## Dev and Build Tools
The Firmware for this system is developed in WSL using Arm compilers and make files. To get started, follow the steps below.

1. Install WSL by opening up a Powershell terminal and running the command `wsl --install -d Ubuntu`. This command will set up WSL using an Ubuntu Linux distribution. It will ask for you to create a username and password to complete the setup. Once installed, you can access the Ubuntu terminal by searching for Ubuntu in Windows, or it will be available in VSCode once the setup is complete.

2. Install [usbipd](https://github.com/dorssel/usbipd-win/releases) from the releases page using the .msi file.

3. Install VSCode for Windows by going to the [VSCode download page](https://code.visualstudio.com/download) and select the correct package for your system. Follow the directions in the installer.

4. Next, we will link VSCode to WSL using the [WSL Extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-wsl). Navigate to the extensions tab in VSCode and search for WSL. VSCode has [detailed instructions](https://code.visualstudio.com/docs/remote/wsl) on how to set this up and how to open a remote connection to WSL.

5. Next, we will install a series of build tools that are needed to compile the firmware. Open up a WSL terminal and run the following commands:
`sudo apt update`
`sudo apt upgrade`
`sudo apt install git make gcc-arm-none-eabi gdb-multiarch stlink-tools`
Here is a short explanation of each of these tools:
    - Git: repository version control
    - Make: a tool that helps manage software project compilation and building process.
    - GCC-Arm-none-eabi: embedded Arm-specific build tools and compiler.
    - GDB-Multiarch: adds support for microcontroller debugging
    - STLink-Tools: ST-specific tools for developing and uploading embedded code
7. Finally, we will need to install a handful of VSCode extensions, these give us some tools to upload code to the boards as well as see registers and variables while debugging. These extensions are included in [extensions.json](https://github.com/Cyclone-Racing/CR28_Telem/blob/main/.vscode/extensions.json) file in the `.vscode` directory on the `main` branch.

Once you've completed these steps, you can start developing firmware. The best place to start will be the Microcontroller Reference Manual. Read through the section about the peripheral you are trying to work with.

## Building code
In order to build the code, use Make and Makefiles. Open up an Ubuntu terminal and navigate to the project directory. Next, run the command `make` in the terminal. This will build the code using the instructions in the makefile. Make will only build files you change; if you would like to rebuild the whole project, run the command `make clean` before running `make`.

## Passing a Device to WSL
In order to use the WSL enviroment, you'll have to pass the target board/debugger into WSL. This involves using the usbipd tool that we installed earlier.

1. If you are on Windows, open up Powershell as an administrator and run `usbipd list`. This will list all of your current USB devices on your computer. Look for the STMircroelectronics device and note the BusID in the left column.

2. If you have never connected this board/debugger to WSL before, or if the ST Link device doesn't say Shared in the right column of the list, run the command `usbipd bind --busid <BUS_ID>` replace `<BUS_ID>` with the ID shown in the above list command. This will share the device to WSL.

3. Once the device is shared, you are able to attach it to WSL by running the command `usbipd attach --busid <BUS_ID> --wsl` again replacing `<BUS_ID>` with the ID of your device. To make sure that this command worked, you are able to open a WSL terminal and run the command `lsusb` which should now show the STMicroelectronics device.

## Uploading and Debugging the Code
To debug and upload code, we use the Cortex-Debug extension. This extension uses 2 configuration files in the .vscode folder: settings.json and launch.json. Basic versions of these files are provided in the repository and can be modified if needed. You must build the code using the `make` command at least once before you attempt to use the Cortex-Debug configuration.

    - settings.json has 3 important keys: cortex-debug.armToolchainPath, cortex-debug.gdbPath, and cortex-debug.stutilPath. All of these file paths are universal in WSL/Ubuntu and should be the same if you setup your dev enviroment by following the directions above.
    - launch.json configures the debugging profile for Cortex-Debug. The default values in this file should also work well apart from two: svdFile and device. svdFile is the path to an svd file for the microcontroller that you are using and svd file tells the debugger what peripherials are at what memory addresses and allows for easier debugging. The device will need to match the microcontroller on whichever board you are debugging.

1. Once these files are configured, go to the run and debug window on the left column in VSCode (Looks like a play button with a bug) and you should see an option called Cortex Debug. With the Cortex Debug option selected press the green play button.

2. This will create a debug menu at the top of the screen to control the debug session which gives you options such as step over, step into, and step out as well as reset and end session.

3. At the bottom of the left column, there is also a xPeripherials option. These are the register values of the device you are debugging and are very useful to find issues.

4. Finally, you are able to set breakpoint by clicking the red dot next to the line number you want to place the breakpoint at. This will stop the code while it is running so that you can look at the register values.

# Links
- [Linktree](https://linktr.ee/cycloneracing)
- [LinkedIn](https://www.linkedin.com/company/cyclone-racing/)
- [Facebook](https://www.facebook.com/CycloneRacingUS/)
- [Twitter](https://twitter.com/cycloneracingus?lang=en)
- [TikTok](https://www.tiktok.com/@cycloneracing)
- [Instagram](https://www.instagram.com/cycloneracingus/)
- [Youtube](https://www.youtube.com/channel/UCQaE_Bqq185kTRbl6uPepTg/videos)

## License

GNU GPLv3

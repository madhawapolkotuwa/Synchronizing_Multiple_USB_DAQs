## Synchronizing Multiple USB NI Data Acquisition Devices using ANSI C and NI-DAQmx

This project provides a Visual Studio solution for synchronizing and acquiring data from multiple USB National Instruments (NI) Data Acquisition (DAQ) devices using ANSI C and the NI-DAQmx driver. The solution includes code for synchronizing two USB-DAQ devices, each equipped with 8 analog input channels. The acquired data from all 16 analog input channels is saved to a file for further analysis.

![intro](https://github.com/madhawapolkotuwa/Synchronizing_Multiple_USB_DAQs/assets/32560614/aa6cc436-c015-4d94-94e5-50e4abf3d6b6)


### Features

- Synchronizes data acquisition between two USB NI DAQ devices
- Acquires data from 16 analog input channels (8 channels per device)
- Saves the acquired data to a file for subsequent analysis

### Requirements

- Two USB NI DAQ devices (compatible all ni-usb DAQ models)
- Visual Studio (tested with [2015])
- NI-DAQmx driver software 

### Usage

1. Connect both USB NI DAQ devices to your computer.
2. Install the NI-DAQmx driver software ([download link](https://www.ni.com/en-gb/support/downloads/drivers/download.ni-daq-mx.html#480879)) if it is not already installed.
3. Open the Visual Studio solution provided.
4. Build the solution to generate the executable.
5. Run the executable.
6. The program will synchronize the two devices and initiate data acquisition from all 16 analog input channels.
7. The acquired data will be saved to a specified file format (e.g., CSV, TXT) for further analysis.

### Device Compatibility

This code has been tested with the following USB NI DAQ devices:

- Two : [USB-6001](https://www.ni.com/en-gb/support/model.usb-6001.html)
- Two : [USB-6008](https://www.ni.com/en-gb/support/model.usb-6008.html)

Note: The code can be adapted to work with other USB NI DAQ devices by modifying the device-specific configurations and settings.

### Contributions

Contributions to this project are welcome. If you encounter any issues or have suggestions for improvements, please feel free to open an issue or submit a pull request.

### License

This project is licensed under the [MIT License](LICENSE).

### Acknowledgments

- The code in this project draws inspiration from [source/reference].
- Special thanks to [contributor/acknowledgment] for their valuable contributions.

### References

- [NI-DAQmx driver documentation](https://www.ni.com/en-in/support/documentation/drivers-and-updates/drivers/ni-daqmx.html)

Please make sure to update the placeholders in square brackets with the relevant information specific to your project.

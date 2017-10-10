### logfile_iterator_for_video_device

This example is based on the logfile_iterator example. It differs in that it provides a concrete example of iterating over video device data stored as a `ps_image_data_msg` in a `plog` file.

Within the logfile iterator callback, this example shows how to use the ps_image_data_msg to convert raw, non-abstracted `plog` data to
individual image files.

### Requirements

This example requries that the image data was logged with YUYV pixel formatting. This is the default format for video-device nodes defined in the SDF. To verify the logged pixel format, check the SDF entry for the device.

### Dependencies

To install on Ubuntu:

```bash
sudo apt install libusb-1.0
sudo apt install libpopt-dev
```
If you're on an amb64 Ubuntu 16.04 machine you're good to go.

Otherwise: we've got to build this one ourselves
- libuvc
```bash
git clone https://github.com/ktossell/libuvc.git
cd libuvc
git checkout 77b43d6
mkdir build
cd build
cmake ..
make
cp libuvc.a /path/to/logfile_iterator_for_video_device/lib
```

### Building

```bash
$ cd logfile_iterator_for_video_device
$ make
```

### Running
```bash
$ ./bin/polysync-logfile-iterator-for-video_device-c -p <PATH> -o <PATH> -f <FORMAT>
```
### Example Usage
```bash
$ ./bin/polysync-logfile-iterator-for-video_device-c -p ~/.local/share/polysync/rnr_logs/70802/video-device.1688895556945937.plog
```
This will write `.ppm` image files generated from the video-device log in session 70802 to a `/tmp/plog_images.XXXXXX` where `XXXXXX` represents a random string.

### Usage Details
```bash
Usage: polysync-logfile-iterator-for-video-device-c [OPTION...]
  -p, --path=PATH         required, path to video-device plog
  -o, --outdir=PATH       specifiy image file output directory path, defaults
                          to /tmp/plog_images.XXXXXX where `XXXXXX` represents
                          a random string
  -f, --format=FORMAT     specifiy file format 'bmp' or 'ppm', defaults to
                          'ppm'

Help options:
  -?, --help              Show this help message
      --usage             Display brief usage message
```

For more API examples, visit the "Tutorials" and "Development" sections in the PolySync Help Center [here](http://docs.polysync.io/articles/).

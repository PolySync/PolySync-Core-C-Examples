### serial_writer

You would use this as a reference if you needed a node to talk directly to a device over a serial connection.

This example creates and writes data to a serial connection, to a hardware device, or to another software application.

You can use this with the `serial_reader` example.

### Dependencies

Packages: libglib2.0-dev

To install on Ubuntu: 

```bash
sudo apt-get install libglib2.0-dev
```

### Building and running the node

```bash
$ cd serial_writer
$ make
$ ./bin/polysync-serial-writer-c 
```

For more API examples, visit the "Tutorials" and "Development" sections in the PolySync Help Center [here](https://help.polysync.io/articles/).

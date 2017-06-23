
### can_reader

Using the PolySync state machine, this example performs a simple CAN read in the OK state.
This example also acts as a guide for when you need a node to listen to a CAN channel.

### Hardware requirements

- linuxcan-compatible hardware is connected, i.e. a Kvaser Leaf Light

### Dependencies

Packages: libglib2.0-dev

To install on Ubuntu: 

```bash
sudo apt-get install libglib2.0-dev
```

### Building and running the node

```bash
$ cd can_reader
$ make
# You need to have an CAN channel to run this example see:  [Configure CAN Channels](http://docs.polysync.io/articles/configuration/runtime-node-configuration/configure-can-channels/)
$ ./bin/polysync-can-reader-c 1
```

For more API examples, visit the "Tutorials" and "Development" sections in the PolySync Help Center [here](https://help.polysync.io/articles/).

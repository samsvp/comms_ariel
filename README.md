comms_ariel
======================

Ariel's USV and Drone communication package.

Dependencies
======================

Download Protobuf from https://github.com/protocolbuffers/protobuf/releases, and extract the file. Then go to the Protobuf directory you just extracted and run:


    $ ./configure
    $ make
    $ make check
    $ sudo make install
    $ sudo ldconfig

Install
======================
    $ catkin_make

Run
======================

To check if everything is ok run:

    $ build/comms_ariel/main

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

To test if everything is ok, initialize the server:

    $ build/comms_ariel_server/main_server
    
In the client, in the "main()" function write the IP of the server and run:

    $ build/comms_ariel_client/main_client
    
If everything is fine you should see the same message printed on the client and server terminal's.

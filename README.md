# ClientServerApp

  This project is the implementation of cross-platform Client-Server communication, it is written using ACE (Adaptive Communication Enviroment) 7.0.0 framework.
The project consists of the multithreaded server with TCP and a thread-pool strategy and the client. 
The main idea of communication is in transferring encoded data of arbitrary length from a client to the server, so it is simplistically implemented in the following way:
client encodes length of data with TLV(Tag-Length-Value) rules for length and sends it to the server, then server decodes the length value, allocates a certain memory size (of this length) for data and then recieves data of this size.

To run the client and the server you have to do following:
- Obtain ACE 7.0.0 from https://download.dre.vanderbilt.edu/.
- Build/use shared lib version by specifying your paths to library in "CMakeLists.txt" of Server and Client.
- After successful building copy "ACEd.dll" from ../ACE_wrappers/lib to each executable file directory.

Now you can run executable files of Server and Client!

Building:

1) cmake .
2) make

Project Name: 330chat
  
Project Repo: https://github.com/taiseiak/330chat.git

Team Members: Taisei Klasen, Michael Hagel, Jackson VanVuren, Daniel Van Cleave

Description: A chat room application for UNIX. Users can create and host a room, and join to chat in real time.

Implementation approach: Start small by creating a chat room application
with a server and host with IRC. Expand later by creating our own protocol and/or GUI.

Risk Management: Since there are existing protocols and making the base chat application should be easy,
the risk management is to have a working product that is simple and reliable before moving into anything
that is remotely complex. If we cannot finish the complex features, we will have a working product underneath.

### Individual Contributions:
<b>Michael Hagel</b>: Iterated on the Tcp server. Wrote code to initialize the server, facilitate reading and writing with custom
signals and slots, as well as handle all new client connections and deal with their read/write requests and the storage of messages/live
connections and removal on disconnect. Worked with the GUI guys to hook up each client to the server and enable them to communicate. 
Note: I didn't work on the file sharing stuff. All credit for that goes to Jackson.


# HOW TO DEPLOY AND USE THE PROGRAM

## IMPORTANT: PLEASE HAVE QT INSTALLED ON YOUR SYSTEM. QT 5.9.4 OR ABOVE.

Note:
  If you would like to bypass this, link Qt on a system with Qt statically and then run qmake with the static tag in the CONFIG.

## Creating on IX

Step 1) clone the repository

Step 2) cd into the directory tcpServer

Step 3) in bash run $ qmake tcpServer.pro, then $ make

Step 4) run $ ./tcpServer

### Now the server is running. Follow next steps to get GUI running

Step 6) Quit the server (ctrl-c), and cd into the previous directory

Step 7) cd into 330chatGUI

Step 8) run $ qmake 330chatGUI.pro, then $ make

Step 9) run $ ./330chatGUI

Note: this will not work since there is no display on ix.

## On other systems WITH QT 5.9 OR ABOVE
Run the same steps as ix, but with a display and the GUI will work

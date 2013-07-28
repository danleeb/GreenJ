### Versions

Current version: **1.1**

Old versions: see branches

# About GreenJ

GreenJ is an open source Voice-over-IP phone software using pjsip and Qt. It can easily be used to build your own VoIP phone system. Our approach was not to build a complete phone with user interface, but instead provide an application that handles only the communication. The program logic and user interface are separated from the application by using an integrated browser. We use webkit as browser engine, which is well integrated into Qt (QWebView). A Javascript interface handles all communications between application and webpage. This means that you can use GreenJ as it is and create your VoIP phone entirely in HTML and JavaScript.

Visit http://www.greenj.org for further information, binary downloads and an introduction to the javascript library.

# Build GreenJ

For a complete overview, see [building GreenJ under Windows or Linux](http://www.loremipsum.at/produkte/greenj/introduction/#builden)

In addition, I want to give a step by step guide on how GreenJ can be compiled under Ubuntu.

## Ubuntu (12.04 32-bit Desktop)

_Note: Also tested under Ubuntu 11.10 32-bit Desktop with the exact same steps._

Install dependencies:

    $ sudo apt-get update
    $ sudo apt-get install build-essential
    $ sudo apt-get install libqt4-dev
    $ sudo apt-get install libasound2-dev
    $ sudo apt-get install libphonon-dev
    
You may also need libssl-dev (or disable the dependency in greenj.pro, search for -lssl)

[Download PJSIP](http://www.pjsip.org/download.htm) (e.g. pjproject-1.14.tar.bz2) and extract it somewhere.

    $ cd path/to/pjsip
    $ ./configure
    $ make dep & make

Now you can either install pjsip into your systemlibs or you have to set the path in the greenj project file later.

    $ sudo make install
    
Clone this repository or download and extract GreenJ from [greenj.org](http://www.greenj.org).

    $ git clone git://github.com/danleeb/GreenJ.git path/to/greenj

Open greenj.pro with an editor

    $ nano path/to/greenj/build/greenj.pro
    
and change the unix PJSIP_TARGET entry so it matches your system (e.g. i686-pc-linux-gnu). (You could use $ find path/to/pjsip -name '*.a').

If you haven't installed pjsip into your systemlibs (see above), you have to set PJSIP_DIR to path/to/pjsip.

Compile GreenJ:

    $ cd path/to/greenj/build
    $ qmake
    $ make
    $ ../bin/linux/GreenJ
    
A file ~/.greenj/settings.conf has been created where you can change settings.
